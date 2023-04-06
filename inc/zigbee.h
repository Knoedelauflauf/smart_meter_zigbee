/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ZB_DEVICE_CLUSTERS_H_
#define ZB_DEVICE_CLUSTERS_H_
/* Includes ------------------------------------------------------------------*/
#include <zboss_api.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Device endpoint, used to receive ZCL commands. */
#define SMZ_ENDPOINT_ID 10
#define SML_INIT_BASIC_POWER_SOURCE ZB_ZCL_BASIC_POWER_SOURCE_DC_SOURCE
/* Version of the application software (1 byte). */
#define SMZ_INIT_BASIC_APP_VERSION 01
/* Version of the implementation of the Zigbee stack (1 byte). */
#define SMZ_INIT_BASIC_STACK_VERSION 10
/* Version of the hardware of the device (1 byte). */
#define SMZ_INIT_BASIC_HW_VERSION 11
/* Manufacturer name (32 bytes). */
#define SMZ_INIT_BASIC_MANUF_NAME "Knoedelauflauf"
/* Model number assigned by manufacturer (32-bytes long string). */
#define SMZ_INIT_BASIC_MODEL_ID "SMZ_v0.1"
/* First 8 bytes specify the date of manufacturer of the device
 * in ISO 8601 format (YYYYMMDD). The rest (8 bytes) are manufacturer specific.
 */
#define SMZ_INIT_BASIC_DATE_CODE "20230404"
/* Describes the type of physical environment.
 * For possible values see section 3.2.2.2.10 of ZCL specification.
 */
#define SMZ_INIT_BASIC_PH_ENV ZB_ZCL_BASIC_ENV_UNSPECIFIED
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Initialization and Configuration functions  ******************************/
void start_zigbee();
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
#define REQUIRED_OBIS (ACTIVE_ENERGY_IMPORT_FLAG | CURRENT_ACTIVE_POWER_FLAG)
/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#endif // ZB_DEVICE_CLUSTERS_H_
