#ifndef WIFISCANNER_H
#define WIFISCANNER_H
#include "WiFi.h"
#include "BluetoothSerial.h"

extern int n;
extern String ssid;
extern String pass;

void wifiScan(BluetoothSerial &SerialBT);
void wifiInit(BluetoothSerial &SerialBT, unsigned long &timeout, int &trial);
void wifiConnection(BluetoothSerial &SerialBT);

#endif