#ifndef WIFISCANNER_H
#define WIFISCANNER_H
#include "WiFi.h"


void wifiScan();
void wifiInit(unsigned long &timeout, int &trial);
void wifiConnection();

#endif