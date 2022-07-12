#include <Arduino.h>

#include "PowerManagement.h"
PowerManagement power;

void setup() {
  Serial.begin(115200);

  power.setup();

  Serial.println("###  Looping time\n");
}

void loop() {
  power.update();

  Serial.printf(" -- VBAT_STAT_SENSE_PIN: %s --> ChargingStatus: %d ",  power.getVBatStat() ? "true": "false", power.getChargingStatus());
  Serial.printf("  **  vBus %1.3fmV vBat: %1.3fmV --> PowerStatus: %d\n", power.vBusSense.mV, power.vBatSense.mV, power.getPowerStatus());

  delay(20);
}
