#include <TPL0102.h>
#include <I2CTool.h>
#include <Arduino.h>




TPL0102::TPL0102(float highVoltage)
: highVoltage(highVoltage)
{
    log_d("Trying to create TPL0102 object.");

    bool ex, en, wpa, wpb, nvw;

    ex = exitShutdown();
    en = enableNonVolatileWriting();

    wpa = getWiperA();
    wpb = getWiperB();
    
    nvw = disableNonVolatileWriting();

    if(!ex || ! en || ! wpa || !wpb || !nvw){
        log_e("Failed to create TPL0102 object!");
    }
    else{
        log_i("TPL0102 object initialized successfully.");
    }

    DefaultWiperPosA = wiperPosA;
    DefaultWiperPosB = wiperPosB;
}



bool TPL0102::SetVolatileWiperA(uint8_t position){
    log_d("Trying to set WiperA to %d...", position);
    if(canWriteAutoRetry() && I2CT.I2C1Write(address, registerWiperA, position)){
        wiperPosA = position;
        log_d("WiperA set successfully.");
        return true;
    }
    else{
        log_e("Failed to set WiperA!");
        return false;
    }
}

bool TPL0102::SetVolatileWiperB(uint8_t position){
    log_d("Trying to set WiperB to %d...", position);
    if(canWriteAutoRetry() && I2CT.I2C1Write(address, registerWiperB, position)){
        wiperPosB = position;
        log_d("WiperB set successfully.");
        return true;
    }
    else{
        log_e("Failed to set WiperB!");
        return false;
    }
}

bool TPL0102::SetNonVolatileWiperA(uint8_t position){
    log_d("Trying to set non-volatile WiperA to %d...", position);

    if(!enableNonVolatileWriting()){
        log_e("Failed to set non-volatile WiperA!");
        return false;
    }

    if(canWriteAutoRetry() && I2CT.I2C1Write(address, registerWiperA, position)){
        DefaultWiperPosA = position;
    }
    else{
        log_e("Failed to set non-volatile WiperA!");
        return false;
    }

    if(!disableNonVolatileWriting()){
        log_e("Set non-volatile WiperA, but failed to disable non-volatile Writing!");
        return false;
    }

    log_d("Non-volatile WiperA set successfully.");
    return true;
}

bool TPL0102::SetNonVolatileWiperB(uint8_t position){
    log_d("Trying to set non-volatile WiperB to %d...", position);

    if(!enableNonVolatileWriting()){
        log_e("Failed to set non-volatile WiperB!");
        return false;
    }

    if(canWriteAutoRetry() && I2CT.I2C1Write(address, registerWiperB, position)){
        DefaultWiperPosB = position;
    }
    else{
        log_e("Failed to set non-volatile WiperB!");
        return false;
    }

    if(!disableNonVolatileWriting()){
        log_e("Set non-volatile WiperB, but failed to disable non-volatile Writing!");
        return false;
    }

    log_d("Non-volatile WiperB set successfully.");
    return true;
}


bool TPL0102::getWiperA(){
    log_d("Reading WiperA...");
    if(I2CT.I2C1Read(address, registerWiperA, wiperPosA)){
        log_d("WiperA read successfully: %d", wiperPosA);
        return true;
    }
    else{
        log_e("Failed to read WiperA!");
        return false;
    }
}

bool TPL0102::getWiperB(){
    log_d("Reading WiperB...");
    if(I2CT.I2C1Read(address, registerWiperB, wiperPosB)){
        log_d("WiperB read successfully: %d", wiperPosB);
        return true;
    }
    else{
        log_e("Failed to read WiperB!");
        return false;
    }
}


bool TPL0102::enableNonVolatileWriting(){
    log_d("Trying to enable non-volatile writing...");

    uint8_t output = 0b01000000;
    if(shutdownEnabled){
        output = 0b00000000;
    }

    if(canWriteAutoRetry() && I2CT.I2C1Write(address, registerSettings, output)){
        log_d("Non-volatile writing enabled successfully.");
        return true;
    }
    else{
        log_e("Failed to enable non-volatile writing!");
        return false;
    }
}

bool TPL0102::disableNonVolatileWriting(){
    log_d("Trying to disable non-volatile writing...");

    uint8_t output = 0b11000000;
    if(shutdownEnabled){
        output = 0b10000000;
    }

    if(canWriteAutoRetry() && I2CT.I2C1Write(address, registerSettings, output)){
        log_d("Non-volatile writing disabled successfully.");
        return true;
    }
    else{
        log_e("Failed to disable non-volatile writing!");
        return false;
    }
}


bool TPL0102::canWrite(){
    log_d("Checking if writable...");

    uint8_t currentSettings;
    if(!I2CT.I2C1Read(address, registerSettings, currentSettings)){
        log_e("Failed to read WIP!");
        return false;
    }

    uint8_t WIP = (currentSettings & registerSettings_WIPMask) >> 5;

    log_d("WIP read successfully: %d", WIP);

    if(WIP){
        return false;
    }
    return true;
}

bool TPL0102::canWriteAutoRetry(){
    log_d("Autor-Retry Checking if writable...");
    for(int i = 0; i < canWriteAutoRetryAttempts; i++){
        if(canWrite()){
            log_d("Writable confirmed after %d tries.", ++i);
            return true;
        }
        else{
            log_e("WIP is set high (or something went wrong), waiting %dms and trying again!", canWriteAutoRetryDelay);
            delay(canWriteAutoRetryDelay);
        }
    }
    log_e("After %d attempts there was no canWrite resolution!", canWriteAutoRetryAttempts);
    return false;
}




