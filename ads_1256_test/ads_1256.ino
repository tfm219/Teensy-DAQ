//my code
//note...this is written for TEENSY meaning I am using DigitalWriteFast to speed things up.
//thus the CS pin must be hard coded. in my case, this is pin 21 but you will have to change it for yourself if needed
//see this post about how to use: https://forum.pjrc.com/threads/24573-Speed-of-digitalRead-and-digitalWrite-with-Teensy3-0

//built up on the work of:
//https://github.com/Flydroid/ADS12xx-Library
//https://gist.github.com/dariosalvi78/f2e990b4317199d235bbf5963c3486ae
//https://github.com/adienakhmad/ADS1256

#include <SPI.h>

int32_t lastVal;
int32_t lastLastVal;
bool lastLast;
int count;
/** Initializes ADS values */
void initADS() {
    //Serial.println(ADS_Gain);
    SPI.begin();
    lastVal = 0;
    count = 0;
    lastLast = true;
    //attachInterrupt(ADS_RDY_PIN, DRDY_Interuppt, FALLING);
    digitalWrite(ADS_RST_PIN, LOW);
    delay(10); // LOW at least 4 clock cycles of onboard clock. 100 microsecons is enough
    digitalWrite(ADS_RST_PIN, HIGH); // now reset to deafult values
  
    delay(1000);
  
    //now reset the ADS
    Reset();
  
    //let the system power up and stabilize (datasheet pg 24)
    delay(2000);
    //this enables the buffer which gets us more accurate voltage readings
    //SetRegisterValue(STATUS,B00110100);//B00110010);
    //delay(2000);
    Serial.println(GetRegisterValue(STATUS),BIN);
  
    //next set the mux register
    //we are only trying to read differential values from pins 0 and 1. your needs may vary.
    //this is the default setting so we can just reset it
    SetRegisterValue(MUX,MUX_RESET); //set the mux register
     //B00001000 for single ended measurement
  
    //now set the ADCON register
    //set the PGA to 64x
    //you need to adjust the constants for the other ones according to datasheet pg 31 if you need other values
    SetRegisterValue(ADCON, PGA_1); //set the adcon register
  
    //next set the data rate
    SetRegisterValue(DRATE, DR_30000); //set the drate register
  
    //we're going to ignore the GPIO for now...
  
    //lastly, we need to calibrate the system
  
    //let it settle
    delay(2000);
  
    //then do calibration
    SendCMD(SELFOCAL); //send the calibration command
  
    //then print out the values
    //delay(5);
    waitforDRDY();
    Serial.print("OFC0: ");
    Serial.println(GetRegisterValue(OFC0), BIN);
    Serial.print("OFC1: ");
    Serial.println(GetRegisterValue(OFC1), BIN);
    Serial.print("OFC2: ");
    Serial.println(GetRegisterValue(OFC2), BIN);
    Serial.print("FSC0: ");
    Serial.println(GetRegisterValue(FSC0), BIN);
    Serial.print("FSC1: ");
    Serial.println(GetRegisterValue(FSC1), BIN);
    Serial.print("FSC2: ");
    Serial.println(GetRegisterValue(FSC2), BIN);
}



/** 
 *  Reads one value 
 *  differential from (0,1)
 *  this assumes that we are not changing the mux action
 */
int32_t read_Value() {
    int32_t adc_val;
    waitforDRDY(); // Wait until DRDY is LOW
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
    digitalWriteFast(ADS_CS_PIN, LOW); //Pull SS Low to Enable Communications with ADS1247
    //delayMicroseconds(5); // RD: Wait 25ns for ADC12xx to get ready
    SPI.transfer(RDATA); //Issue RDATA
    delayClocks(50);
    //delayMicroseconds(7); //
    adc_val |= SPI.transfer(NOP);
    //delayMicroseconds(10);
    adc_val <<= 8;
    adc_val |= SPI.transfer(NOP);
    //delayMicroseconds(10);
    adc_val <<= 8;
    adc_val |= SPI.transfer(NOP);
    //delayMicroseconds(5);
    digitalWriteFast(ADS_CS_PIN, HIGH);
    SPI.endTransaction();
  
    if (adc_val > 0x7fffff) { //if MSB == 1
      adc_val = adc_val - 16777216; //do 2's complement, keep the sign this time!
    }  
    return adc_val;
}

void setChannelSingle(byte pChan) {
  setChannel(pChan, AINCOM);
}

