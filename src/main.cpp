#include <SoftwareSerial.h>
#include <TaskScheduler.h>
#include<stdio.h>
#include "rcmanager.h"
#include "utils.h"


#define  LED_WATCHDOG LED_BUILTIN
#define RC_RX_PIN 10
#define RC_TX_PIN 11

Scheduler runner;
std::vector<String> myQueue;


const int ledPin =  LED_BUILTIN;// the number of the LED pin
uint8_t print_count = 0;
uint8_t ledState = LOW;
char msg[256];

void serialThread();
Task readSerialPort(40, TASK_FOREVER, &serialThread);

void frameThread();
Task sendSbusFrame(SBUS_UPDATE_RATE, TASK_FOREVER, &frameThread);
SoftwareSerial RcSerial(RC_RX_PIN, RC_TX_PIN);  // Set up the software serial port on pins 2 and 3 for the RC.

void parseReceivedFrame();
Task parseRxTask(40, TASK_FOREVER, &parseReceivedFrame);

void blinkLed();
Task ledBlinkTask(1000, TASK_FOREVER, blinkLed);

void blinkLed(){
  digitalWrite(LED_WATCHDOG, !digitalRead(LED_WATCHDOG));
}

void serialThread() {


  while(Serial.available()>0){
    // FRAME = KBUS;<Time Stamp>;<Sequence ID>;<Function Enum>;<Number of Elements>;<Element>;ENDK;
    String s=Serial.readString();

    char str[18] = "";
    Serial.println("new frame on queue");
    String frame;
    frame.concat(FRAME_HEADER);
    frame.concat(";");
    Utils::timeToString(str, sizeof(str));
    frame.concat(str);
    frame.concat(";");
    frame.concat(FRAME_FUNCTION_SEND_SINGLE_POT_VALUE);
    s.trim();
    frame.concat(s);
    frame.concat(FRAME_END);

    myQueue.push_back(frame);
    sprintf(msg, "push:%s", frame.c_str());
    Serial.println(msg);
    parseRxTask.enableIfNot();

  }

}

void function_set_single_value(String payload){
	 //payload
    std::vector<String> myCommand = Utils::stringSplit(payload, ',');
//    for(uint8_t i = 0; i < myCommand.size(); i++){
//		sprintf(msg, "keyvalue pair %d=%s", i, myCommand[i].c_str());
//		Serial.println(msg);
//	}
    int index = atoi(myCommand[0].c_str());
    int value = atoi(myCommand[1].c_str());
	RcManager::rcChannels[index] = value;
    sprintf(msg, "set channel %d=%d", index, value);
    Serial.println(msg);

}

void function_take_off(){
	RcManager::rcChannels[0] = 1000;
	RcManager::rcChannels[1] = 2000;
	RcManager::rcChannels[2] = 1000;
	RcManager::rcChannels[3] = 1000;
	sprintf(msg, "TAKE OFF");
    Serial.println(msg);

}

void function_rest(){
	RcManager::rcChannels[0] = 1500;
	RcManager::rcChannels[1] = 1500;
	RcManager::rcChannels[2] = 1500;
	RcManager::rcChannels[3] = 1500;
	sprintf(msg, "REST");
    Serial.println(msg);

}

void parseReceivedFrame(){
  Serial.println(">parseReceivedFrame");
  bool x = myQueue.size();
  sprintf(msg, "pop=%d", x);
  Serial.println(msg);
  while(!myQueue.empty() ){
    String frame = myQueue.back();
    myQueue.pop_back();
    sprintf(msg, "pop=%s", frame.c_str());
    Serial.println(msg);
    std::vector<String> myFunctionList = Utils::stringSplit(frame, ';');
//    for(uint8_t i = 0; i < myFunctionList.size(); i++){
//		sprintf(msg, "keyvalue pair %d=%s", i, myFunctionList[i].c_str());
//		Serial.println(msg);
//	}

    if (myFunctionList.size() < 7){
      Serial.println("WRONG_FRAME_SIZE");

      continue;
    }

    if (strcmp(myFunctionList[0].c_str(), FRAME_HEADER) != 0){
      sprintf(msg, "WRONG FRAME START expect=%s got=%s", FRAME_HEADER, myFunctionList[0].c_str());
      Serial.println(msg);

      continue;
    }

    if (strcmp(myFunctionList[6].c_str(), FRAME_END) != 0){
      sprintf(msg, "WRONG FRAME END expect=%s got=%s", FRAME_END, myFunctionList[6].c_str());
	  Serial.println(msg);
      continue;
    }

	//function
	sprintf(msg, "FUNCTION %s", myFunctionList[3].c_str());
	Serial.println(msg);
	int function = atoi(myFunctionList[3].c_str());

	switch(function){
			case 1:
				function_set_single_value(myFunctionList[5].c_str());
			break;
			case 2:
				function_take_off();
			break;
			case 3:
				function_rest();
			break;

			default:
				sprintf(msg, "FUNCTION %d unsuported", function);
				Serial.println(msg);
				continue;
	}



  }
  parseRxTask.disable();
  //force frameThread update
  Serial.println("<parseReceivedFrame");

}

void frameThread(){
    digitalWrite(LED_WATCHDOG, !digitalRead(LED_WATCHDOG));
    RcManager::sbusPreparePacket(false, false);
    RcSerial.write(RcManager::sbusPacket, SBUS_PACKET_LENGTH);
    if (print_count > 100){
		print_count = 0;
		Utils::print_hex(RcManager::sbusPacket, SBUS_PACKET_LENGTH, msg);
		Serial.println(msg);
	} else {
		print_count++;
	}

    digitalWrite(LED_WATCHDOG, !digitalRead(LED_WATCHDOG));

}

void setup() {

    for (uint8_t i = 0; i < SBUS_CHANNEL_NUMBER; i++) {
        RcManager::rcChannels[i] = 1500;
    }

    // define pin modes for tx, rx:
    pinMode(LED_WATCHDOG, OUTPUT);
    pinMode(RC_RX_PIN, INPUT);
    pinMode(RC_TX_PIN, OUTPUT);
    RcSerial.begin(100000);
    Serial.begin(115200);
    Serial.println("Serial Started!");

    runner.init();
    Serial.println("Initialized scheduler");

    runner.addTask(readSerialPort);
    runner.addTask(sendSbusFrame);
    //runner.addTask(ledBlinkTask);
    //ledBlinkTask.enable();
    runner.addTask(parseRxTask);
    readSerialPort.enable();
    sendSbusFrame.enable();
}


void loop() {

    runner.execute();


}
