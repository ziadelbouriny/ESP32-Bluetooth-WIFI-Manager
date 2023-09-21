#include "BluetoothSerial.h"
#include "wifiScanner.h"

#define CONFIG_SW_COEXIST_ENABLE 1

int n = 0;
String ssid = "";
String pass = "";
unsigned long timestart = 0;
unsigned long timeout=0;
int trial = 0;
static int no_ssid = 0;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif

BluetoothSerial SerialBT;

void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  if(event == ESP_SPP_SRV_OPEN_EVT){
    Serial.println("Client Connected");
  }
 
  if(event == ESP_SPP_CLOSE_EVT ){
    Serial.println("Client disconnected");
  }
}

void setup() 
{
  Serial.begin(115200);
  SerialBT.register_callback(callback);
  SerialBT.begin("Yalla"); //Bluetooth device name
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
      SerialBT.print("Enter the no. of the network you want to connect");
      while (SerialBT.available() == 0){}
      no_ssid = SerialBT.read();
      wifiConnection();
  }


}

void wifiInit(unsigned long &timeout, int &trial)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WiFi.SSID(no_ssid - 49), pass);

    SerialBT.print("Connecting to Wifi...");
    timeout = millis();
    while((WiFi.status() != WL_CONNECTED) && (millis() - timeout < 8000))
    {
        SerialBT.print(".");
        delay(1000);
    }
    
    SerialBT.println("");

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

void wifiConnection()
{
    ssid = WiFi.SSID(no_ssid-49);
    pass.trim();
  
    if((WiFi.encryptionType(no_ssid-49)) != WIFI_AUTH_OPEN)
    {
        SerialBT.println("Please enter the password of the network you chose");
        while(!SerialBT.available()){}
        SerialBT.setTimeout(5000);
        pass = SerialBT.readString();
        pass.trim();
        for(int i = 0; i < pass.length(); i++) SerialBT.print("*");
        SerialBT.println("");
    }
    else
    {
        pass = "";
    }
}
