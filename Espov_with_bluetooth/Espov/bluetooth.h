

#pragma once

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
    Serial.println("Bluetooth not initialized.");
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
