#include <TPL0102.h>
#include <I2CTool.h>
#include <Wire.h>



TPL0102::TPL0102(float highVoltage)
: highVoltage(highVoltage)
{
    log_d("Trying to create TPL0102 object.");

    int ex, en, di;

    ex = exitShutdown();
    en = enableNonVolatileWriting();

    DefaultWiperPosA, wiperPosA =  getWiperA();
    DefaultWiperPosB, wiperPosB =  getWiperB();

    di = disableNonVolatileWriting();

    if(ex == -1 || en == -1 || di == -1 || DefaultWiperPosA == -1 || wiperPosA == -1 || DefaultWiperPosB == -1 || wiperPosB == -1){
        log_e("Failed to create TPL0102 object!");
    }
    else{
        log_i("TPL0102 object initialized successfully.");
    }
}




int TPL0102::SetVolatileWiperA(uint8_t position){
    log_d("Trying to set WiperA to %d...", position);

    if(canWriteAutoRetry()){
        log_d("Setting WiperA...");
        Wire1.beginTransmission(address);
        Wire1.write(registerWiperA);

        Wire1.write(position);
        if (Wire1.endTransmission(true) != 0) {
            log_e("I2C write failed!");
            return -1;
        }
    }
    else{
        log_e("Failed to set WiperA!");
        return -1;
    }

    wiperPosA = position;

    log_d("WiperA set successfully.");
    return 0;
}

int TPL0102::SetVolatileWiperB(uint8_t position){
    log_d("Trying to set WiperB to %d...", position);

    if(canWriteAutoRetry()){
        log_d("Setting WiperB...");
        Wire1.beginTransmission(address);
        Wire1.write(registerWiperB);

        Wire1.write(position);
        if (Wire1.endTransmission(true) != 0) {
            log_e("I2C write failed!");
            return -1;
        }
    }
    else{
        log_e("Failed to set WiperB!");
        return -1;
    }

    wiperPosB = position;

    log_d("WiperB set successfully.");
    return 0;
}

int TPL0102::SetNonVolatileWiperA(uint8_t position){
    log_d("Trying to set non-volatile WiperA to %d...", position);

    if(enableNonVolatileWriting() == -1){
        log_e("Failed to set non-volatile WiperA!");
        return -1;
    }

    if(canWriteAutoRetry()){
        log_d("Setting non-volatile WiperA...");
        Wire1.beginTransmission(address);
        Wire1.write(registerWiperA);

        Wire1.write(position);
        if (Wire1.endTransmission(true) != 0) {
            log_e("I2C write failed!");
            return -1;
        }
    }
    else{
        log_e("Failed to set non-volatile WiperA!");
        return -1;
    }

    DefaultWiperPosA = position;

    if(disableNonVolatileWriting() == -1){
        log_e("Set non-volatile WiperA, but failed to disable non-volatile Writing!");
        return -1;
    }

    log_d("Non-volatile WiperA set successfully.");
    return 0;
}

int TPL0102::SetNonVolatileWiperB(uint8_t position){
    log_d("Trying to set non-volatile WiperB to %d...", position);

    if(enableNonVolatileWriting() == -1){
        log_e("Failed to set non-volatile WiperB!");
        return -1;
    }

    if(canWriteAutoRetry()){
        log_d("Setting non-volatile WiperB...");
        Wire1.beginTransmission(address);
        Wire1.write(registerWiperB);

        Wire1.write(position);
        if (Wire1.endTransmission(true) != 0) {
            log_e("I2C write failed!");
            return -1;
        }
    }
    else{
        log_e("Failed to set non-volatile WiperB!");
        return -1;
    }

    DefaultWiperPosB = position;

    if(disableNonVolatileWriting() == -1){
        log_e("Set non-volatile WiperB, but failed to disable non-volatile Writing!");
        return -1;
    }

    log_d("Non-volatile WiperB set successfully.");
    return 0;
}


int TPL0102::getWiperA(){
    log_d("Reading WiperA...");
    Wire1.beginTransmission(address);


    Wire1.write(registerWiperA);
    if (Wire1.endTransmission(false) != 0) {
        log_e("I2C write failed!");
        return -1;
    }


    uint8_t bytesReceived = Wire1.requestFrom(address, (uint8_t)1);
    if (bytesReceived != 1) {
        log_e("I2C read failed!");
        return -1;
    }

    wiperPosA = Wire1.read();

    log_d("WiperA read successfully: %d", wiperPosA);

    return wiperPosA;
}

