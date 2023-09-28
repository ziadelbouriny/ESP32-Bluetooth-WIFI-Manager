#ifndef WIFISCANNER_H
#define WIFISCANNER_H
#include "WiFi.h"
#include "BluetoothSerial.h"
#include <Preferences.h>


void wifiScan(BluetoothSerial &SerialBT);
void wifiInit(BluetoothSerial &SerialBT, Preferences &preferences, unsigned long &timeout, int &trial, uint8_t isEEPROM);
void wifiConnection(BluetoothSerial &SerialBT);

#endif