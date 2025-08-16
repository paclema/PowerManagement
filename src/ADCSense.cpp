#include "ADCSense.h"

ADCSense::ADCSense(uint8_t reqpin, adc1_channel_t channel, float coeff, String initName) : 
	ADCpin(reqpin),
	ADCchannel(channel),
	voltageDividerCoeff(coeff),
	name(initName)
{
	pinMode(ADCpin, INPUT);


}

ADCSense::~ADCSense(void) {
}

void ADCSense::setup(adc_atten_t atten){
    adc_bits_width_t adc_width;
    #if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3)
        adc_width = ADC_WIDTH_BIT_13;
		#else
        adc_width = ADC_WIDTH_BIT_12;
	#endif
	adc1_config_width(adc_width);
		
	esp_err_t ret;
	ret = esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP);
	if (ret == ESP_ERR_NOT_SUPPORTED) {
		Serial.printf( "Calibration scheme not supported, skip software calibration\n");
	} else if (ret == ESP_ERR_INVALID_VERSION) {
		Serial.printf( "eFuse not burnt, skip software calibration\n");
	} else if (ret == ESP_OK) {
		// Serial.printf( "eFuse burnt, software calibration can proceed\n");

		/*
		esp_adc_cal_characteristics_t adc_chars;
		esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_6, ADC_WIDTH_BIT_DEFAULT, ESP_ADC_CAL_VAL_DEFAULT_VREF, &adc_chars);
		//Check type of calibration value used to characterize ADC
		if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
			Serial.printf("eFuse Vref\n");
		} else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
			Serial.printf("Two Point\n");
		} else {
			Serial.printf("Default\n");
		}
		*/
		Serial.printf("---- ADC: atten: %d channel: %d coeff: %1.3f\n", atten, ADCchannel, voltageDividerCoeff);

        adc1_config_channel_atten(ADCchannel, atten);
        esp_adc_cal_value_t val_type = esp_adc_cal_characterize(
            ADC_UNIT_1,
            atten,
            adc_width,
            0,
            adc_chars
        );
    }

	for (int i = 0; i < ADC_SAMPLES; i++) readings[i] = 0;
	for (int i = 0; i < ADC_SAMPLES; i++) updatedADC();
}

void ADCSense::updatedADC(void){

	raw = adc1_get_raw(ADCchannel);
	// uint32_t raw_cal_mV = esp_adc_cal_raw_to_voltage(raw, adc_chars);
	// mV = raw_cal_mV * voltageDividerCoeff;
	// Serial.printf("VBAT sense reads %dmV --> %1.3fV VoltageDivider: %1.3fV - raw: %d\n", raw_cal_mV, (float)raw_cal_mV/1000, vBat/1000, raw);


	readTotal = readTotal - readings[readIndex];
	readings[readIndex] = raw;
	readTotal = readTotal + readings[readIndex];
	readIndex = readIndex + 1;
	if (readIndex >= ADC_SAMPLES) readIndex = 0;

	// mV = readTotal / ADC_SAMPLES;
	raw_cal_mV = esp_adc_cal_raw_to_voltage(readTotal / ADC_SAMPLES, adc_chars);
	mV = raw_cal_mV * (float)voltageDividerCoeff;
	// float t = (float)voltageDividerCoeff;
	// if (readIndex == 0) Serial.printf("VBAT sense reads %dmV --> %1.3fV VoltageDivider: %1.3fV --> %1.2fV- raw: %d voltageDividerCoeff: %1.4f\n", raw_mV, (float)raw_mV/1000, vBat/1000,vBat/1000, raw, t);


}