int TPL0102::getWiperB(){
    log_d("Reading WiperB...");
    Wire1.beginTransmission(address);


    Wire1.write(registerWiperB);
    if (Wire1.endTransmission(false) != 0) {
        log_e("I2C write failed!");
        return -1;
    }


    uint8_t bytesReceived = Wire1.requestFrom(address, (uint8_t)1);
    if (bytesReceived != 1) {
        log_e("I2C read failed!");
        return -1;
    }

    wiperPosB = Wire1.read();

    log_d("WiperB read successfully: %d", wiperPosA);

    return wiperPosB;
}


int TPL0102::enableNonVolatileWriting(){
    log_d("Trying to enable non-volatile writing...");

    if(canWriteAutoRetry()){
        uint8_t output = 0b01000000;
        if(shutdownEnabled){
            output = 0b00000000;
        }

        log_d("Enabling non-volatile writing...");
        Wire1.beginTransmission(address);
        Wire1.write(registerSettings);

        Wire1.write(output);
        if (Wire1.endTransmission(true) != 0) {
            log_e("I2C write failed!");
            return -1;
        }
    }
    else{
        log_e("Failed to enable non-volatile writing!");
        return -1;
    }

    log_d("Non-volatile writing enabled successfully.");
    return 0;
}

int TPL0102::disableNonVolatileWriting(){
    log_d("Trying to disable non-volatile writing...");

    if(canWriteAutoRetry()){
        uint8_t output = 0b11000000;
        if(shutdownEnabled){
            output = 0b10000000;
        }

        log_d("Disabling non-volatile writing...");
        Wire1.beginTransmission(address);
        Wire1.write(registerSettings);

        Wire1.write(output);
        if (Wire1.endTransmission(true) != 0) {
            log_e("I2C write failed!");
            return -1;
        }
    }
    else{
        log_e("Failed to disable non-volatile writing!");
        return -1;
    }

    log_d("Non-volatile writing disabled successfully.");
    return 0;
}


int TPL0102::canWrite(){
    log_d("Checking if writable...");

    Wire1.beginTransmission(address);

    Wire1.write(registerSettings);
    if (Wire1.endTransmission(false) != 0) {
        log_e("I2C write failed!");
        return -1;
    }


    uint8_t bytesReceived = Wire1.requestFrom(address, (uint8_t)1);
    if (bytesReceived != 1) {
        log_e("I2C read failed!");
        return -1;
    }

    uint8_t currentSettings = Wire1.read();

    uint8_t WIP = (currentSettings & registerSettings_WIPMask) >> 5;



    log_d("WIP read successfully: %d", WIP);

    if(WIP){
        return 0;
    }
    return 1;
}

int TPL0102::canWriteAutoRetry(){
    log_d("Autor-Retry Checking if writable...");

    for(int i = 0; i < canWriteAutoRetryAttempts; i++){
        int canW = canWrite();
        if(canW && canW != -1){
            return 1;
        }
        else if(canW == -1){
            log_e("Something went wrong, waiting %dms and trying again!", canWriteAutoRetryDelay);
            delay(canWriteAutoRetryDelay);
        }
        else{
            log_e("WIP is set high, waiting %dms and trying again!", canWriteAutoRetryDelay);
            delay(canWriteAutoRetryDelay);
        }
    }

    log_e("After %d attempts there was no canWrite resolution!", canWriteAutoRetryAttempts);
    return 0;
}




int TPL0102::setVoltageA(float voltage){
    log_d("Trying to set VoltageA to %fV", voltage);

    if(voltage > highVoltage){
        log_e("Voltage target is higher that highVoltage!");
        log_e("Failed to set VoltageA!");
        return -1;
    }

    if(voltage < 0.0f){
        log_e("Voltage target is negative!");
        log_e("Failed to set VoltageA!");
        return -1;
    }

    float estPosf = (voltage / highVoltage) * 256;
    estPosf = std::roundf(estPosf);
    if(estPosf == 256) estPosf--;

    uint8_t estPos = static_cast<uint8_t>(estPosf);


    if(SetVolatileWiperA(estPos) == -1){
        log_e("Failed to set VoltageA!");
        return -1;
    }

    float actualVoltage = highVoltage * (estPosf / 256);
    log_d("VoltageA set to %fV", actualVoltage);
    return 0;
}

