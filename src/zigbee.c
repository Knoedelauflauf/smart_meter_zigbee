/* Includes ------------------------------------------------------------------*/
#include "zigbee.h"

#include <string.h>
#include <zb_nrf_platform.h>
#include <zboss_api_addons.h>
#include <zephyr/dfu/mcuboot.h>
#include <zephyr/sys/reboot.h>
#include <zigbee/zigbee_app_utils.h>
#include <zigbee/zigbee_error_handler.h>
#include <zigbee/zigbee_fota.h>

#include "sml_processing_obis_handler.h"
#include "zb_smz.h"
/* LOG channel def -----------------------------------------------------------*/
LOG_MODULE_REGISTER(zigbee);
/* Private typedef -----------------------------------------------------------*/
struct zb_device_ctx {
    zb_zcl_basic_attrs_ext_t basic_attr;
    zb_zcl_identify_attrs_t identify_attr;
    zb_zcl_el_measurement_attrs_ext_t el_measurement;
    zb_zcl_metering_attrs_custom_t metering;
};
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static struct zb_device_ctx dev_ctx;
ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST(identify_attr_list, &dev_ctx.identify_attr.identify_time);

ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT(basic_attr_list, &dev_ctx.basic_attr.zcl_version, &dev_ctx.basic_attr.app_version,
                                     &dev_ctx.basic_attr.stack_version, &dev_ctx.basic_attr.hw_version,
                                     dev_ctx.basic_attr.mf_name, dev_ctx.basic_attr.model_id,
                                     dev_ctx.basic_attr.date_code, &dev_ctx.basic_attr.power_source,
                                     dev_ctx.basic_attr.location_id, &dev_ctx.basic_attr.ph_env,
                                     dev_ctx.basic_attr.sw_ver);

ZB_ZCL_DECLARE_ELECTRICAL_MEASUREMENT_ATTRIB_LIST_EXT(
    el_measurement_attr_list, &dev_ctx.el_measurement.measurement_type, &dev_ctx.el_measurement.rmsCurrentPhA,
    &dev_ctx.el_measurement.rmsCurrentPhB, &dev_ctx.el_measurement.rmsCurrentPhC, &dev_ctx.el_measurement.rmsVoltagePhA,
    &dev_ctx.el_measurement.rmsVoltagePhB, &dev_ctx.el_measurement.rmsVoltagePhC, &dev_ctx.el_measurement.acFrequency,
    &dev_ctx.el_measurement.voltageMultiplier, &dev_ctx.el_measurement.voltageDivisor,
    &dev_ctx.el_measurement.currentMultiplier, &dev_ctx.el_measurement.currentDivisor,
    &dev_ctx.el_measurement.powerMultiplier, &dev_ctx.el_measurement.powerDivisor,
    &dev_ctx.el_measurement.frequencyMultiplier, &dev_ctx.el_measurement.frequencyDivisor,
    &dev_ctx.el_measurement.powerFactor);

ZB_ZCL_DECLARE_METERING_ATTRIB_LIST_CUSTOM(metering_attr_list, &dev_ctx.metering.curr_summ_delivered,
                                           &dev_ctx.metering.instantaneous_demand, &dev_ctx.metering.multiplier,
                                           &dev_ctx.metering.divisor);

ZB_DECLARE_SMZ_CLUSTER_LIST(smz_cluster, basic_attr_list, identify_attr_list, el_measurement_attr_list,
                            metering_attr_list);

ZB_DECLARE_SMZ_EP(smz_ep, SMZ_ENDPOINT_ID, smz_cluster);

/* Declare application's device context (list of registered endpoints)
 * for Dimmer Switch device.
 */
#if SMZ_ENDPOINT_ID == CONFIG_ZIGBEE_FOTA_ENDPOINT
#error "SMZ_ENDPOINT_ID and Zigbee OTA endpoints should be different."
#endif

