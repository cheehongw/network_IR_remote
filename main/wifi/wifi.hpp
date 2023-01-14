#include <ESP8266WiFi.h>
#include <time.h>
#include <TZ.h>
#include <LittleFS.h>
#include <CertStoreBearSSL.h>


char ssid[] = "Chee";
char password[] = "abcd1234";

namespace wifi {
    
    BearSSL::CertStore certStore;
    BearSSL::WiFiClientSecure wificlient;

    // Set time via NTP, as required for x.509 validation
    void setClock() {
        // You can use your own timezone, but the exact time is not used at all.
        // Only the date is needed for validating the certificates.
        configTime(TZ_Asia_Singapore, "pool.ntp.org", "time.nist.gov");

        Serial.print("Waiting for NTP time sync: ");
        time_t now = time(nullptr);
        while (now < 8 * 3600 * 2) {
            delay(500);
            Serial.print(".");
            now = time(nullptr);
        }
        Serial.println();

        struct tm timeinfo;
        gmtime_r(&now, &timeinfo);
        Serial.printf("%s %s", tzname[0], asctime(&timeinfo));
    }

    void init_wifi_connection() {
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("");
        Serial.print("Connected to ");
        Serial.println(ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }

    void init() {
        init_wifi_connection();
        setClock();

        LittleFS.begin();
        int numCerts = certStore.initCertStore(LittleFS, PSTR("/certs.idx"), PSTR("/certs.ar"));
        Serial.printf("Number of CA certs read: %d\n", numCerts);
        if (numCerts == 0) {
            Serial.printf("No certs found. Did you run certs-from-mozilla.py and upload the LittleFS directory before running?\n");
            return; // Can't connect to anything w/o certs!
        }

        wificlient.setCertStore(&certStore);
    }
}