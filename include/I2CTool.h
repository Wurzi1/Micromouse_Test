#pragma once
#include <functional>

// currently limited to 256 registers per device

class I2CTool {
    private:
        const int WRITERETRYATTEMPTS = 5;
        const int WRITERETRYDELAY = 1;
        const int READRETRYATTEMPTS = 5;
        const int READRETRYDELAY = 1;

    public:
        

        I2CTool(unsigned int I2C0ClockSpeed = 200000, unsigned int I2C1ClockSpeed = 50000);

        
        bool I2C0Write(uint8_t Address, uint8_t Register, uint8_t Data, bool AutoRetry = true);
        bool I2C0Write(uint8_t Address, uint8_t Register, uint16_t Data, bool AutoRetry = true);
        bool I2C0Write(uint8_t Address, uint8_t Register, const uint8_t* DataStart, size_t Size, bool AutoRetry = true);
        bool I2C1Write(uint8_t Address, uint8_t Register, uint8_t Data, bool AutoRetry = true);
        bool I2C1Write(uint8_t Address, uint8_t Register, uint16_t Data, bool AutoRetry = true);
        bool I2C1Write(uint8_t Address, uint8_t Register, const uint8_t* DataStart, size_t Size, bool AutoRetry = true);

        bool I2C0Read(uint8_t Address, uint8_t Register, uint8_t& ReadOutput, bool AutoRetry = true);
        bool I2C0Read(uint8_t Address, uint8_t Register, uint16_t& ReadOutput, bool AutoRetry = true);
        bool I2C0Read(uint8_t Address, uint8_t Register, uint8_t* ReadOutput, size_t Size, bool AutoRetry = true);
        bool I2C1Read(uint8_t Address, uint8_t Register, uint8_t& ReadOutput, bool AutoRetry = true);
        bool I2C1Read(uint8_t Address, uint8_t Register, uint16_t& ReadOutput, bool AutoRetry = true);
        bool I2C1Read(uint8_t Address, uint8_t Register, uint8_t* ReadOutput, size_t Size, bool AutoRetry = true);
        

        void flip(uint16_t& Data);
        void flip(uint8_t* Data, size_t Size);

        void I2CScanner();
};


extern I2CTool I2CT;