#include "wifiScanner.h"

int n = 0;
String ssid = "";
String pass = "";
static int no_ssid = 0;

void wifiScan(BluetoothSerial &SerialBT) {

  n = WiFi.scanNetworks();
  SerialBT.println("Scan started");

  delay(500);


  if (n == 0) {
    SerialBT.println("Not networks found");
  } else {
    for (int i = 0; i < n; i++) {
      SerialBT.print(i + 1);
      SerialBT.print(": ");
      SerialBT.print(WiFi.SSID(i));
      SerialBT.print(" (");
      SerialBT.print(WiFi.RSSI(i));
      SerialBT.print(")");
      SerialBT.println("");
      SerialBT.flush();


      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println("");
    }
    SerialBT.println("Enter the no. of the network you want to connect");
    SerialBT.flush();
    while (SerialBT.available() == 0) {}
    no_ssid = SerialBT.parseInt();
    //SerialBT.flush();

    while (SerialBT.available()) {
      char t = SerialBT.read();
    }

    wifiConnection(SerialBT);
  }
}

void wifiInit(BluetoothSerial &SerialBT, unsigned long &timeout, int &trial) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WiFi.SSID(no_ssid - 1), pass);

  SerialBT.print("Connecting to Wifi...");
  timeout = millis();
  while ((WiFi.status() != WL_CONNECTED) && (millis() - timeout < 8000)) {
    SerialBT.print(".");
    delay(1000);
  }

  SerialBT.println("");

  if (WiFi.status() != WL_CONNECTED && WiFi.status() != WL_NO_SSID_AVAIL) {
    SerialBT.println("Password is not correct");
    wifiConnection(SerialBT);
  } else if (WiFi.status() != WL_CONNECTED && WiFi.status() == WL_NO_SSID_AVAIL) {
    SerialBT.println("Wifi network is not avaliable");
  } else {
    WiFi.disconnect();
    SerialBT.print("");
    SerialBT.print("Connected successfully");
    SerialBT.print("IP Address : ");
    SerialBT.print(WiFi.localIP());
    Serial.print("");
    Serial.print("Connected successfully");
    Serial.print("IP Address : ");
    Serial.print(WiFi.localIP());
    WiFi.begin(WiFi.SSID(no_ssid - 1), pass);
    Serial.print(WiFi.localIP());
    trial = 0;
  }
}

void wifiConnection(BluetoothSerial &SerialBT) {
  ssid = WiFi.SSID(no_ssid - 1);
  pass.trim();

  if ((WiFi.encryptionType(no_ssid - 1)) != WIFI_AUTH_OPEN) {
    SerialBT.println("Please enter the password of the network you chose");
    while (!SerialBT.available()) {}
    //SerialBT.setTimeout(5000);

    while (SerialBT.available()) {
      char incomingChar = SerialBT.read();
      Serial.print("Incomming:  ");
      Serial.println(incomingChar);
      if (incomingChar != '\n') {
        pass += String(incomingChar);
        Serial.println(pass);
      }
    }
    //pass = SerialBT.readString();
    pass.trim();
  } else {
    pass = "";
  }
}