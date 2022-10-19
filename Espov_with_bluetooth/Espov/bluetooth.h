/// ********************************************************************************************************************
/// multiple include protection
/// ********************************************************************************************************************

#pragma once

/// ********************************************************************************************************************
/// includes

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

namespace bluetooth {

namespace {
BluetoothSerial BT;
}


void begin() {
  bool success = BT.begin("ESP32");
  if (!success) {
    Serial.println("An error occurred initializing Bluetooth.");
    ESP.restart();
  } else {
    Serial.println("Bluetooth initialized.");
  }
}

int available(void) {
  return BT.available();
}

int read(void) {
  return BT.read();
}


}
