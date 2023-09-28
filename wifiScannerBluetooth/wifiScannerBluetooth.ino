#include "BluetoothSerial.h"
#include "wifiScanner.h"
#include <Preferences.h>



unsigned long timestart = 0;
unsigned long timeout = 0;
int trial = 0;
uint8_t isConnected = 0;
uint8_t isEEPROM = 0;


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif

BluetoothSerial SerialBT;
Preferences preferences;

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    Serial.println("Client Connected");
    isConnected = 1;
  }

  if (event == ESP_SPP_CLOSE_EVT) {
    Serial.println("Client disconnected");
  }
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Yalla");  //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  preferences.begin("wifi_config", false);

  String temp_pref_ssid = preferences.getString("pref_ssid", "");
  if (!temp_pref_ssid.length()) {
    while (!isConnected) { SerialBT.register_callback(callback); }
    wifiScan(SerialBT);
    wifiInit(SerialBT, preferences, timeout, trial, isEEPROM);
  }
  else {
    isEEPROM = 1;
    wifiInit(SerialBT, preferences, timeout, trial, isEEPROM);
  }
}

void loop() {

  if (trial > 3) {
    wifiScan(SerialBT);
    wifiInit(SerialBT, preferences, timeout, trial, isEEPROM);
  } else {
    if ((millis() - timestart > 2000) && (WiFi.status() != WL_CONNECTED)) {
      SerialBT.println("Reconnecting to Wifi...");
      WiFi.disconnect();
      isEEPROM = 1;
      wifiInit(SerialBT, preferences, timeout, trial, isEEPROM);
      trial++;
      WiFi.reconnect();
      timestart = millis();
    }
  }
}