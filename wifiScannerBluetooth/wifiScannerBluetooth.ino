#include "BluetoothSerial.h"
#include "wifiScanner.h"

int n = 0;
String ssid = "";
String pass = "";
unsigned long timestart = 0;
unsigned long timeout=0;
int trial = 0;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif

BluetoothSerial SerialBT;

void setup() 
{
  Serial.begin(115200);
  SerialBT.begin("Zoza"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  wifiScan();
  wifiInit(timeout, trial);
}

void loop() 
{

  if(trial > 3)
  {
    wifiScan();
    wifiInit(timeout, trial);
  }
  else
  {
    if((millis() - timestart > 2000) && (WiFi.status()  != WL_CONNECTED))
    {
      Serial.println("Reconnecting to Wifi...");
      WiFi.disconnect();
      wifiInit(timeout, trial);
      trial++;
      WiFi.reconnect();
      timestart=millis();
    }
  }
}

void wifiScan()
{
  
  n = WiFi.scanNetworks();

  
  SerialBT.print("Scan started");
  
  delay(500);

  
  if(n == 0)
  {
      SerialBT.print("Not networks found");
  }
  else
  {
      for(int i = 0; i < n; i++)
      {
        SerialBT.print(i + 1);
        SerialBT.print(": ");
        SerialBT.print(WiFi.SSID(i));
        SerialBT.print(" (");
        SerialBT.print(WiFi.RSSI(i));
        SerialBT.print(")");
      }
      int no_ssid;
      SerialBT.print("Enter the no. of the network you want to connect");
      while (SerialBT.available() == 0){}
        no_ssid = SerialBT.read();
      wifiConnection(no_ssid);
  }


}

void wifiInit(unsigned long &timeout, int &trial)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    SerialBT.print("Connecting to Wifi...");
    timeout = millis();
    while((WiFi.status() != WL_CONNECTED) && (millis() - timeout < 8000))
    {
        SerialBT.print(".");
        delay(1000);
    }
    

    if(WiFi.status() != WL_CONNECTED && WiFi.status() != WL_NO_SSID_AVAIL)
    {
        SerialBT.print("Password is not correct");
    }
    else if(WiFi.status() != WL_CONNECTED && WiFi.status() == WL_NO_SSID_AVAIL)
    {
        SerialBT.print("Wifi network is not avaliable");
    }
    else
    {
        SerialBT.print("");
        SerialBT.print("Connected successfully");
        SerialBT.print("IP Address : ");
        SerialBT.print(WiFi.localIP());
        trial = 0;
    }
}

void wifiConnection(int no_ssid)
{
    Serial.println("k");
    ssid = WiFi.SSID(no_ssid-1);
  
    //if((WiFi.encryptionType(no_ssid-1)) != WIFI_AUTH_OPEN)
    //{
        Serial.println("kk");
        SerialBT.print("Please enter the password of the network you chose");
        Serial.println("kkk");
        while (!SerialBT.available()){}

        pass = SerialBT.readString();
        //pass = SerialBT.readString();
        SerialBT.println(pass);
        //for(int i = 0; i < pass.length(); i++) SerialBT.print("*");
    //}
    //else
    //{
    //    Serial.println("kkkk");
    //    pass = "";
    //}
}
