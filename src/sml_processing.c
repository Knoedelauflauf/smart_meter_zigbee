/* Includes ------------------------------------------------------------------*/
#include "sml_processing.h"

#include <errno.h>
#include <math.h>
#include <sml/sml_crc16.h>
#include <sml/sml_file.h>
#include <stdio.h>
#include <string.h>
#include <zboss_api.h>
#include <zcl/zb_zcl_el_measurement.h>
#include <zcl/zb_zcl_metering.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/atomic_builtin.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/util.h>

#include "sml_processing_obis_handler.h"
#include "zb_config_platform.h"
#include "zephyr/toolchain/gcc.h"
/* LOG channel def -----------------------------------------------------------*/
LOG_MODULE_REGISTER(sml_processing, LOG_LEVEL_WRN);
/* Thread Storage def --------------------------------------------------------*/
K_THREAD_STACK_DEFINE(sml_parsing_thread_stack, 2048);
/* Private typedef -----------------------------------------------------------*/
typedef struct {
    const uint8_t OBIS[6];
    const char *description;
    void (*process_value)(sml_list *entry);
} obis_mapping_t;
/* Private define ------------------------------------------------------------*/
/* change this to any other UART peripheral if desired */
#define UART_DEVICE_NODE DT_ALIAS(uart)
#define SML_START_SEQUENCE_LENGTH 8
#define SML_START_SEQUENCE "\x1B\x1B\x1B\x1B\x01\x01\x01\x01"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);
/* Private variables ---------------------------------------------------------*/
static uint8_t sml_msg_buffer[SML_RX_BUFFER_SIZE];
static size_t sml_msg_buffer_pos;
static atomic_t processing_thread_running = 0;
static struct k_sem sml_msg_received_sem;
static struct k_thread sml_parsing_thread_data;
static const obis_mapping_t obis_mappings[] = {
    {{0x01, 0x00, 0x01, 0x08, 0x00, 0xff}, "Active Energy Import", process_active_energy_import},
    {{0x01, 0x00, 0x10, 0x07, 0x00, 0xff}, "Current Active Power", process_current_active_power},
    {{0x01, 0x00, 0x20, 0x07, 0x00, 0xff}, "Voltage L1", process_voltage_l1},
    {{0x01, 0x00, 0x34, 0x07, 0x00, 0xff}, "Voltage L2", process_voltage_l2},
    {{0x01, 0x00, 0x48, 0x07, 0x00, 0xff}, "Voltage L3", process_voltage_l3},
    {{0x01, 0x00, 0x1F, 0x07, 0x00, 0xff}, "Current L1", process_current_l1},
    {{0x01, 0x00, 0x33, 0x07, 0x00, 0xff}, "Current L2", process_current_l2},
    {{0x01, 0x00, 0x47, 0x07, 0x00, 0xff}, "Current L3", process_current_l3},
    {{0x01, 0x00, 0x0E, 0x07, 0x00, 0xff}, "Frequency", process_frequency},

    {{0x01, 0x00, 0x02, 0x08, 0x00, 0xff}, "Active Energy Export", ignore_obis_value},
    {{0x01, 0x00, 0x51, 0x07, 0x04, 0xff}, "Angle L1", ignore_obis_value},
    {{0x01, 0x00, 0x51, 0x07, 0x0F, 0xff}, "Angle L2", ignore_obis_value},
    {{0x01, 0x00, 0x51, 0x07, 0x1A, 0xff}, "Angle L3", ignore_obis_value},
    {{0x01, 0x00, 0x00, 0x02, 0x00, 0x00}, "LR-Firmware Version", ignore_obis_value},
    {{0x01, 0x00, 0x00, 0x02, 0x00, 0x01}, "NLR-Firmware Version", ignore_obis_value},
    {{0x01, 0x00, 0x60, 0x5A, 0x02, 0x01}, "LR-Firmware CRC", ignore_obis_value},
    {{0x01, 0x00, 0x60, 0x5A, 0x02, 0x02}, "NLR-Firmware CRC", ignore_obis_value},
    {{0x01, 0x00, 0x60, 0x32, 0x01, 0x01}, "Power Threshold", ignore_obis_value},
    {{0x01, 0x00, 0x60, 0x01, 0x00, 0xff}, "Equipment Identifier", ignore_obis_value},
    // Add more OBIS mappings here as needed
};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
// Function to process SML files and print active energy OBIS numbers
void parse_sml_data(uint8_t *buffer, size_t buffer_size) {
    sml_file *file = sml_file_parse(buffer, buffer_size);
    if (!file) {
        LOG_ERR("Error parsing SML data.\n");
        return;
    }
    // sml_file_print(file);

    for (int i = 0; i < file->messages_len; i++) {
        sml_message *message = file->messages[i];
        if (*message->message_body->tag != SML_MESSAGE_GET_LIST_RESPONSE) {
            continue;
        }

        sml_get_list_response *resp = (sml_get_list_response *)message->message_body->data;
        for (sml_list *entry = resp->val_list; entry != NULL; entry = entry->next) {
            if (!entry->value) {
                LOG_ERR("Error in data stream. entry->value should not be NULL. Skipping this.\n");
                continue;
            }
            // Process the OBIS code in the value entry
            bool obis_handler_found = false;
            for (size_t i = 0; i < ARRAY_SIZE(obis_mappings); i++) {
                if (entry->obj_name->len != 6) {
                    LOG_ERR("Not len 6\n");
                    continue;
                }
                if (memcmp(entry->obj_name->str, obis_mappings[i].OBIS, 6) == 0) {
                    LOG_INF("%s: ", obis_mappings[i].description);
                    obis_mappings[i].process_value(entry);
                    obis_handler_found = true;
                    break;
                }
            }
            // Call the default handler if no matching OBIS handler is found
            if (!obis_handler_found) {
                process_default_handler(entry);
            }
        }
    }

    sml_file_free(file);
}

