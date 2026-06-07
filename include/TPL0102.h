#pragma once
#include <stdint.h>

class TPL0102{
    private:
        const uint8_t address = 0x50;
        const uint8_t registerWiperA = 0x00;
        const uint8_t registerWiperB = 0x01;
        const uint8_t registerSettings = 0x10;
        const uint8_t registerSettings_VOLMask = 0b10000000;
        const uint8_t registerSettings_SHDNMask = 0b01000000;
        const uint8_t registerSettings_WIPMask = 0b00100000;

        const float highVoltage;
        uint8_t wiperPosA;
        uint8_t wiperPosB;
        uint8_t DefaultWiperPosA;
        uint8_t DefaultWiperPosB;
        bool shutdownEnabled;

        const uint16_t canWriteAutoRetryAttempts = 10;
        const uint16_t canWriteAutoRetryDelay = 50;
        

        int SetVolatileWiperA(uint8_t position);
        int SetVolatileWiperB(uint8_t position);
        int SetNonVolatileWiperA(uint8_t position);
        int SetNonVolatileWiperB(uint8_t position);

        int getWiperA();
        int getWiperB();

        int enableNonVolatileWriting();
        int disableNonVolatileWriting();

        int canWrite();
        int canWriteAutoRetry();


    public:
        TPL0102(float highVoltage);

        int setVoltageA(float voltage);
        int setVoltageB(float voltage);
        int setDefaultVoltageA(float voltage);
        int setDefaultVoltageB(float voltage);

        float getVoltageA();
        float getVoltageB();
        float getDefaultVoltageA();
        float getDefaultVoltageB();
        
        int enterShutdown();
        int exitShutdown();

        float getHighVoltage();
};