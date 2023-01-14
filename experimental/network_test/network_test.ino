#include <ESP8266WiFi.h>
#include <Ticker.h>

extern "C"
{
#include "wpa2_enterprise.h"
}

// SSID to connect to
char ssid[] = "NUS_STU";
char username[] = "nusstu\\e0543594";
char identity[] = "nusstu\\e0543594";
char password[] = "ngY6&Py$rS4SXv/";

bool connect()
{
    wifi_set_opmode(STATION_MODE);
    struct station_config wifi_config;
    memset(&wifi_config, 0, sizeof(wifi_config));
    strcpy((char *)wifi_config.ssid, ssid);
    wifi_station_set_config(&wifi_config);
    wifi_station_dhcpc_start();
    wifi_station_clear_cert_key();
    wifi_station_set_wpa2_enterprise_auth(1);
    wifi_station_set_enterprise_identity((uint8 *)username, strlen(username));
    wifi_station_set_enterprise_username((uint8 *)username, strlen(username));
    wifi_station_set_enterprise_password((uint8 *)password, strlen(password));
    // wifi_station_set_enterprise_ca_cert((byte *)ca_cert, strlen(ca_cert));
    wifi_station_connect();
    Ticker timer;
    bool connected = true;
    timer.attach(5, [&]()
                 { connected = false; });
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(10);
    }
    timer.detach();
    return connected;
}

void connectBlock()
{
    while (!connect())
    {
        delay(100);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.printf("SDK version: %s\n", system_get_sdk_version());
    Serial.printf("free heap: %d\n", ESP.getFreeHeap());

    connectBlock();

    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

unsigned int msgCnt = 0;
unsigned int last = millis();
void loop()
{
    WiFiClient client;
    IPAddress remote(192, 168, 3, 1);
    if (last + 2000 < millis())
    {
        last = millis();
        Serial.printf("WiFi status: %d \n", WiFi.status());
        Serial.println("connecting");
        if (client.connect(remote, 2000))
        {
            ++msgCnt;
            Serial.println("connected");
            client.printf("this is message nr: %d\n", msgCnt);
            Serial.println("written");
            client.stop();
            Serial.println("stopped");
        }
    }
}