void setChannel(byte pChan, byte nChan) {
    int32_t adc_val = 0; // unsigned long is on 32 bits
  
    digitalWriteFast(ADS_CS_PIN, LOW); //Speed test for faster location of this write
    //delayMicroseconds(10);
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
    //digitalWriteFast(ADS_CS_PIN, LOW); //Speed test for faster location of this write
    //delayMicroseconds(5);

    if (pChan != 0 && pChan <= 8) {
      Serial.print(pChan);
      Serial.print(", ");
      Serial.println(nChan);
      //Serial.println("LESS THAN 8");
      pChan = pChan << 4;
      Serial.println(pChan);
    }
    //byte data = (pChan << 4) | nChan; //xxxx1000 - P_AIN = pChan, N_AIN = nChan
    byte chans = pChan | nChan; //xxxx1000 - P_AIN = pChan, N_AIN = nChan
    
    //while (digitalRead(ADS_RDY_PIN)) {} ;
    //waitforDRDY();
    
    SPI.transfer(WREG | MUX); // send 1st command byte, address of the register // write (0x50) MUX register (0x01)
    SPI.transfer(0x00);     // send 2nd command byte, number of registers to be read/written − 1, write one register only
    SPI.transfer(chans);   // write the databyte to the register
    delayMicroseconds(2);
    
    waitforDRDY();
    
    //SYNC command 1111 1100
    SPI.transfer(SYNC);
    delayMicroseconds(5);
  
    //WAKEUP 0000 0000
    SPI.transfer(WAKEUP);
    delayMicroseconds(1);
  
    SPI.transfer(RDATA); // Read Data 0000  0001 (01h)
    delayMicroseconds(7);
}





/** 
 *  Read single ended with one channel defined
 */
int32_t readSingle(byte pChan) {
  return readDiff(pChan, AINCOM);
}

/**
 * Read differential, changes mux each read
 */
int32_t readDiff(byte pChan, byte nChan) {
    int32_t adc_val = 0; // unsigned long is on 32 bits
    //waitforDRDY();
    digitalWriteFast(ADS_CS_PIN, LOW); //Speed test for faster location of this write
    //delayMicroseconds(10);
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
    //digitalWriteFast(ADS_CS_PIN, LOW); //Speed test for faster location of this write
   // waitforDRDY();
    if (pChan != 0 && pChan <= 8) {
      Serial.print(pChan);
      Serial.print(", ");
      Serial.println(nChan);
      //Serial.println("LESS THAN 8");
      pChan = pChan << 4;
      Serial.println(pChan);
    }
    //byte data = (pChan << 4) | nChan; //xxxx1000 - P_AIN = pChan, N_AIN = nChan
    byte chans = pChan | nChan; //xxxx1000 - P_AIN = pChan, N_AIN = nChan
    
    //while (digitalRead(ADS_RDY_PIN)) {} ;
    waitforDRDY();
    
    SPI.transfer(WREG | MUX); // send 1st command byte, address of the register // write (0x50) MUX register (0x01)
    SPI.transfer(0x00);     // send 2nd command byte, number of registers to be read/written − 1, write one register only
    SPI.transfer(chans);   // write the databyte to the register
    //delayMicroseconds(1);
    pause4;
    
    //waitforDRDY();
    
    //SYNC command 1111 1100
    SPI.transfer(SYNC);
    //delayMicroseconds(3);
    pause24;
    //WAKEUP 0000 0000
    SPI.transfer(WAKEUP);
    //delayMicroseconds(1);
  
    SPI.transfer(RDATA); // Read Data 0000  0001 (01h)
    //delayMicroseconds(7);
    //delayClocks(50);
    pause50;
    
    adc_val = SPI.transfer(NOP);
    adc_val <<= 8; //shift to left
    adc_val |= SPI.transfer(NOP);
    adc_val <<= 8;
    adc_val |= SPI.transfer(NOP);
    
    digitalWrite(ADS_CS_PIN, HIGH);
    SPI.endTransaction();
    if (adc_val > 0x7fffff) { //if MSB == 1
      adc_val = adc_val - 16777216; //do 2's complement, keep the sign this time!
    }
    /*if (count < 4)
    {
      count++;
    }
    if (count == 4 && adc_val == lastLastVal)
    {
      lastLast = false;
    }
    if (lastLast)
    {
      lastLastVal = lastVal;
      lastVal = adc_val;
      return lastVal;
    }
    else 
    {
      lastVal = adc_val;
      return lastVal; 
    }*/
    return adc_val;
}



//library files

volatile int DRDY_state = HIGH;

void waitforDRDY() {
    while (digitalReadFast(ADS_RDY_PIN)) {}
    /*while (DRDY_state){}
    noInterrupts();
    DRDY_state = HIGH;
    interrupts();*/
    
}



//Interrupt function
void DRDY_Interuppt() {
  
    DRDY_state = LOW;
}



int32_t GetRegisterValue(uint8_t regAdress) {
    uint8_t bufr;
    digitalWriteFast(ADS_CS_PIN, LOW);
    delayMicroseconds(10);
    SPI.transfer(RREG | regAdress); // send 1st command byte, address of the register
    SPI.transfer(0x00);     // send 2nd command byte, read only one register
    delayMicroseconds(10);
    bufr = SPI.transfer(NOP); // read data of the register
    delayMicroseconds(10);
    digitalWriteFast(ADS_CS_PIN, HIGH);
    //digitalWrite(_START, LOW);
    SPI.endTransaction();
    return bufr;
}



void SendCMD(uint8_t cmd) {
    waitforDRDY();
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
    digitalWriteFast(ADS_CS_PIN, LOW);
    delayMicroseconds(10);
    SPI.transfer(cmd);
    delayMicroseconds(10);
    digitalWriteFast(ADS_CS_PIN, HIGH);
    SPI.endTransaction();
}



