/* Includes ------------------------------------------------------------------*/
#include "sml_processing_obis_handler.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <zephyr/logging/log.h>

#include "zigbee.h"
/* LOG channel def -----------------------------------------------------------*/
LOG_MODULE_REGISTER(sml_obis_handler);
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
#define MAX_ZB_INT_BYTES 6
typedef enum {
    ZB_UINT48,
    ZB_INT24,
    ZB_UINT16,
    ZB_INT16,
} zb_type_t;
// Precomputed powers of 10 and their inverses up to 10^9 (for int32_t range)
// If you need larger exponents, add more elements to the arrays.
static const double powers_of_10[] = {1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9};
static const double inv_powers_of_10[] = {1e0, 1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8, 1e-9};
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static double pow10(int8_t exp) {
    bool is_negative = exp < 0;
    uint8_t exp_abs = (is_negative) ? (uint8_t)-exp : (uint8_t)exp;

    if (exp_abs < sizeof(powers_of_10) / sizeof(powers_of_10[0])) {
        return (is_negative) ? inv_powers_of_10[exp_abs] : powers_of_10[exp_abs];
    }

    // Fallback to binary exponentiation for out-of-range exponents
    return pow(10, exp);
}

static void convert_to_zb_type(double value, zb_type_t type, void *result) {
    // Clip and convert the value based on the specified Zigbee type
    switch (type) {
        case ZB_INT24: {
            int32_t int24_value;

            if (value > 0x7FFFFF) {
                int24_value = 0x7FFFFF;
            } else if (value < -0x800000) {
                int24_value = -0x800000;
            } else {
                int24_value = (int32_t)value;
            }

            memcpy(result, &int24_value, 3);
            break;
        }
        case ZB_UINT16: {
            uint16_t uint16_value;

            if (value > UINT16_MAX) {
                uint16_value = UINT16_MAX;
            } else if (value < 0) {
                uint16_value = 0;
            } else {
                uint16_value = (uint16_t)value;
            }

            memcpy(result, &uint16_value, sizeof(uint16_t));
            break;
        }
        case ZB_INT16: {
            int16_t int16_value;

            if (value > INT16_MAX) {
                int16_value = INT16_MAX;
            } else if (value < INT16_MIN) {
                int16_value = INT16_MIN;
            } else {
                int16_value = (int16_t)value;
            }

            memcpy(result, &int16_value, sizeof(int16_t));
            break;
        }
        case ZB_UINT48: {
            uint64_t uint48_value;

            if (value > 0xFFFFFFFFFFFF) {
                uint48_value = 0xFFFFFFFFFFFF;
            } else if (value < 0) {
                uint48_value = 0;
            } else {
                uint48_value = (uint64_t)value;
            }

            memcpy(result, &uint48_value, 6);
            break;
        }
    }
}

void process_data(sml_list *entry, uint32_t data_type, uint32_t obis_flag, uint16_t zb_attr_id, uint16_t zb_cluster_id,
                  uint8_t zb_type, uint32_t multiplier) {
    if ((entry->value->type & SML_TYPE_FIELD) != data_type) {
        LOG_WRN("Unexpected Type.\n");
        return;
    }
    double value = sml_value_to_double(entry->value);
    int8_t scaler = (entry->scaler) ? *entry->scaler : 0;
    value = value * pow10(scaler) * multiplier;

    uint8_t zb_data[MAX_ZB_INT_BYTES] = {0};
    convert_to_zb_type(value, zb_type, &zb_data);

    ZB_ZCL_SET_ATTRIBUTE(SMZ_ENDPOINT_ID, zb_cluster_id, ZB_ZCL_CLUSTER_SERVER_ROLE, zb_attr_id, (zb_uint8_t *)&zb_data,
                         ZB_FALSE);
    atomic_or(&sml_obis_initialized, obis_flag);
}
/* Exported variables ---------------------------------------------------------*/
atomic_t sml_obis_initialized = 0;
/* Exported functions --------------------------------------------------------*/
void process_active_energy_import(sml_list *entry) {
    // Process and print imported active energy (OBIS 1.8.0). Unit: kWh
    process_data(entry, SML_TYPE_UNSIGNED, ACTIVE_ENERGY_IMPORT_FLAG,
                 ZB_ZCL_ATTR_METERING_CURRENT_SUMMATION_DELIVERED_ID, ZB_ZCL_CLUSTER_ID_METERING, ZB_UINT48, 1);
}

void process_current_active_power(sml_list *entry) {
    // Process and print current active power (OBIS 16.7.0). Unit: W
    process_data(entry, SML_TYPE_INTEGER, CURRENT_ACTIVE_POWER_FLAG, ZB_ZCL_ATTR_METERING_INSTANTANEOUS_DEMAND_ID,
                 ZB_ZCL_CLUSTER_ID_METERING, ZB_INT24, 1);
}

void process_voltage_l1(sml_list *entry) {
    // Process and print voltage value for phase L1 (OBIS 32.7.0). Unit: V
    process_data(entry, SML_TYPE_UNSIGNED, VOLTAGE_L1_FLAG, ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_ID,
                 ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT, ZB_UINT16, 100);
}

void process_voltage_l2(sml_list *entry) {
    // Process and print voltage value for phase L2 (OBIS 52.7.0). Unit: V
    process_data(entry, SML_TYPE_UNSIGNED, VOLTAGE_L2_FLAG, ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHB_ID,
                 ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT, ZB_UINT16, 100);
}

void process_voltage_l3(sml_list *entry) {
    // Process and print voltage value for phase L3 (OBIS 72.7.0). Unit: V
    process_data(entry, SML_TYPE_UNSIGNED, VOLTAGE_L3_FLAG, ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHC_ID,
                 ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT, ZB_UINT16, 100);
}

void process_current_l1(sml_list *entry) {
    // Process and print current value for phase L1 (OBIS 31.7.0). Unit: A
    process_data(entry, SML_TYPE_UNSIGNED, CURRENT_L1_FLAG, ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_ID,
                 ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT, ZB_UINT16, 100);
}

void process_current_l2(sml_list *entry) {
    // Process and print current value for phase L2 (OBIS 51.7.0). Unit: A
    process_data(entry, SML_TYPE_UNSIGNED, CURRENT_L2_FLAG, ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHB_ID,
                 ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT, ZB_UINT16, 100);
}

void process_current_l3(sml_list *entry) {
    // Process and print current value for phase L3 (OBIS 71.7.0). Unit: A
    process_data(entry, SML_TYPE_UNSIGNED, CURRENT_L3_FLAG, ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHC_ID,
                 ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT, ZB_UINT16, 100);
}

void process_frequency(sml_list *entry) {
    // Process and print the frequency value (OBIS 14.7.0). Unit Hz
    process_data(entry, SML_TYPE_UNSIGNED, FREQUENCY_FLAG, ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AC_FREQUENCY_ID,
                 ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT, ZB_UINT16, 100);
}

void process_default_handler(sml_list *entry) {
    // Add your processing logic here if necessary
    LOG_INF("Default Handler: OBIS code %d-%d:%d.%d.%d*%d not found or not implemented.\n", entry->obj_name->str[0],
            entry->obj_name->str[1], entry->obj_name->str[2], entry->obj_name->str[3], entry->obj_name->str[4],
            entry->obj_name->str[5]);
}

void ignore_obis_value(sml_list *entry) {
    ARG_UNUSED(entry);
    // Do nothing and ignore the OBIS value
}
