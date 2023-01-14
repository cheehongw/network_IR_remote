#pragma once
#include <IRremote.hpp>

#include "wifi/wifi.hpp"
#include "ir_sender/ir_sender.hpp"
#include "mqtt/mqtt.hpp"

#define IR_RECV D4
#define BUTTON D3

volatile long last_pressed_time = 0;

IRAM_ATTR void onPress() {
    long pressed = millis();

    if (pressed - last_pressed_time < 400) {
        return;
    }

    last_pressed_time = pressed;
    ir_sender::send(0x45);
}


void setup() {
    Serial.begin(9600);

    wifi::init();
    
    pinMode(BUTTON, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON), onPress, RISING);

    IrReceiver.begin(IR_RECV, ENABLE_LED_FEEDBACK);
    Serial.print("Ready to receive IR signals of protocols: ");
    printActiveIRProtocols(&Serial);
    Serial.println("at pin " + String(IR_RECV));

    ir_sender::init();
    if (!mqtt::init()) {
        Serial.println("Initialization to broker failed!");
    }; 
}


void loop() {
    if (IrReceiver.decode()) {
        // Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
        // IrReceiver.printIRResultShort(&Serial);
        IrReceiver.printIRSendUsage(&Serial);
        delay(500);
        IrReceiver.resume();
    }

    mqtt::mqttClient.poll();
    delay(50);                  
}