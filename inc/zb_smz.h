/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ZB_SMZ_H
#define ZB_SMZ_H
/* Includes ------------------------------------------------------------------*/
#include <zboss_api.h>
/* Exported types ------------------------------------------------------------*/
typedef struct {
    zb_uint32_t measurement_type;
    zb_uint16_t rmsCurrentPhA;
    zb_uint16_t rmsCurrentPhB;
    zb_uint16_t rmsCurrentPhC;
    zb_uint16_t rmsVoltagePhA;
    zb_uint16_t rmsVoltagePhB;
    zb_uint16_t rmsVoltagePhC;
    zb_uint16_t acFrequency;
    zb_uint16_t voltageMultiplier;
    zb_uint16_t voltageDivisor;
    zb_uint16_t currentMultiplier;
    zb_uint16_t currentDivisor;
    zb_uint16_t powerMultiplier;
    zb_uint16_t powerDivisor;
    zb_uint16_t frequencyMultiplier;
    zb_uint16_t frequencyDivisor;
    zb_uint16_t powerFactor;
} zb_zcl_el_measurement_attrs_ext_t;

typedef struct {
    zb_uint48_t curr_summ_delivered;
    zb_int24_t instantaneous_demand;
    zb_uint24_t multiplier;
    zb_uint24_t divisor;
} zb_zcl_metering_attrs_custom_t;
/* Exported constants --------------------------------------------------------*/
/** SMZ Device ID*/
#define ZB_SMZ_DEVICE_ID 0x0008
/** SMZ device version */
#define ZB_DEVICE_VER_SMZ 0
/** SMZ IN (server) clusters number */
#define ZB_SMZ_IN_CLUSTER_NUM 4
/** SMZ OUT (client) clusters number */
#define ZB_SMZ_OUT_CLUSTER_NUM 0
#define ZB_SMZ_CLUSTER_NUM (ZB_SMZ_IN_CLUSTER_NUM + ZB_SMZ_OUT_CLUSTER_NUM)
#define ZB_SMZ_EL_REPORT_COUNT 7
#define ZB_SMZ_METERING_REPORT_COUNT 2
/** Number of attribute for reporting on SMZ device */
#define ZB_SMZ_REPORT_ATTR_COUNT ZB_SMZ_EL_REPORT_COUNT + ZB_SMZ_METERING_REPORT_COUNT
/* Exported macro ------------------------------------------------------------*/
#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHB_ID(data_ptr) \
    {                                                                                           \
        ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHB_ID, ZB_ZCL_ATTR_TYPE_S16,           \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING, (void *)data_ptr       \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHC_ID(data_ptr) \
    {                                                                                           \
        ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACTIVE_POWER_PHC_ID, ZB_ZCL_ATTR_TYPE_S16,           \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING, (void *)data_ptr       \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHB_ID(data_ptr) \
    {                                                                                         \
        ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHB_ID, ZB_ZCL_ATTR_TYPE_U16,           \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING, (void *)data_ptr     \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHC_ID(data_ptr) \
    {                                                                                         \
        ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHC_ID, ZB_ZCL_ATTR_TYPE_U16,           \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING, (void *)data_ptr     \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHB_ID(data_ptr) \
    {                                                                                         \
        ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHB_ID, ZB_ZCL_ATTR_TYPE_U16,           \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING, (void *)data_ptr     \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHC_ID(data_ptr) \
    {                                                                                         \
        ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHC_ID, ZB_ZCL_ATTR_TYPE_U16,           \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING, (void *)data_ptr     \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AC_FREQUENCY_ID(data_ptr) \
    {                                                                                       \
        ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AC_FREQUENCY_ID, ZB_ZCL_ATTR_TYPE_U16,           \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING, (void *)data_ptr   \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_METERING_CURRENT_SUMMATION_RECEIVED_ID(data_ptr) \
    {                                                                                       \
        ZB_ZCL_ATTR_METERING_CURRENT_SUMMATION_RECEIVED_ID, ZB_ZCL_ATTR_TYPE_U48,           \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY | ZB_ZCL_ATTR_ACCESS_REPORTING, (void *)data_ptr   \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AC_FREQUENCY_MULTIPLIER_ID(data_ptr) \
    {                                                                                                  \
        ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AC_FREQUENCY_MULTIPLIER_ID, ZB_ZCL_ATTR_TYPE_U16,           \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY, (void *)data_ptr                                             \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AC_FREQUENCY_DIVISOR_ID(data_ptr) \
    {                                                                                               \
        ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AC_FREQUENCY_DIVISOR_ID, ZB_ZCL_ATTR_TYPE_U16,           \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY, (void *)data_ptr                                          \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_MULTIPLIER_ID(data_ptr) \
    {                                                                                               \
        ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_MULTIPLIER_ID, ZB_ZCL_ATTR_TYPE_U16,           \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY, (void *)data_ptr                                          \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_DIVISOR_ID(data_ptr)                     \
    {                                                                                                                \
        ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_DIVISOR_ID, ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY, \
            (void *)data_ptr                                                                                         \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_MULTIPLIER_ID(data_ptr) \
    {                                                                                               \
        ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_MULTIPLIER_ID, ZB_ZCL_ATTR_TYPE_U16,           \
            ZB_ZCL_ATTR_ACCESS_READ_ONLY, (void *)data_ptr                                          \
    }

#define ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_DIVISOR_ID(data_ptr)                     \
    {                                                                                                                \
        ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_DIVISOR_ID, ZB_ZCL_ATTR_TYPE_U16, ZB_ZCL_ATTR_ACCESS_READ_ONLY, \
            (void *)data_ptr                                                                                         \
    }

/** @brief Declare attribute list for Electrical Measurement cluster
        @param attr_list - attribute list name
        @param measurement_type - pointer to variable to store Measurement Type
     attribute value
        @param s - zb_zcl_el_measurement_attrs_ext_t struct
*/
#define ZB_ZCL_DECLARE_ELECTRICAL_MEASUREMENT_ATTRIB_LIST_EXT(                                                         \
    attr_list, measurement_type, rmsCurrentPhA, rmsCurrentPhB, rmsCurrentPhC, rmsVoltagePhA, rmsVoltagePhB,            \
    rmsVoltagePhC, acFrequency, voltageMultiplier, voltageDivisor, currentMultiplier, currentDivisor, powerMultiplier, \
    powerDivisor, frequencyMultiplier, frequencyDivisor, powerFactor)                                                  \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_ELECTRICAL_MEASUREMENT)                        \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_MEASUREMENT_TYPE_ID, (measurement_type))                   \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_ID, (rmsCurrentPhA))                            \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHB_ID, (rmsCurrentPhB))                        \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHC_ID, (rmsCurrentPhC))                        \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_ID, (rmsVoltagePhA))                            \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHB_ID, (rmsVoltagePhB))                        \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHC_ID, (rmsVoltagePhC))                        \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AC_FREQUENCY_ID, (acFrequency))                            \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_MULTIPLIER_ID, (voltageMultiplier))              \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACVOLTAGE_DIVISOR_ID, (voltageDivisor))                    \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_MULTIPLIER_ID, (currentMultiplier))              \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACCURRENT_DIVISOR_ID, (currentDivisor))                    \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_MULTIPLIER_ID, (powerMultiplier))                  \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_ACPOWER_DIVISOR_ID, (powerDivisor))                        \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AC_FREQUENCY_MULTIPLIER_ID, (frequencyMultiplier))         \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_AC_FREQUENCY_DIVISOR_ID, (frequencyDivisor))               \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_ELECTRICAL_MEASUREMENT_POWER_FACTOR_ID, (powerFactor))                            \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

