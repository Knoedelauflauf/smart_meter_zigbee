/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SML_PROCESSING_OBIS_HANDLER_H
#define SML_PROCESSING_OBIS_HANDLER_H
/* Includes ------------------------------------------------------------------*/
#include <zephyr/sys/atomic.h>

#include "sml/sml_list.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
typedef enum {
    ACTIVE_ENERGY_IMPORT_FLAG = 1 << 0,
    ACTIVE_ENERGY_EXPORT_FLAG = 1 << 1,
    CURRENT_ACTIVE_POWER_FLAG = 1 << 2,
    VOLTAGE_L1_FLAG = 1 << 3,
    VOLTAGE_L2_FLAG = 1 << 4,
    VOLTAGE_L3_FLAG = 1 << 5,
    CURRENT_L1_FLAG = 1 << 6,
    CURRENT_L2_FLAG = 1 << 7,
    CURRENT_L3_FLAG = 1 << 8,
    ANGLE_L1_FLAG = 1 << 9,
    ANGLE_L2_FLAG = 1 << 10,
    ANGLE_L3_FLAG = 1 << 11,
    FREQUENCY_FLAG = 1 << 12,
} sml_obis_flag_t;
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern atomic_t sml_obis_initialized;
/* Exported functions --------------------------------------------------------*/
void process_active_energy_import(sml_list *entry);
void process_current_active_power(sml_list *entry);
void process_voltage_l1(sml_list *entry);
void process_voltage_l2(sml_list *entry);
void process_voltage_l3(sml_list *entry);
void process_current_l1(sml_list *entry);
void process_current_l2(sml_list *entry);
void process_current_l3(sml_list *entry);
void process_frequency(sml_list *entry);
void process_default_handler(sml_list *entry);
void ignore_obis_value(sml_list *entry);
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




#endif  // SML_PROCESSING_OBIS_HANDLER_H