void Reset() {
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with  clock, MSB first, SPI Mode1
  digitalWriteFast(ADS_CS_PIN, LOW);
  delayMicroseconds(10);
  SPI.transfer(RESET); //Reset
  delay(2); //Minimum 0.6ms required for Reset to finish.
  SPI.transfer(SDATAC); //Issue SDATAC
  delayMicroseconds(100);
  digitalWriteFast(ADS_CS_PIN, HIGH);
  SPI.endTransaction();
}



void SetRegisterValue(uint8_t regAdress, uint8_t regValue) {
  uint8_t regValuePre = GetRegisterValue(regAdress);
  if (regValue != regValuePre) {
    //digitalWrite(_START, HIGH);
    delayMicroseconds(10);
    waitforDRDY();
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with SPI_SPEED, MSB first, SPI Mode1
    digitalWriteFast(ADS_CS_PIN, LOW);
    delayMicroseconds(10);
    SPI.transfer(WREG | regAdress); // send 1st command byte, address of the register
    SPI.transfer(0x00);   // send 2nd command byte, write only one register
    SPI.transfer(regValue);         // write data (1 Byte) for the register
    delayMicroseconds(10);
    digitalWriteFast(ADS_CS_PIN, HIGH);
    //digitalWrite(_START, LOW);
    if (regValue != GetRegisterValue(regAdress)) {   //Check if write was succesfull
      Serial.print("Write to Register 0x");
      Serial.print(regAdress, HEX);
      Serial.println(" failed!");
    }
    else {
      Serial.println("success");
    }
    SPI.endTransaction();
  }
}

double convertToVolts(double resolution, double gain, double vRef, double value) {
  double bitToVolt = (resolution*Gain)/(2 * vRef);
  return (value / bitToVolt);
}

double convertToVolts(double resolution, double gain, double vRef, int32_t value) {
  double bitToVolt = (resolution*Gain)/(2 * vRef);
  return (value / bitToVolt);
}

double convertToVolts(double bitToVolt, double value) {
  return (value / bitToVolt);
}

void delayClocks(uint32_t clocks) {
    do {
      __asm("nop");
    } while (--clocks);
}

/**
 * Set channel functions for ease of use
 */
void setChannelSingle_0() {
    setChannel(P_AIN0, AINCOM);
}
void setChannelSingle_1() {
    setChannel(P_AIN1, AINCOM);
}
void setChannelSingle_2() {
    setChannel(P_AIN2, AINCOM);
}
void setChannelSingle_3() {
    setChannel(P_AIN3, AINCOM);
}
void setChannelSingle_4() {
    setChannel(P_AIN4, AINCOM);
}
void setChannelSingle_5() {
    setChannel(P_AIN5, AINCOM);
}
void setChannelSingle_6() {
    setChannel(P_AIN6, AINCOM);
}
void setChannelSingle_7() {
    setChannel(P_AIN7, AINCOM);
}
void setChannel_0_1() {
    setChannel(P_AIN0, N_AIN1);
}
void setChannel_1_2() {
    setChannel(P_AIN1, N_AIN2);
}
void setChannel_2_3() {
    setChannel(P_AIN2, N_AIN3);
}
void setChannel_3_4() {
    setChannel(P_AIN3, N_AIN4);
}
void setChannel_4_5() {
    setChannel(P_AIN4, N_AIN5);
}
void setChannel_5_6() {
    setChannel(P_AIN5, N_AIN6);
}
void setChannel_6_7() {
    setChannel(P_AIN6, N_AIN7);
}



/**
 * Read functions for ease of use
 */
int32_t readSingle_0() {
    return readDiff(P_AIN0, AINCOM);
}
int32_t readSingle_1() {
    return readDiff(P_AIN1, AINCOM);
}
int32_t readSingle_2() {
    return readDiff(P_AIN2, AINCOM);
}
int32_t readSingle_3() {
    return readDiff(P_AIN3, AINCOM);
}
int32_t readSingle_4() {
  return readDiff(P_AIN4, AINCOM);
}
int32_t readSingle_5() {
    return readDiff(P_AIN5, AINCOM);
}
int32_t readSingle_6() {
    return readDiff(P_AIN6, AINCOM);
}
int32_t readSingle_7() {
  return readDiff(P_AIN7, AINCOM);
}
int32_t readDiff_0_1() {
  return readDiff(P_AIN0, N_AIN1);
}
int32_t readDiff_1_2() {
    return readDiff(P_AIN1, N_AIN2);
}
int32_t readDiff_2_3() {
    return readDiff(P_AIN2, N_AIN3);
}
int32_t readDiff_3_4() {
    return readDiff(P_AIN3, N_AIN4);
}
int32_t readDiff_4_5() {
    return readDiff(P_AIN4, N_AIN5);
}
int32_t readDiff_5_6() {
    return readDiff(P_AIN5, N_AIN6);
}
int32_t readDiff_6_7() {
    return readDiff(P_AIN6, N_AIN7);
}