#define ZB_ZCL_DECLARE_METERING_ATTRIB_LIST_CUSTOM(attr_list, curr_summ_delivered, instantaneous_demand, multiplier, \
                                                   divisor)                                                          \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_METERING)                                    \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_METERING_CURRENT_SUMMATION_DELIVERED_ID, (curr_summ_delivered))                 \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_METERING_INSTANTANEOUS_DEMAND_ID, (instantaneous_demand))                       \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_METERING_MULTIPLIER_ID, (multiplier))                                           \
    ZB_ZCL_SET_ATTR_DESC(ZB_ZCL_ATTR_METERING_DIVISOR_ID, (divisor))                                                 \
    ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST

/**
 * @brief Declare cluster list for SMZ device
 * @param cluster_list_name - cluster list variable name
 * @param basic_attr_list - attribute list for Basic cluster
 * @param identify_attr_list - attribute list for Identify cluster
 * @param el_measurment_attr_list - attribute list for electrical measurment
 * @param metering_attr_list - attribute list for metering
 * cluster
 */
#define ZB_DECLARE_SMZ_CLUSTER_LIST(cluster_list_name, basic_attr_list, identify_attr_list, el_measurment_attr_list, \
                                    metering_attr_list)                                                              \
    zb_zcl_cluster_desc_t cluster_list_name[] = {                                                                    \
        ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_METERING, ZB_ZCL_ARRAY_SIZE(metering_attr_list, zb_zcl_attr_t),        \
                            (metering_attr_list), ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_MANUF_CODE_INVALID),            \
        ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT,                                                \
                            ZB_ZCL_ARRAY_SIZE(el_measurment_attr_list, zb_zcl_attr_t), (el_measurment_attr_list),    \
                            ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_MANUF_CODE_INVALID),                                  \
        ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_IDENTIFY, ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t),        \
                            (identify_attr_list), ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_MANUF_CODE_INVALID),            \
        ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_BASIC, ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t),              \
                            (basic_attr_list), ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_MANUF_CODE_INVALID)}

