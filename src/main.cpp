#include <Arduino.h>
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 12 // Pin pro receiver ovladače
#define ENTER 4127850240 // Kód klávesy ENTER
#define RED_LED 5 // Pin pro barvu červenou
#define GREEN_LED 6 // Pin pro barvu zelenou
#define BLUE_LED 9 // Pin pro barvu modrou

const unsigned long codes[] = {
    3910598400, // 0
    4077715200, // 1
    3877175040, // 2
    2707357440, // 3
    4144561920, // 4
    3810328320, // 5
    2774204160, // 6
    3175284480, // 7
    2907897600, // 8
    3041591040, // 9
};

String stringNum = "";
int currentLED = 0; // 0 = red, 1 = green, 2 = blue

String receivedNum(unsigned long irData) {
    for (unsigned int i = 0; i < sizeof(codes) / sizeof(codes[0]); i++) {
        if (irData == codes[i]) {
            return String(i);
        }
    }
    return "";
}

void setLED(int value) {
    int ledPin;
    String ledColor;

    switch (currentLED) {
        case 0: ledPin = RED_LED; ledColor = "Červená"; break;
        case 1: ledPin = GREEN_LED; ledColor = "Zelená"; break;
        case 2: ledPin = BLUE_LED; ledColor = "Modrá"; break;
    }

    analogWrite(ledPin, value);
    Serial.print(ledColor + " LED: ");
    Serial.println(value);
}

void setup() {
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
    Serial.begin(9600);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
}

void loop() {
    if (IrReceiver.decode()) {
        unsigned long irData = IrReceiver.decodedIRData.decodedRawData;

        if (irData > 0) {
            Serial.print(irData);
            Serial.print(",");

            if (irData == ENTER) {
                int intNum = stringNum.toInt();
                if (intNum >= 0 && intNum <= 255) {
                    setLED(intNum);
                    Serial.print("Nastavená hodnota: ");
                    Serial.println(intNum);
                    stringNum = "";
                } else {
                    Serial.println("Mimo rozsah");
                    stringNum = "";
                }
            } else {
                switch (irData) {
                    case 3125149440: currentLED = 0; Serial.println("Vybrána červená LED"); break;
                    case 3108437760: currentLED = 1; Serial.println("Vybrána zelená LED"); break;
                    case 3091726080: currentLED = 2; Serial.println("Vybrána modrá LED"); break;
                    default: stringNum += receivedNum(irData); Serial.println(stringNum); break;
                }
            }
        }

        IrReceiver.resume();
    }
}
