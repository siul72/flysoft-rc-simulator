#ifndef RCMANAGER_H
#define RCMANAGER_H

#include <Arduino.h>

#define RC_CHANNEL_MIN 990
#define RC_CHANNEL_MAX 2010

#define SBUS_MIN_OFFSET 173
#define SBUS_MID_OFFSET 992
#define SBUS_MAX_OFFSET 1811
#define SBUS_CHANNEL_NUMBER 16
#define SBUS_PACKET_LENGTH 25
#define SBUS_FRAME_HEADER 0x0f
#define SBUS_FRAME_FOOTER 0x00
#define SBUS_FRAME_FOOTER_V2 0x04
#define SBUS_STATE_FAILSAFE 0x08
#define SBUS_STATE_SIGNALLOSS 0x04
//#define SBUS_UPDATE_RATE 15 //ms
#define SBUS_UPDATE_RATE 10000
#define FRAME_HEADER "KBUS"
#define  FRAME_FUNCTION_SEND_SINGLE_POT_VALUE "SEQID0;"
#define FRAME_END "ENDK;"






class RcManager
{
public:
    RcManager();
    static void sbusPreparePacket(bool isSignalLoss, bool isFailsafe);
    void parser(char * rc_commands, int sizeOfCommand, char * received_message, int message_size);

    const String BF_START_WORD="AAAA";
    static uint8_t sbusPacket[SBUS_PACKET_LENGTH];
    static int rcChannels[SBUS_CHANNEL_NUMBER];
    static unsigned long sbusTime;
};

#endif // RCMANAGER_H