/** @cond internals_doc */

/**
 * @brief Declare simple descriptor for SMZ device
 * @param ep_name - endpoint variable name
 * @param ep_id - endpoint ID
 * @param in_clust_num - number of supported input clusters
 * @param out_clust_num - number of supported output clusters
 */
#define ZB_ZCL_DECLARE_SMZ_SIMPLE_DESC(ep_name, ep_id, in_clust_num, out_clust_num) \
    ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num);                            \
    ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num)                             \
    simple_desc_##ep_name = {ep_id,                                                 \
                             ZB_AF_HA_PROFILE_ID,                                   \
                             ZB_SMZ_DEVICE_ID,                                      \
                             ZB_DEVICE_VER_SMZ,                                     \
                             0,                                                     \
                             in_clust_num,                                          \
                             out_clust_num,                                         \
                             {                                                      \
                                 ZB_ZCL_CLUSTER_ID_BASIC,                           \
                                 ZB_ZCL_CLUSTER_ID_IDENTIFY,                        \
                                 ZB_ZCL_CLUSTER_ID_ELECTRICAL_MEASUREMENT,          \
                                 ZB_ZCL_CLUSTER_ID_METERING,                        \
                             }}
/**
 * @brief Declare endpoint for SMZ device
 * @param ep_name - endpoint variable name
 * @param ep_id - endpoint ID
 * @param cluster_list - endpoint cluster list
 */
#define ZB_DECLARE_SMZ_EP(ep_name, ep_id, cluster_list)                                                      \
    ZB_ZCL_DECLARE_SMZ_SIMPLE_DESC(ep_name, ep_id, ZB_SMZ_IN_CLUSTER_NUM, ZB_SMZ_OUT_CLUSTER_NUM);           \
    ZBOSS_DEVICE_DECLARE_REPORTING_CTX(reporting_info##ep_name, ZB_SMZ_REPORT_ATTR_COUNT);                   \
    ZB_AF_DECLARE_ENDPOINT_DESC(ep_name, ep_id, ZB_AF_HA_PROFILE_ID, 0, NULL,                                \
                                ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), cluster_list,        \
                                (zb_af_simple_desc_1_1_t *)&simple_desc_##ep_name, ZB_SMZ_REPORT_ATTR_COUNT, \
                                reporting_info##ep_name, 0, NULL) /* No CVC ctx */
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* Initialization and Configuration functions  ******************************/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#endif /* ZB_SMZ_H */