extern zb_af_endpoint_desc_t zigbee_fota_client_ep;
ZBOSS_DECLARE_DEVICE_CTX_2_EP(smz_ctx, zigbee_fota_client_ep, smz_ep);
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void confirm_image(void) {
    if (!boot_is_img_confirmed()) {
        int ret = boot_write_img_confirmed();

        if (ret) {
            LOG_ERR("Couldn't confirm image: %d", ret);
        } else {
            LOG_INF("Marked image as OK");
        }
    }
}

static void ota_evt_handler(const struct zigbee_fota_evt *evt) {
    switch (evt->id) {
        case ZIGBEE_FOTA_EVT_FINISHED:
            LOG_INF("Reboot application.");
            sys_reboot(SYS_REBOOT_COLD);
            break;

        case ZIGBEE_FOTA_EVT_ERROR:
            LOG_ERR("OTA image transfer failed.");
            break;

        default:
            break;
    }
}
/**@brief Callback function for handling ZCL commands.
 *
 * @param[in]   bufid   Reference to Zigbee stack buffer
 *                      used to pass received data.
 */
static void zcl_device_cb(zb_bufid_t bufid) {
    zb_zcl_device_callback_param_t *device_cb_param = ZB_BUF_GET_PARAM(bufid, zb_zcl_device_callback_param_t);

    if (device_cb_param->device_cb_id == ZB_ZCL_OTA_UPGRADE_VALUE_CB_ID) {
        zigbee_fota_zcl_cb(bufid);
    } else {
        device_cb_param->status = RET_NOT_IMPLEMENTED;
    }
}