int TPL0102::setVoltageB(float voltage){
    log_d("Trying to set VoltageB to %fV", voltage);

    if(voltage > highVoltage){
        log_e("Voltage target is higher that highVoltage!");
        log_e("Failed to set VoltageB!");
        return -1;
    }

    if(voltage < 0.0f){
        log_e("Voltage target is negative!");
        log_e("Failed to set VoltageB!");
        return -1;
    }

    float estPosf = (voltage / highVoltage) * 256;
    estPosf = std::round(estPosf);
    if(estPosf == 256) estPosf--;

    uint8_t estPos = static_cast<uint8_t>(estPosf);

    if(SetVolatileWiperB(estPos) == -1){
        log_e("Failed to set VoltageB!");
        return -1;
    }

    float actualVoltage = highVoltage * (estPosf / 256);
    log_d("VoltageB set to %fV", actualVoltage);
    return 0;
}

int TPL0102::setDefaultVoltageA(float voltage){
    log_d("Trying to set Default-VoltageA to %fV", voltage);

    if(voltage > highVoltage){
        log_e("Voltage target is higher that highVoltage!");
        log_e("Failed to set Default-VoltageA!");
        return -1;
    }

    if(voltage < 0.0f){
        log_e("Voltage target is negative!");
        log_e("Failed to set Default-VoltageA!");
        return -1;
    }

    float estPosf = (voltage / highVoltage) * 256;
    estPosf = std::round(estPosf);
    if(estPosf == 256) estPosf--;

    uint8_t estPos = static_cast<uint8_t>(estPosf);

    if(SetNonVolatileWiperA(estPos) == -1){
        log_e("Failed to set Default-VoltageA!");
        return -1;
    }

    float actualVoltage = highVoltage * (estPosf / 256);
    log_d("Default-VoltageA set to %fV", actualVoltage);
    return 0;
}

int TPL0102::setDefaultVoltageB(float voltage){
    log_d("Trying to set Default-VoltageB to %fV", voltage);
    
    if(voltage > highVoltage){
        log_e("Voltage target is higher that highVoltage!");
        log_e("Failed to set Default-VoltageB!");
        return -1;
    }

    if(voltage < 0.0f){
        log_e("Voltage target is negative!");
        log_e("Failed to set Default-VoltageB!");
        return -1;
    }

    float estPosf = (voltage / highVoltage) * 256;
    estPosf = std::round(estPosf);
    if(estPosf == 256) estPosf--;

    uint8_t estPos = static_cast<uint8_t>(estPosf);

    if(SetNonVolatileWiperB(estPos) == -1){
        log_e("Failed to set Default-VoltageB!");
        return -1;
    }

    float actualVoltage = highVoltage * (estPosf / 256);
    log_d("Default-VoltageB set to %fV", actualVoltage);
    return 0;
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



int TPL0102::enterShutdown(){
    log_d("Trying to enter shutdown");
    
    if(canWriteAutoRetry()){
        uint8_t output = 0b10000000;

        log_d("Entering Shutdown...");
        Wire1.beginTransmission(address);
        Wire1.write(registerSettings);

        Wire1.write(output);
        if (Wire1.endTransmission(true) != 0) {
            log_e("I2C write failed!");
            return -1;
        }
    }
    else{
        log_e("Failed to enter shutdown!");
        return -1;
    }

    shutdownEnabled = true;

    log_d("Entered shutdown successfully.");
    return 0;
}

int TPL0102::exitShutdown(){
    log_d("Trying to exit shutdown");
    
    if(canWriteAutoRetry()){
        uint8_t output = 0b11000000;

        log_d("Exiting Shutdown...");
        Wire1.beginTransmission(address);
        Wire1.write(registerSettings);

        Wire1.write(output);
        if (Wire1.endTransmission(true) != 0) {
            log_e("I2C write failed!");
            return -1;
        }
    }
    else{
        log_e("Failed to exit shutdown!");
        return -1;
    }

    shutdownEnabled = false;

    log_d("Exited shutdown successfully.");
    return 0;
}

float TPL0102::getHighVoltage(){
    return highVoltage;
}