void uart_callback(const struct device *dev, void *user_data) {
    ARG_UNUSED(dev);
    ARG_UNUSED(user_data);
    static bool message_started = false;
    if (!uart_irq_update(uart_dev)) {
        return;
    }

    if (!uart_irq_rx_ready(uart_dev)) {
        return;
    }
    uint8_t byte;
    /* read until FIFO empty */
    while (uart_fifo_read(uart_dev, &byte, 1) == 1) {
        if (atomic_get(&processing_thread_running) == false) {
            sml_msg_buffer[sml_msg_buffer_pos++] = byte;

            // Search for the start of an SML message
            if (!message_started) {
                if (sml_msg_buffer_pos >= SML_START_SEQUENCE_LENGTH) {
                    if (memcmp(sml_msg_buffer + sml_msg_buffer_pos - SML_START_SEQUENCE_LENGTH, SML_START_SEQUENCE,
                               SML_START_SEQUENCE_LENGTH) == 0) {
                        message_started = true;
                        LOG_DBG("Message started.");
                    } else {
                        // Shift the buffer to search for the SML_START_SEQUENCE
                        memmove(sml_msg_buffer, sml_msg_buffer + 1, SML_START_SEQUENCE_LENGTH - 1);
                        sml_msg_buffer_pos--;
                    }
                }
            } else {
                // Check for the end of the SML message (Escape-Sequence and End of SML file)
                if (sml_msg_buffer_pos >= 8 &&
                    memcmp(sml_msg_buffer + sml_msg_buffer_pos - 8, "\x1b\x1b\x1b\x1b\x1a", 5) == 0) {
                    uint8_t padding = sml_msg_buffer[sml_msg_buffer_pos - 3];
                    if (sml_msg_buffer_pos >= (8 + padding)) {
                        LOG_DBG("Found end and Escape-Sequence.");
                        if (sml_crc16_calculate(sml_msg_buffer, sml_msg_buffer_pos - 2) ==
                            sys_get_be16(&sml_msg_buffer[sml_msg_buffer_pos - 2])) {
                            // Signal that a complete SML message has been received
                            LOG_DBG("Giving Semaphore. Padding: %u", padding);
                            atomic_set(&processing_thread_running, true);
                            k_sem_give(&sml_msg_received_sem);
                        } else {
                            LOG_DBG("CRC mismatch.");
                            sml_msg_buffer_pos = 0;
                        }
                        message_started = false;
                    }
                }
            }

            // If the buffer is full, reset the buffer position to prevent overflow
            if (sml_msg_buffer_pos >= SML_RX_BUFFER_SIZE) {
                LOG_ERR("SML msg buffer full.");
                sml_msg_buffer_pos = 0;
                message_started = false;
            }
        }
    }
}

void sml_parsing_thread(void *arg1, void *arg2, void *arg3) {
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);
    while (1) {
        LOG_DBG("Waiting for Semaphore.");
        k_sem_take(&sml_msg_received_sem, K_FOREVER);
        LOG_DBG("Parsing Thread started.");
        // Parse the SML message and process the result
        parse_sml_data(sml_msg_buffer + 8, sml_msg_buffer_pos - 16);
        // Reset buffer position for the next SML message
        sml_msg_buffer_pos = 0;
        atomic_set(&processing_thread_running, false);
        LOG_DBG("Parsing Thread ended.");
    }
}
/* Exported functions --------------------------------------------------------*/
int init_sml_processing() {
    if (!device_is_ready(uart_dev)) {
        LOG_ERR("UART device not found!");
        return ENODEV;
    }

    k_sem_init(&sml_msg_received_sem, 0, 1);

    /* configure interrupt and callback to receive data */
    uart_irq_callback_user_data_set(uart_dev, uart_callback, NULL);
    uart_irq_rx_enable(uart_dev);

    return 0;
}

void start_sml_processing() {
    k_thread_create(&sml_parsing_thread_data, sml_parsing_thread_stack, K_THREAD_STACK_SIZEOF(sml_parsing_thread_stack),
                    sml_parsing_thread, NULL, NULL, NULL, K_PRIO_COOP(7), 0, K_NO_WAIT);
    k_thread_name_set(&sml_parsing_thread_data, "SML Parser");
}
