#include <Wire.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>
#include <MQTT.h>
#include <PubSubClient.h>

int scanTime = 5; //In seconds
BLEScan *pBLEScan;
WiFiClient net;
char *SSID;
char *PWD;
unsigned long WMillis = 0;
int status = WL_IDLE_STATUS;
bool WifiDown = 0;

WiFiClient mqtt;
PubSubClient client(mqtt);
IPAddress BROKER;
bool MqttDown = 0;
int PORT = 1883;
unsigned long MMillis = 0;
char *willtopic = "errorwifi";
char *willmessage = "error";
int willqos = 2;
int willretain = 0;
char *id;
char *factory = "default";

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        if (advertisedDevice.getName() == "MJ_HT_V1")
        {
            Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
            int x = advertisedDevice.getPayload()[21];           //temperature lsb
            int y = advertisedDevice.getPayload()[22] * 16 * 16; //temperature hsb
            float temp = (x + y) / 10.0;
            Serial.printf("température: %f \n", temp);
        }
    }
};

void wificonnect()
{
    SSID = "pepper_1_24G";
    PWD = "astro4student";
    WiFi.setAutoConnect(true);

    int i = 0;
    Serial.print("Attempting to connect to ");
    Serial.print(SSID);
    while (status != WL_CONNECTED && i <= 10)
    {
        status = WiFi.begin(SSID, PWD);
        i++;
        delay(2000);
        Serial.print(".");
        if (i == 11)
        {
            Serial.println("");
            Serial.print("connexion fail with status:");
            Serial.println(WiFi.status());
        }
    }
    if (status == WL_CONNECTED)
    {
        Serial.println("");
        Serial.print("Connected to:");
        Serial.println(WiFi.SSID());
        Serial.print("ip :");
        Serial.println(WiFi.localIP());
    }
}

//! Long time delay, it is recommended to use shallow sleep, which can effectively reduce the current consumption
void espDelay(int ms)
{
    esp_sleep_enable_timer_wakeup(ms * 1000);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
    esp_light_sleep_start();
}

void ble_scan()
{
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    Serial.println("Scan done!");
    pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
    delay(1000);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Start");

    //initialisation du bluetooth
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99); // less or equal setInterval value
    wificonnect();
}

void loop()
{
    ble_scan();
}
