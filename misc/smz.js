const fz = require('zigbee-herdsman-converters/converters/fromZigbee');
const tz = require('zigbee-herdsman-converters/converters/toZigbee');
const exposes = require('zigbee-herdsman-converters/lib/exposes');
const reporting = require('zigbee-herdsman-converters/lib/reporting');
const extend = require('zigbee-herdsman-converters/lib/extend');
const e = exposes.presets;
const ea = exposes.access;

const definition = {
    zigbeeModel: ['SMZ_v0.1'], // The model ID from: Device with modelID 'lumi.sens' is not supported.
    model: 'SMZ_v0.1', // Vendor model number, look on the device for a model number
    vendor: 'Knoedelauflauf', // Vendor of the device (only used for documentation and startup logging)
    description: 'Zigbee Smart Meter Interface', // Description of the device, copy from vendor site. (only used for documentation and startup logging)
    fromZigbee: [fz.electrical_measurement, fz.metering], // We will add this later
    toZigbee: [], // Should be empty, unless device can be controlled (e.g. lights, switches).
    exposes: [e.energy(), e.produced_energy(), e.power(), e.ac_frequency(), e.current(), e.current_phase_b(), e.current_phase_c(), e.voltage(), e.voltage_phase_b(), e.voltage_phase_c(),], // Defines what this device exposes, used for e.g. Home Assistant discovery and in the frontend
    configure: async (device, coordinatorEndpoint, logger) => {
        const endpoint = device.getEndpoint(10);
        try {
            await reporting.bind(endpoint, coordinatorEndpoint, ['haElectricalMeasurement']);
            await reporting.readEletricalMeasurementMultiplierDivisors(endpoint, true);
        } catch (e) {
            logger.warn(`SMZ_v0.1 failed to setup electricity measurements (${e.message})`);
            logger.debug(e.stack);
        }
        try {
            await reporting.bind(endpoint, coordinatorEndpoint, ['seMetering']);
            await reporting.readMeteringMultiplierDivisor(endpoint);
            await reporting.currentSummDelivered(endpoint, {change: 0});
        } catch (e) {
            logger.warn(`SMZ_v0.1 failed to setup metering (${e.message})`);
            logger.debug(e.stack);
        }
    },

};

module.exports = definition;