bool TPL0102::setVoltageA(float voltage){
    log_d("Trying to set VoltageA to %fV", voltage);

    if(voltage > highVoltage){
        log_e("Voltage target is higher that highVoltage!");
        log_e("Failed to set VoltageA!");
        return false;
    }

    if(voltage < 0.0f){
        log_e("Voltage target is negative!");
        log_e("Failed to set VoltageA!");
        return false;
    }

    float estPosf = (voltage / highVoltage) * 256;
    estPosf = std::roundf(estPosf);
    if(estPosf == 256) estPosf--;

    uint8_t estPos = static_cast<uint8_t>(estPosf);


    if(!SetVolatileWiperA(estPos)){
        log_e("Failed to set VoltageA!");
        return false;
    }

    float actualVoltage = highVoltage * (estPosf / 256);
    log_d("VoltageA set to %fV", actualVoltage);
    return true;
}

bool TPL0102::setVoltageB(float voltage){
    log_d("Trying to set VoltageB to %fV", voltage);

    if(voltage > highVoltage){
        log_e("Voltage target is higher that highVoltage!");
        log_e("Failed to set VoltageB!");
        return false;
    }

    if(voltage < 0.0f){
        log_e("Voltage target is negative!");
        log_e("Failed to set VoltageB!");
        return false;
    }

    float estPosf = (voltage / highVoltage) * 256;
    estPosf = std::round(estPosf);
    if(estPosf == 256) estPosf--;

    uint8_t estPos = static_cast<uint8_t>(estPosf);

    if(!SetVolatileWiperB(estPos)){
        log_e("Failed to set VoltageB!");
        return false;
    }

    float actualVoltage = highVoltage * (estPosf / 256);
    log_d("VoltageB set to %fV", actualVoltage);
    return true;
}

bool TPL0102::setDefaultVoltageA(float voltage){
    log_d("Trying to set Default-VoltageA to %fV", voltage);

    if(voltage > highVoltage){
        log_e("Voltage target is higher that highVoltage!");
        log_e("Failed to set Default-VoltageA!");
        return false;
    }

    if(voltage < 0.0f){
        log_e("Voltage target is negative!");
        log_e("Failed to set Default-VoltageA!");
        return false;
    }

    float estPosf = (voltage / highVoltage) * 256;
    estPosf = std::round(estPosf);
    if(estPosf == 256) estPosf--;

    uint8_t estPos = static_cast<uint8_t>(estPosf);

    if(!SetNonVolatileWiperA(estPos)){
        log_e("Failed to set Default-VoltageA!");
        return false;
    }

    float actualVoltage = highVoltage * (estPosf / 256);
    log_d("Default-VoltageA set to %fV", actualVoltage);
    return true;
}

bool TPL0102::setDefaultVoltageB(float voltage){
    log_d("Trying to set Default-VoltageB to %fV", voltage);
    
    if(voltage > highVoltage){
        log_e("Voltage target is higher that highVoltage!");
        log_e("Failed to set Default-VoltageB!");
        return false;
    }

    if(voltage < 0.0f){
        log_e("Voltage target is negative!");
        log_e("Failed to set Default-VoltageB!");
        return false;
    }

    float estPosf = (voltage / highVoltage) * 256;
    estPosf = std::round(estPosf);
    if(estPosf == 256) estPosf--;

    uint8_t estPos = static_cast<uint8_t>(estPosf);

    if(!SetNonVolatileWiperB(estPos)){
        log_e("Failed to set Default-VoltageB!");
        return false;
    }

    float actualVoltage = highVoltage * (estPosf / 256);
    log_d("Default-VoltageB set to %fV", actualVoltage);
    return true;
}



float TPL0102::getVoltageA(){
    float posf = static_cast<float>(wiperPosA);

    float voltage = highVoltage * (posf / 256);

    log_d("VoltageA is %fV", voltage);
    return voltage;
}

float TPL0102::getVoltageB(){
    float posf = static_cast<float>(wiperPosB);

    float voltage = highVoltage * (posf / 256);

    log_d("VoltageB is %fV", voltage);
    return voltage;
}

float TPL0102::getDefaultVoltageA(){
    float posf = static_cast<float>(DefaultWiperPosA);

    float voltage = highVoltage * (posf / 256);

    log_d("Default-VoltageA is %fV", voltage);
    return voltage;
}

float TPL0102::getDefaultVoltageB(){
    float posf = static_cast<float>(DefaultWiperPosB);

    float voltage = highVoltage * (posf / 256);

    log_d("Default-VoltageB is %fV", voltage);
    return voltage;
}



bool TPL0102::enterShutdown(){
    log_d("Trying to enter shutdown");
    const uint8_t output = 0b10000000;

    if(canWriteAutoRetry() && I2CT.I2C1Write(address, registerSettings, output)){
        shutdownEnabled = true;
        log_d("Entered shutdown successfully.");
        return true;
    }
    else{
        log_e("Failed to enter shutdown!");
        return false;
    }
}

bool TPL0102::exitShutdown(){
    log_d("Trying to exit shutdown");
    const uint8_t output = 0b11000000;

    if(canWriteAutoRetry() && I2CT.I2C1Write(address, registerSettings, output)){
        shutdownEnabled = false;
        log_d("Exited shutdown successfully.");
        return true;
    }
    else{
        log_e("Failed to exit shutdown!");
        return false;
    }
}

float TPL0102::getHighVoltage(){
    return highVoltage;
}
