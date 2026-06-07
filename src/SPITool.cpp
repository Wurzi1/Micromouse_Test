#include <SPITool.h>
#include <SPI.h>
#include <Arduino.h>

SPITool::SPITool(unsigned int ClockSpeed)
: ClockSpeed(ClockSpeed)
{
    log_d("Trying to create SPITool object.");
    
    // configure CS pin
    pinMode(LSM_CS, OUTPUT);
    digitalWrite(LSM_CS, HIGH);

    SPISettings SPIsett(ClockSpeed, MSBFIRST, SPI_MODE0);
    SPI.begin(SCLK, MISO, MOSI);
    SPI.beginTransaction(SPIsett);

    log_i("SPI initiated.");
}