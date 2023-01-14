#include <ArduinoMqttClient.h>
#include <string.h>

#define BROKER "972890f6d60643c99e457ffb53a0b775.s1.eu.hivemq.cloud"
#define PORT 8883
#define USER "chwong"
#define PASSWORD "cegPB4GPDT!X9yC"
#define TOPIC "IRCODE"
#define SUB_QOS 1

namespace mqtt
{
    MqttClient mqttClient(wifi::wificlient);

    void handle_msg(int message_size) {
        // we received a message, print out the topic and contents
        Serial.print("Received a message with topic '");
        Serial.print(mqttClient.messageTopic());
        Serial.print("', length ");
        Serial.print(message_size);
        Serial.println(" bytes:");

        uint8_t buf[message_size + 1];

        // use the Stream interface to print the contents
        mqttClient.read(buf, message_size);
        buf[message_size] = '\0';
        String s = String(reinterpret_cast<char*>(buf));
        Serial.println(s);
        
        long code = strtol(reinterpret_cast<char*>(buf), NULL, 16);
        Serial.println(code);
        ir_sender::send(code);
    }

    bool init() {

        mqttClient.setUsernamePassword(USER, PASSWORD);

        Serial.print("Attempting to connect to the MQTT broker: ");
        Serial.println(BROKER);
        if (!mqttClient.connect(BROKER, PORT)){
            Serial.print("MQTT connection failed! Error code: ");
            Serial.println(mqttClient.connectError());
            return false;
        }

        Serial.println("You're connected to the MQTT broker!\n");

        mqttClient.onMessage(handle_msg);

        Serial.print("Subscribing to topic: ");
        Serial.println(TOPIC);

        mqttClient.subscribe(TOPIC);
        return true;
    }


} // namespace mqtt
