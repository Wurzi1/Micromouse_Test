#include <Arduino.h>

#include <I2CTool.h>
#include <SPITool.h>
#include <TPL0102.h>


/*
// put function declarations here:
void GPIOstates(byte PullX);

void checkGPIOshort();

void I2C_Scanner(int SCLpin, int SDApin, int clockSpeed = 200000, int iterations = 1);

int BQIntTemp();

int TMPLocalTemp();

int TPLSetVolt(byte channel, byte resVal);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  delay(2000);
  Serial.println("BEGIN: Serial.println");
  log_i("BEGIN: log_i");


  I2C_Scanner(35, 36, 50000, 10);
  //I2C_Scanner(45, 48, 200000, 4);

  //BQIntTemp();
  //TMPLocalTemp();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Serial.println");
  log_i("log_i");
  delay(5000);
}

// put function definitions here:
void GPIOstates(byte PullX){
  const int numGPIO = 34;
  int GPIOs[numGPIO] = {4, 5, 6, 7, 15, 16, 17, 18, 8, 3, 46, 9, 10, 11, 12, 13, 14, 21, 47, 48, 45, 0, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 2, 1};

  for(int i = 0; i < numGPIO; i++){
      int io = GPIOs[i];

      pinMode(io, PullX);
      int val = digitalRead(io);

      log_i("GPIO%d State is: %d \n", io, val);

  }
}

void checkGPIOshort(){
  const int numGPIO = 34;
  int GPIOs[numGPIO] = {4, 5, 6, 7, 15, 16, 17, 18, 8, 3, 46, 9, 10, 11, 12, 13, 14, 21, 47, 48, 45, 0, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 2, 1};

  for(int i = 0; i < numGPIO; i++){
      int io = GPIOs[i];

      pinMode(io, INPUT_PULLUP);

  }

  for(int i = 1; i < numGPIO - 1; i++){
      int io = GPIOs[i];

      pinMode(io, OUTPUT);
      digitalWrite(io, LOW);

      for(int j = 0; j < numGPIO; j++){
        int jo = GPIOs[j];
        int val = digitalRead(jo);

        if(j != i && val == LOW){
          log_e("SHORT!!!---------------------");
          log_i("GPIO%d  Short", jo);
        }
      }
      
      digitalWrite(io, HIGH);
      pinMode(io, INPUT_PULLUP);

      log_i("GPIO%d  Done", io);

      delay(100);
  }
}

void I2C_Scanner(int SCLpin, int SDApin, int clockSpeed, int iterations){
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning...");

  if(Wire.begin(SDApin, SCLpin, clockSpeed)){
    log_i("I2C initiated succesfully.");
  }
  else{
    log_e("FAILED to initialize I2C!");
  }

  for(iterations; iterations > 0; iterations--){
    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
        if (error == 0) {
          Serial.print("I2C device found at address 0x");
          if (address<16) {
            Serial.print("0");
          }
          Serial.println(address,HEX);
          nDevices++;
        }
        else if (error==4) {
          Serial.print("Unknow error at address 0x");
          if (address<16) {
            Serial.print("0");
          }
          Serial.println(address,HEX);
        }    
      }
  }
  
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  //delay(5000);   
}

int BQIntTemp(){
  const byte addr = 0x08;
  const byte tempComm = 0x28;


  if(Wire.begin(36, 35, 100000)){
    log_i("I2C initiated succesfully.");
  }
  else{
    log_e("FAILED to initialize I2C!");
  }

  Wire.beginTransmission(addr);
  Wire.write(tempComm);

  // Send repeated start, keep connection active
    if (Wire.endTransmission(false) != 0) {
        Serial.println("I2C write failed");
        return -1;
    }

    // Request 2 bytes from slave
    uint8_t bytesReceived = Wire.requestFrom(addr, (uint8_t)2);

    if (bytesReceived != 2) {
        Serial.println("I2C read failed");
        return -1;
    }

    uint8_t lsb = Wire.read();
    uint8_t msb = Wire.read();

    // Combine into 16-bit value
    uint16_t raw = ((uint16_t)msb << 8) | lsb;
    delay(100);
    Serial.print("Raw value: ");
    Serial.println(raw);

    // Optional hex output
    Serial.print("Hex: 0x");
    Serial.println(raw, HEX);

    float temperature = 1.0 * static_cast<float>(raw);

    log_i("BQ internal die-temperature: %f°C");


    for(int i = 0; i < 10; i++){
       Wire.beginTransmission(addr);
  Wire.write(tempComm);

  // Send repeated start, keep connection active
    if (Wire.endTransmission(false) != 0) {
        Serial.println("I2C write failed");
        return -1;
    }

    // Request 2 bytes from slave
    uint8_t bytesReceived = Wire.requestFrom(addr, (uint8_t)2);

    if (bytesReceived != 2) {
        Serial.println("I2C read failed");
        return -1;
    }

    uint8_t lsb = Wire.read();
    uint8_t msb = Wire.read();

    // Combine into 16-bit value
    uint16_t raw = ((uint16_t)msb << 8) | lsb;
    delay(100);
    Serial.print("Raw value: ");
    Serial.println(raw);

    // Optional hex output
    Serial.print("Hex: 0x");
    Serial.println(raw, HEX);

    float temperature = 1.0 * static_cast<float>(raw);

    log_i("BQ internal die-temperature: %f°C");

    delay(1000);
    }

    return 0;
}

int TMPLocalTemp(){
  const byte addr = 0x48;
  const byte tempComm = 0x00;


  if(Wire.begin(36, 35, 50000)){
    log_i("I2C initiated succesfully.");
  }
  else{
    log_e("FAILED to initialize I2C!");
  }

  Wire.beginTransmission(addr);
  Wire.write(tempComm);

  // Send repeated start, keep connection active
    if (Wire.endTransmission(false) != 0) {
        Serial.println("I2C write failed");
        return -1;
    }

    // Request 2 bytes from slave
    uint8_t bytesReceived = Wire.requestFrom(addr, (uint8_t)2);

    if (bytesReceived != 2) {
        Serial.println("I2C read failed");
        return -1;
    }

    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();

    // Combine into 16-bit value
    uint16_t raw = ((uint16_t)msb << 8) | lsb;
    delay(100);
    Serial.print("Raw value: ");
    Serial.println(raw);

    // Optional hex output
    Serial.print("Hex: 0x");
    Serial.println(raw, HEX);

    uint16_t temperat = ((uint16_t)msb << 5) | (lsb >> 3);

    float Treal = (static_cast<float>(temperat)) * 0.0625f;

    Serial.print("Real Temperature:");
    Serial.println(Treal);

    return 0;
}

int TPLSetVolt(byte channel, byte resVal){
  const byte addr = 0x50;
  const byte registerAdd = 0x01;


  if(Wire.begin(36, 35, 50000)){
    log_i("I2C initiated succesfully.");
  }
  else{
    log_e("FAILED to initialize I2C!");
  }

  Wire.beginTransmission(addr);
  Wire.write(registerAdd);

  Wire.write(resVal);

  // Send repeated start, keep connection active
    if (Wire.endTransmission(true) != 0) {
        Serial.println("I2C write failed");
        return -1;
    }

    return 0;
}
*/



void setup() {
  Serial.begin(9600);
  Serial.setDebugOutput(true);
  delay(2000);
  Serial.println("BEGIN: Serial.println");
  log_i("BEGIN: log_i");


  I2CTool toolw(200000, 50000);
  toolw.I2CScanner();

  TPL0102 tpl(3.4);

  delay(5000);

  float voltage = tpl.getVoltageA();
  log_i("VoltageA: %f", voltage);

  tpl.setVoltageA(3.0f);

  voltage = tpl.getVoltageA();
  log_i("VoltageA: %f", voltage);



  voltage = tpl.getVoltageB();
  log_i("VoltageB: %f", voltage);

  tpl.setVoltageB(3.3f);

  voltage = tpl.getVoltageB();
  log_i("VoltageB: %f", voltage);

  tpl.enterShutdown();

  delay(10000);

  tpl.exitShutdown();
}

void loop() {
  Serial.println("Serial.println");
  log_i("log_i");
  delay(5000);
}
