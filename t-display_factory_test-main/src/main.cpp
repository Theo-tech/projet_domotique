#include <string>
#include <Wire.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>
#include <MQTT.h>
#include <iostream>
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
char *willtopic = "TR_temperature";
char *willmessage = "error";
int willqos = 2;
int willretain = 0;
char *id;
char *factory = "default";

void temp(char *_message)
{
    client.publish("TR_temperature", _message);
}
void humidity(char *_message)
{
    client.publish("TR_humidite", _message);
}

void connectMqtt()
{
    id = "testtest";
    client.setServer("192.168.1.94", 1883);
    while (!client.connected())
    {
        Serial.println("Connecting to MQTT...");
        if (client.connect(id, NULL, NULL, willtopic, willqos, willretain, willmessage, false))
        {
            Serial.println("connected");
            client.subscribe(id);
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        if (advertisedDevice.getName() == "MJ_HT_V1")
        {

            auto flag = advertisedDevice.getPayload()[18];
            if (flag == 0x0D)
            {
                Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
                int x = advertisedDevice.getPayload()[21];           //temperature lsb
                int y = advertisedDevice.getPayload()[22] * 16 * 16; //temperature hsb
                float tp = (x + y) / 10.0f;
                Serial.printf("température: %f \n", tp);
                char buffer[5];
                char *val = dtostrf(tp, sizeof(buffer), 2, buffer);
                temp(val);
                delay(40);
                int z = advertisedDevice.getPayload()[23];
                int u = advertisedDevice.getPayload()[24] * 16 * 16;
                float hd = (z + u) / 10.0f;
                Serial.printf("humidité:%f \n", hd);
                char bufer[5];
                char *bal = dtostrf(hd, sizeof(bufer), 2, bufer);
                printf(bal);
                humidity(bal);
            }
        }
    }
};

void wificonnect()
{
    SSID = "ASUS";
    PWD = "astro4student";
    WiFi.setAutoConnect(true);

    int i = 0;
    Serial.print("Attempting to connect to ");
    Serial.print(SSID);
    WiFi.mode(WIFI_STA);
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
    connectMqtt();
}

void loop()
{
    ble_scan();
}
