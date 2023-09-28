#include "BluetoothSerial.h"
#include "wifiScanner.h"
#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#define FORMAT_LITTLEFS_IF_FAILED true
DynamicJsonDocument Config(2048);
JsonObject obj = Config.as<JsonObject>();


unsigned long timestart = 0;
unsigned long timeout = 0;
int trial = 0;
uint8_t isConnected = 0;
String credentials_array[2];

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif

BluetoothSerial SerialBT;

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
  if (credentials_array[0] == "") {
    while (!isConnected) { SerialBT.register_callback(callback); }
    wifiScan(SerialBT);
    wifiInit(SerialBT, timeout, trial);
  }
  else {
    wifiInit(SerialBT, timeout, trial);
  }
}

void loop() {

  if (trial > 3) {
    wifiScan(SerialBT);
    wifiInit(SerialBT, timeout, trial);
  } else {
    if ((millis() - timestart > 2000) && (WiFi.status() != WL_CONNECTED)) {
      SerialBT.println("Reconnecting to Wifi...");
      WiFi.disconnect();
      wifiInit(SerialBT, timeout, trial);
      trial++;
      WiFi.reconnect();
      timestart = millis();
    }
  }
}