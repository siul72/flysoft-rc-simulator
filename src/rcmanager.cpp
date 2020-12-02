#include "rcmanager.h"

unsigned long RcManager::sbusTime = 0;
uint8_t RcManager::sbusPacket[SBUS_PACKET_LENGTH] = {};
int RcManager::rcChannels[SBUS_CHANNEL_NUMBER] = {};

RcManager::RcManager(){

}

void RcManager::sbusPreparePacket(bool isSignalLoss, bool isFailsafe)
{
    static long output[SBUS_CHANNEL_NUMBER] = {0};

    /*
     * Map 1000-2000 with middle at 1500 chanel values to
     * 173-1811 with middle at 992 S.BUS protocol requires
     */
    for (uint8_t i = 0; i < SBUS_CHANNEL_NUMBER; i++) {
        output[i] = map(RcManager::rcChannels[i], RC_CHANNEL_MIN, RC_CHANNEL_MAX, SBUS_MIN_OFFSET, SBUS_MAX_OFFSET);
    }

    uint8_t stateByte = 0x00;
    if (isSignalLoss) {
        stateByte |= SBUS_STATE_SIGNALLOSS;
    }
    if (isFailsafe) {
        stateByte |= SBUS_STATE_FAILSAFE;
    }
    RcManager::sbusPacket[0] = SBUS_FRAME_HEADER; //Header

    RcManager::sbusPacket[1] = uint8_t (output[0] & 0x07FF);
    RcManager::sbusPacket[2] = uint8_t ((output[0] & 0x07FF)>>8 | (output[1] & 0x07FF)<<3);
    RcManager::sbusPacket[3] = uint8_t ((output[1] & 0x07FF)>>5 | (output[2] & 0x07FF)<<6);
    RcManager::sbusPacket[4] = uint8_t ((output[2] & 0x07FF)>>2);
    RcManager::sbusPacket[5] = uint8_t ((output[2] & 0x07FF)>>10 | (output[3] & 0x07FF)<<1);
    RcManager::sbusPacket[6] = uint8_t ((output[3] & 0x07FF)>>7 | (output[4] & 0x07FF)<<4);
    RcManager::sbusPacket[7] = uint8_t ((output[4] & 0x07FF)>>4 | (output[5] & 0x07FF)<<7);
    RcManager::sbusPacket[8] = uint8_t ((output[5] & 0x07FF)>>1);
    RcManager::sbusPacket[9] = uint8_t ((output[5] & 0x07FF)>>9 | (output[6] & 0x07FF)<<2);
    RcManager::sbusPacket[10] = uint8_t ((output[6] & 0x07FF)>>6 | (output[7] & 0x07FF)<<5);
    RcManager::sbusPacket[11] = uint8_t ((output[7] & 0x07FF)>>3);
    RcManager::sbusPacket[12] = uint8_t ((output[8] & 0x07FF));
    RcManager::sbusPacket[13] = uint8_t ((output[8] & 0x07FF)>>8 | (output[9] & 0x07FF)<<3);
    RcManager::sbusPacket[14] = uint8_t ((output[9] & 0x07FF)>>5 | (output[10] & 0x07FF)<<6);
    RcManager::sbusPacket[15] = uint8_t ((output[10] & 0x07FF)>>2);
    RcManager::sbusPacket[16] = uint8_t ((output[10] & 0x07FF)>>10 | (output[11] & 0x07FF)<<1);
    RcManager::sbusPacket[17] = uint8_t ((output[11] & 0x07FF)>>7 | (output[12] & 0x07FF)<<4);
    RcManager::sbusPacket[18] = uint8_t ((output[12] & 0x07FF)>>4 | (output[13] & 0x07FF)<<7);
    RcManager::sbusPacket[19] = uint8_t ((output[13] & 0x07FF)>>1);
    RcManager::sbusPacket[20] = uint8_t ((output[13] & 0x07FF)>>9 | (output[14] & 0x07FF)<<2);
    RcManager::sbusPacket[21] = uint8_t ((output[14] & 0x07FF)>>6 | (output[15] & 0x07FF)<<5);
    RcManager::sbusPacket[22] = uint8_t ((output[15] & 0x07FF)>>3);

    RcManager::sbusPacket[23] = stateByte; //Flags byte
    RcManager::sbusPacket[24] = SBUS_FRAME_FOOTER; //Footer

}

void RcManager::parser(char *rc_commands, int sizeOfCommand, char *received_message, int message_size)
{
    //Protocol definition: Frame: START_WORD;BUFFER_DATA_SIZE;DATA_TYPE,CH INDEX, CH_VALUE;CHKSUM;


    for (int index = 0; index < message_size; index++){

    }
}




