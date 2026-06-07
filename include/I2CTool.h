#pragma once
#include <Arduino.h>

class I2CTool {
    private:


    public:
        unsigned int I2C0ClockSpeed;
        unsigned int I2C1ClockSpeed;

        I2CTool(unsigned int I2C0ClockSpeed = 200000, unsigned int I2C1ClockSpeed = 50000);

        //void changeClockSpeed(); don't need that

        void I2CScanner();
};