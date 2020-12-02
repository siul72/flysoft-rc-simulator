#include <SoftwareSerial.h>
#include "rcmanager.h"
#include "utils.h"
#include<stdio.h>


#define RC_RX_PIN 10
#define RC_TX_PIN 11

const int ledPin =  LED_BUILTIN;// the number of the LED pin
uint8_t ledState = LOW;
unsigned long previousMillis = 0;
unsigned long interval = 0;

char msg[256];
uint8_t x = 1;


SoftwareSerial RcSerial(RC_RX_PIN, RC_TX_PIN);  // Set up the software serial port on pins 2 and 3 for the RC.


void setup() {
  Serial.begin(9600);
  Serial.println("Serial Started!");

    for (uint8_t i = 0; i < SBUS_CHANNEL_NUMBER; i++) {
        RcManager::rcChannels[i] = 1500;
    }



    // define pin modes for tx, rx:
    pinMode(RC_RX_PIN, INPUT);
    pinMode(RC_TX_PIN, OUTPUT);
    RcSerial.begin(100000);
}

void loop() {
    unsigned long currentMillis = millis();

    if (Serial.available()) {
      while(Serial.available()) {
        String a = Serial.readString();// read the incoming data as string
        Serial.println(a);
        int my_int;
        sscanf(a.c_str(), "%d", &my_int);
        RcManager::rcChannels[x] = my_int;

        //String a = Serial.readString();
        //sprintf(msg, "<ECHO>%s", a);
        //Serial.println(msg);
      }
    }

    /*
     * Here you can modify values of rcChannels while keeping it in 1000:2000 range
     */

    if (currentMillis > RcManager::sbusTime) {

        sprintf(msg, "Signal %d=%d", x, RcManager::rcChannels[x]);
        Serial.println(msg);
        RcManager::sbusPreparePacket(false, false);
        RcSerial.write(RcManager::sbusPacket, SBUS_PACKET_LENGTH);
        utils::print_hex(RcManager::sbusPacket, SBUS_PACKET_LENGTH, msg);
        //sprintf(msg, "0x%02X,0x%02X,0x%02X,0x%02X,", RcManager::sbusPacket[0], RcManager::sbusPacket[1],RcManager::sbusPacket[2],RcManager::sbusPacket[3]);
        Serial.println(msg);
        RcManager::sbusTime = currentMillis + SBUS_UPDATE_RATE;
    }

    if (static_cast<unsigned long>(currentMillis - previousMillis) >= interval) {
      previousMillis = currentMillis;
      if (ledState == LOW) {
        ledState = HIGH;
        interval = 100;
      } else {
        ledState = LOW;
        interval = 3000;

      }
      digitalWrite(ledPin, ledState);
    }
}
