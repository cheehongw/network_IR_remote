#include <IRremote.hpp>
#define IR_SEND_PIN D5
#define numberOfRepeats 3

namespace ir_sender {

    void init() {
        IrSender.begin(IR_SEND_PIN, false, 0);
        Serial.println("Ready to send");
    }

    void send(long command) {
        IrSender.sendNEC(0x0, command, numberOfRepeats);
        Serial.print("Sending: ");
        Serial.println(command);
    }

}