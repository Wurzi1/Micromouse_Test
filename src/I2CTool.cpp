#include <I2CTool.h>
#include <Wire.h>

I2CTool::I2CTool(unsigned int I2C0ClockSpeed, unsigned int I2C1ClockSpeed)
: I2C0ClockSpeed(I2C0ClockSpeed), I2C1ClockSpeed(I2C1ClockSpeed)
{
    log_d("Trying to create I2CTool object.");

    if(Wire.begin(SDA0, SCL0, I2C0ClockSpeed)){
        log_i("I2C0 initiated succesfully.");
    }
    else{
        log_e("FAILED to initialize I2C0!");
    }

    if(Wire1.begin(SDA1, SCL1, I2C1ClockSpeed)){
        log_i("I2C1 initiated succesfully.");
    }
    else{
        log_e("FAILED to initialize I2C1!");
    }

}

void I2CTool::I2CScanner(){
    uint8_t Devices0 = 0;
    uint8_t Devices1 = 0;
    uint8_t error;
    uint8_t address;

    log_i("Scanning for I2C devices...");
    log_i("Scanning I2C0...");
    for(address = 1; address < 127; address++){
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if(error == 0){
            Devices0++;
            if(address < 16){
                log_i("I2C0 device found at address 0x0%X", address);
            }
            else{
                log_i("I2C0 device found at address 0x%X", address);
            }
        }
        else if(error == 4){
            if(address < 16){
                log_e("I2C0 error at address 0x0%X", address);
            }
            else{
                log_e("I2C0 error at address 0x%X", address);
            }
        }
    }
    log_i("I2C0 devices found: %d", Devices0);

    log_i("Scanning I2C1...");
    for(address = 1; address < 127; address++){
        Wire1.beginTransmission(address);
        error = Wire1.endTransmission();

        if(error == 0){
            Devices1++;
            if(address < 16){
                log_i("I2C1 device found at address 0x0%X", address);
            }
            else{
                log_i("I2C1 device found at address 0x%X", address);
            }
        }
        else if(error == 4){
            if(address < 16){
                log_e("I2C1 error at address 0x0%X", address);
            }
            else{
                log_e("I2C1 error at address 0x%X", address);
            }
        }
    }
    log_i("I2C1 devices found: %d", Devices1);
}