/**@brief Function for initializing all clusters attributes. */
static void app_clusters_attr_init(void) {
    /* Basic cluster attributes data */
    dev_ctx.basic_attr.zcl_version = ZB_ZCL_VERSION;
    dev_ctx.basic_attr.app_version = SMZ_INIT_BASIC_APP_VERSION;
    dev_ctx.basic_attr.stack_version = SMZ_INIT_BASIC_STACK_VERSION;
    dev_ctx.basic_attr.hw_version = SMZ_INIT_BASIC_HW_VERSION;
    ZB_ZCL_SET_STRING_VAL(dev_ctx.basic_attr.model_id, SMZ_INIT_BASIC_MODEL_ID,
                          ZB_ZCL_STRING_CONST_SIZE(SMZ_INIT_BASIC_MODEL_ID));

    ZB_ZCL_SET_STRING_VAL(dev_ctx.basic_attr.date_code, SMZ_INIT_BASIC_DATE_CODE,
                          ZB_ZCL_STRING_CONST_SIZE(SMZ_INIT_BASIC_DATE_CODE));

    ZB_ZCL_SET_STRING_VAL(dev_ctx.basic_attr.mf_name, SMZ_INIT_BASIC_MANUF_NAME,
                          ZB_ZCL_STRING_CONST_SIZE(SMZ_INIT_BASIC_MANUF_NAME));
    dev_ctx.basic_attr.power_source = SML_INIT_BASIC_POWER_SOURCE;

    dev_ctx.basic_attr.ph_env = SMZ_INIT_BASIC_PH_ENV;

    /* Identify cluster attributes data. */
    dev_ctx.identify_attr.identify_time = ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE;

    /* Electrical Measurement */
    dev_ctx.el_measurement.measurement_type = (zb_uint32_t)(ZB_ZCL_ELECTRICAL_MEASUREMENT_ACTIVE_MEASUREMENT |
                                                            ZB_ZCL_ELECTRICAL_MEASUREMENT_PHASE_A_MEASUREMENT |
                                                            ZB_ZCL_ELECTRICAL_MEASUREMENT_PHASE_B_MEASUREMENT |
                                                            ZB_ZCL_ELECTRICAL_MEASUREMENT_PHASE_C_MEASUREMENT);
    dev_ctx.el_measurement.rmsCurrentPhA = ZB_ZCL_ELECTRICAL_MEASUREMENT_RMSCURRENT_DEFAULT_VALUE;
    dev_ctx.el_measurement.rmsCurrentPhB = ZB_ZCL_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHB_DEFAULT_VALUE;
    dev_ctx.el_measurement.rmsCurrentPhC = ZB_ZCL_ELECTRICAL_MEASUREMENT_RMSCURRENT_PHC_DEFAULT_VALUE;
    dev_ctx.el_measurement.rmsVoltagePhA = ZB_ZCL_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_DEFAULT_VALUE;
    dev_ctx.el_measurement.rmsVoltagePhB = ZB_ZCL_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHB_DEFAULT_VALUE;
    dev_ctx.el_measurement.rmsVoltagePhC = ZB_ZCL_ELECTRICAL_MEASUREMENT_RMSVOLTAGE_PHC_DEFAULT_VALUE;
    dev_ctx.el_measurement.acFrequency = ZB_ZCL_ELECTRICAL_MEASUREMENT_AC_FREQUENCY_DEFAULT_VALUE;
    dev_ctx.el_measurement.voltageMultiplier = ZB_ZCL_ELECTRICAL_MEASUREMENT_ACVOLTAGE_MULTIPLIER_DEFAULT_VALUE;
    dev_ctx.el_measurement.voltageDivisor = (zb_uint16_t)100;
    dev_ctx.el_measurement.currentMultiplier = ZB_ZCL_ELECTRICAL_MEASUREMENT_ACCURRENT_MULTIPLIER_DEFAULT_VALUE;
    dev_ctx.el_measurement.currentDivisor = (zb_uint16_t)100;
    dev_ctx.el_measurement.powerMultiplier = ZB_ZCL_ELECTRICAL_MEASUREMENT_ACPOWER_MULTIPLIER_DEFAULT_VALUE;
    dev_ctx.el_measurement.powerDivisor = (zb_uint16_t)100;
    dev_ctx.el_measurement.frequencyMultiplier = ZB_ZCL_ELECTRICAL_MEASUREMENT_AC_FREQUENCY_MULTIPLIER_DEFAULT_VALUE;
    dev_ctx.el_measurement.frequencyDivisor = (zb_uint16_t)100;
    dev_ctx.el_measurement.powerFactor = ZB_ZCL_ELECTRICAL_MEASUREMENT_POWER_FACTOR_DEFAULT_VALUE;

    /* Metering */
    dev_ctx.metering.curr_summ_delivered = (zb_uint48_t)ZB_INIT_UINT48(0x0, 0x20);
    dev_ctx.metering.instantaneous_demand = (zb_int24_t)ZB_INIT_INT24(0x0, 0x0);
    dev_ctx.metering.multiplier = (zb_uint24_t)ZB_INIT_UINT24(0x0, 0x1);
    dev_ctx.metering.divisor = (zb_uint24_t)ZB_INIT_UINT24(0x0, 0x3e8);
}
/* External Called functions -------------------------------------------------*/
void zboss_signal_handler(zb_bufid_t bufid) {
    zb_zdo_app_signal_type_t sig = zb_get_app_signal(bufid, NULL);
    /* Pass signal to the OTA client implementation. */
    zigbee_fota_signal_handler(bufid);
    switch (sig) {
        case ZB_ZDO_SIGNAL_PRODUCTION_CONFIG_READY:
            /* Call default signal handler. */
            while (atomic_and(&sml_obis_initialized, REQUIRED_OBIS) != REQUIRED_OBIS) {
                k_msleep(250);
            }
            ZB_ERROR_CHECK(zigbee_default_signal_handler(bufid));
            break;

        default:
            /* Call default signal handler. */
            ZB_ERROR_CHECK(zigbee_default_signal_handler(bufid));
            break;
    }

    if (bufid) {
        zb_buf_free(bufid);
    }
}
/* Exported functions --------------------------------------------------------*/
void start_zigbee() {
    /* Initialize Zigbee FOTA download service. */
    zigbee_fota_init(ota_evt_handler);

    /* Mark the current firmware as valid. */
    confirm_image();

    /* Register callback for handling ZCL commands. */
    ZB_ZCL_REGISTER_DEVICE_CB(zcl_device_cb);

    /* Register device context (endpoints). */
    ZB_AF_REGISTER_DEVICE_CTX(&smz_ctx);

    app_clusters_attr_init();
    /* Start Zigbee default thread */
    zigbee_enable();
}
