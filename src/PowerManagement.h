#ifndef PowerManagement_H
#define PowerManagement_H
#ifdef ESP32
#ifndef ARDUINO_IOTPOSTBOX_V1
	#pragma message ("PowerManagement class is configured only for ARDUINO_IOTPOSTBOX_V1 by now. You must check PowerManagement.h class to change the preprocessor definitions accordingly")
#endif
#else
	#pragma message ("PowerManagement class only supports ESP32 hardware")
#endif

#pragma once
#include <Arduino.h>
#include <FunctionalInterrupt.h>

#include "ADCSense.h"
#define VBUS_ADC_CHANNEL	ADC1_CHANNEL_0
#define VBAT_ADC_CHANNEL	ADC1_CHANNEL_1

//ADC Attenuation
#define VBUS_ADC_ATTEN	ADC_ATTEN_DB_11
#define VBAT_ADC_ATTEN	ADC_ATTEN_DB_6

// Voltage divider coefficients:
#define VBUS_R9		3.3		// R9 = 3.3KOhm
#define VBUS_R10	2		// R10 = 2KOhm
#define VBUS_VOLTAGE_DIVIDER_COEFICIENT		(VBUS_R9 + VBUS_R10) / VBUS_R10

#define VBAT_R6		442		// R6 = 442KOhm
#define VBAT_R7		160		// R7 = 160KOhm
#define VBAT_VOLTAGE_DIVIDER_COEFICIENT 	(VBAT_R6 + VBAT_R7) / VBAT_R7

// #define ADC_SAMPLES 30

#ifndef VBUS_MIN
#define VBUS_MIN 4000
#endif

#ifndef VBAT_MIN
#define VBAT_MIN 2700
#endif

#define ISR_FLAG  ARDUINO_ISR_ATTR



enum class PowerStatus {
    Unknown = 0,
    BatteryPowered = 1,
    USBPowered = 2,
    BatteryAndUSBPowered = 3,
};

enum class ChargingStatus {
    Unknown = 0,
    Charging = 1,
    NotCharging = 2,
    Charged = 3,
};



class PowerManagement {
public:

	ADCSense vBatSense;
	ADCSense vBusSense;
	volatile int vBatStat = 0;	// MCP73831/2 Charging = 0, Not charging = 1


	PowerStatus powerStatus = PowerStatus::Unknown;
	PowerStatus lastPowerStatus = PowerStatus::Unknown;
	ChargingStatus chargingStatus = ChargingStatus::Unknown;
	ChargingStatus lastChargingStatus = ChargingStatus::Unknown;
	

	
	PowerManagement(void);
	~PowerManagement(void);

	void setup(void);
	void update(void);

	void ISR_FLAG isrCharging(){
		vBatStat = digitalRead(VBAT_STAT_SENSE_PIN);

		// if (pinState == HIGH) chargingStatus = ChargingStatus::NotCharging;
		// else if (pinState == LOW) chargingStatus = ChargingStatus::Charging;
		// else chargingStatus = ChargingStatus::Unknown;

		Serial.printf(" -- VBAT_STAT_SENSE_PIN changed to: %s --> ChargingStatus: %d\n",  vBatStat ? "true": "false", chargingStatus);
	};

	PowerStatus getPowerStatus() { return powerStatus; }
	PowerStatus getLastPowerStatus() { return lastPowerStatus; }
	ChargingStatus getChargingStatus() { return chargingStatus; }
	ChargingStatus getLastChargingStatus() { return lastChargingStatus; }
	int getVBatStat() { return vBatStat; }


};

#endif