#ifndef ADCSense_H
#define ADCSense_H

#pragma once
#include <Arduino.h>

#ifdef ESP32
#include "driver/adc.h"
#include "esp_adc_cal.h"

#ifndef ADC_SAMPLES
#define ADC_SAMPLES 30
#endif

#else
	#pragma message ("ADCSense class only supports ESP32 hardware")
#endif



class ADCSense {
public:

	const uint8_t ADCpin;
	const adc1_channel_t ADCchannel;
	const float voltageDividerCoeff;
	String name;

	float readings[ADC_SAMPLES];
	int readIndex = 0;
	int readTotal = 0;

	int raw = 0;				// Output values in mV of the voltage divider
	uint32_t raw_cal_mV = 0;	// Output values in calibrated mV of the voltage divider
	float mV = 0;				// Input Values in mV of the voltage divider

	esp_adc_cal_characteristics_t *adc_chars = new esp_adc_cal_characteristics_t;

	
	ADCSense(uint8_t reqpin, adc1_channel_t channel, float coeff, String initName);
	~ADCSense(void);

	void setup(adc_atten_t atten);
	void updatedADC(void);

	uint8_t getPin() { return ADCpin; }
	float getLastmV(void) { return esp_adc_cal_raw_to_voltage(readings[readIndex], adc_chars) *(float)voltageDividerCoeff; }

private:
	volatile unsigned long lastChange;
};

#endif