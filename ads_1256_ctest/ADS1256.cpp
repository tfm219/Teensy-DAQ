//my code
//note...this is written for TEENSY meaning I am using DigitalWriteFast to speed things up.
//thus the CS pin must be hard coded. in my case, this is pin 21 but you will have to change it for yourself if needed
//see this post about how to use: https://forum.pjrc.com/threads/24573-Speed-of-digitalRead-and-digitalWrite-with-Teensy3-0

//built up on the work of:
//https://github.com/Flydroid/ADS12xx-Library
//https://gist.github.com/dariosalvi78/f2e990b4317199d235bbf5963c3486ae
//https://github.com/adienakhmad/ADS1256

#include "ADS1256.h"

void ADS1256::waitforDRDY() {
    //while(DRDY_state){} 
    while (digitalReadFast(RDY_PIN)) {}
    //noInterrupts();
    //DRDY_state = HIGH;
    //interrupts();
}

ADS1256::ADS1256(int RST_PIN, int CS_PIN, int RDY_PIN, double Gain)
{
    this->RST_PIN = RST_PIN;
    this->CS_PIN = CS_PIN;
    this->RDY_PIN = RDY_PIN;
    this->Gain = Gain; 
    this->resolution =  pow(2.0, 24.0) - 1;
    this->dataRate = DR_30000;
    this->vRef = 5.0;
    this->DRDY_state = HIGH;
    //this->DRDY_state = digitalReadFast(RDY_PIN);
    //attachInterrupt(digitalPinToInterrupt(RDY_PIN), DRDY_interrupt , FALLING);
}


/** Initializes ADS values */
void ADS1256::beginADS() 
{
    pinMode(RDY_PIN, INPUT);
    pinMode(CS_PIN, OUTPUT);
    pinMode(RST_PIN, OUTPUT);
    //pState = LOW;
    /*pinMode(0, OUTPUT);
    pinMode(1, OUTPUT);
    pinMode(2, OUTPUT);
    digitalWriteFast(0, LOW);
    digitalWriteFast(1, LOW);
    digitalWriteFast(1, LOW);*/
  //delayClocks(250.);
    Serial.println("INIT");
    
    SPI.begin();
    
    digitalWrite(RST_PIN, LOW);
    delay(10); // LOW at least 4 clock cycles of onboard clock. 100 microsecons is enough
    digitalWrite(RST_PIN, HIGH); // now reset to deafult values
    
    delay(1000);
    //clockMult = 2;
    //now reset the ADS
    resetADS();
  
    //let the system power up and stabilize (datasheet pg 24)
    delay(2000);
    
    //this enables the buffer which gets us more accurate voltage readings
    //SetRegisterValue(STATUS,B00110100);//B00110010);
    
    Serial.println(getRegisterValue(STATUS),BIN);
    
    //next set the mux register
    //we are only trying to read differential values from pins 0 and 1. your needs may vary.
    //this is the default setting so we can just reset it
    setRegisterValue(MUX,MUX_RESET); //set the mux register
     //B00001000 for single ended measurement
    delay(2000);
    //now set the ADCON register
    //you need to adjust the constants for the other ones according to datasheet pg 31 if you need other values
    setRegisterValue(ADCON, PGA_1);
    if (Gain == 2)
    {
        setRegisterValue(ADCON, PGA_2); //set the adcon register
    }
    else if (Gain == 4)
    {
        setRegisterValue(ADCON, PGA_4);
    }
    else if (Gain == 8)
    {
        setRegisterValue(ADCON, PGA_8);
    }
    else if (Gain == 16)
    {
        setRegisterValue(ADCON, PGA_16);
    }
    else if (Gain == 32)
    {
        setRegisterValue(ADCON, PGA_32);
    }
    else if (Gain == 64)
    {
        setRegisterValue(ADCON, PGA_64);
    }
    
    //next set the data rate
    setRegisterValue(DRATE, DR_30000);//dataRate); //set the drate register
    //we're going to ignore the GPIO for now...
  
    //lastly, we need to calibrate the system
  
    //let it settle
    delay(2000);

    //then do calibration
    sendCMD(SELFCAL); //send the calibration command
    //then print out the values
    
    delay(10);
    waitforDRDY();
    Serial.print("OFC0: ");
    Serial.println(getRegisterValue(OFC0), BIN);
    Serial.print("OFC1: ");
    Serial.println(getRegisterValue(OFC1), BIN);
    Serial.print("OFC2: ");
    Serial.println(getRegisterValue(OFC2), BIN);
    Serial.print("FSC0: ");
    Serial.println(getRegisterValue(FSC0), BIN);
    Serial.print("FSC1: ");
    Serial.println(getRegisterValue(FSC1), BIN);
    Serial.print("FSC2: ");
    Serial.println(getRegisterValue(FSC2), BIN);
    bitToVolt = (resolution*Gain)/(2 * vRef);
    Serial.println("END INIT");
    delay(2000);
}

void ADS1256::setGain(int Gain)
{
  this->Gain = Gain;
}

void ADS1256::setDataRate(int dataRate)
{
  this->dataRate = dataRate;
}

/** 
 *  Reads one value 
 *  differential from (0,1)
 *  this assumes that we are not changing the mux action
 */
int32_t ADS1256::readValue() 
{
    unsigned long adc_val;// = 0;
    
    startSPI();

    //waitforDRDY();
    
    SPI.transfer(RDATA); // Read Data 0000  0001 (01h)
    //delayClocks(50);
    delayMicroseconds(7);
    
    adc_val |= SPI.transfer(0x00);
    adc_val <<= 8; //shift to left
    adc_val |= SPI.transfer(0x00);
    adc_val <<= 8;
    adc_val |= SPI.transfer(0x00);
    if (adc_val > 0x7fffff) { //if MSB == 1
        adc_val = adc_val - 16777216; //do 2's complement, keep the sign this time!
    }

    return adc_val;
}



void ADS1256::setChannel(byte pChan, byte nChan) 
{
    byte chans = pChan | nChan; //xxxx1000 - AIN = pChan, AIN = nChan
    
    startSPI();
    
    SPI.transfer(WREG | MUX); // send 1st command byte, address of the register // write (0x50) MUX register (0x01)   
    SPI.transfer(0x00);     // send 2nd command byte, number of registers to be read/written − 1, write one register only
    SPI.transfer(chans);   // write the databyte to the register
    delayMicroseconds(1);
    //SYNC command 1111 1100
    
    SPI.transfer(SYNC);
    delayMicroseconds(3);
    SPI.transfer(WAKEUP);
    
    endSPI();
    
}

/** 
 *  Read single ended with one channel defined
 */
int32_t ADS1256::readSingle(byte pChan) 
{
  return readDiff(pChan, AINCOM);
}

/**
 * Read differential, changes mux each read
 */
int32_t ADS1256::readDiff(byte pChan, byte nChan) 
{
    //int32_t adc_val = 0; // unsigned long is on 32 bits
    waitforDRDY();
    setChannel(pChan, nChan);
    return readValue();
    //byte data = (pChan << 4) | nChan; //xxxx1000 - P_AIN = pChan, N_AIN = nChan
    /*byte chans = pChan | nChan; //xxxx1000 - P_AIN = pChan, N_AIN = nChan
    
    waitforDRDY();
    
    SPI.transfer(WREG | MUX); // send 1st command byte, address of the register // write (0x50) MUX register (0x01)
    SPI.transfer(0x00);     // send 2nd command byte, number of registers to be read/written − 1, write one register only
    SPI.transfer(chans);   // write the databyte to the register
    //digitalWriteFast(0, HIGH);
    delayMicroseconds(1);
    //pause4;
    //digitalWriteFast(0, LOW);
    
    
    //waitforDRDY();
    
    //SYNC command 1111 1100
    SPI.transfer(SYNC);
    //digitalWriteFast(1, HIGH);
    delayMicroseconds(3);
    //pause24;
    //digitalWriteFast(1, LOW);
    
    //WAKEUP 0000 0000
    SPI.transfer(WAKEUP);
    //delayMicroseconds(1);
  
    SPI.transfer(RDATA); // Read Data 0000  0001 (01h)
    //digitalWriteFast(2, HIGH);
    delayMicroseconds(7);
    //pause50;
    //digitalWriteFast(2, LOW);
    
    adc_val = SPI.transfer(NOP);
    adc_val <<= 8; //shift to left
    adc_val |= SPI.transfer(NOP);
    adc_val <<= 8;
    adc_val |= SPI.transfer(NOP);

    delayMicroseconds(1);
    
    endSPI();
    
    if (adc_val > 0x7fffff) { //if MSB == 1
      adc_val = adc_val - 16777216; //do 2's complement, keep the sign this time!
    }
    
    return adc_val;*/
}



int32_t ADS1256::getRegisterValue(uint8_t regAdress) 
{
    uint8_t bufr;
    startSPI();
    delayMicroseconds(10);
    SPI.transfer(RREG | regAdress); // send 1st command byte, address of the register
    SPI.transfer(0x00);     // send 2nd command byte, read only one register
    delayMicroseconds(10);
    bufr = SPI.transfer(0x00); // read data of the register
    delayMicroseconds(10);
    //digitalWrite(_START, LOW);
    endSPI();
    return bufr;
}



void ADS1256::sendCMD(uint8_t cmd) {
    startSPI();
    delayMicroseconds(10);
    SPI.transfer(cmd);
    delayMicroseconds(10);
    endSPI();
}



void ADS1256::resetADS() {
  startSPI();
  delayMicroseconds(10);
  SPI.transfer(RESET); //Reset
  delay(2); //Minimum 0.6ms required for Reset to finish.
  SPI.transfer(SDATAC); //Issue SDATAC
  delayMicroseconds(100);
  endSPI();
}



void ADS1256::setRegisterValue(uint8_t regAdress, uint8_t regValue) {
  uint8_t regValuePre = getRegisterValue(regAdress);
  if (regValue != regValuePre) {
    delayMicroseconds(10);
    waitforDRDY();
    startSPI();
    delayMicroseconds(10);
    SPI.transfer(WREG | regAdress); // send 1st command byte, address of the register
    SPI.transfer(0x00);   // send 2nd command byte, write only one register
    SPI.transfer(regValue);         // write data (1 Byte) for the register
    delayMicroseconds(10);
    //digitalWrite(_START, LOW);
    if (regValue != getRegisterValue(regAdress)) {   //Check if write was succesfull
      Serial.print("Write to Register 0x");
      Serial.print(regAdress, HEX);
      Serial.println(" failed!");
    }
    else {
      Serial.println("success");
    }
    endSPI();
  }
}


double ADS1256::convertToVolts(double value) 
{
  return (value / bitToVolt);
}

/*void ADS1256::delayM(uint32_t nMicros)
{
  uint32_t startMicros = micros();
  do //while (nMicros >= (micros() - startMicros))
  {
    continue;
  } while (nMicros > (micros() - startMicros));
  //dM(nMicros, micros());
}

void ADS1256::dM(uint32_t nMicros, uint32_t startMicros)
{
  if (nMicros >= (micros() - startMicros))
  {
    return;
    //dM(nMicros, startMicros);
  }
  dM(nMicros, startMicros);
}*/

/*void ADS1256::delayClocks(double clocks) 
{
  //clocks *= clockMult;
  //dClk(clocks);
  do
  {
    __asm__ __volatile__("nop\n\t");
    __asm__ __volatile__("nop\n\t");
    __asm__ __volatile__("nop\n\t");
  } while (--clocks);
}
void ADS1256:: dClk(double clocks)
{
  if (clocks == 0)
  {
    return;
  }*/
  //__asm__ __volatile__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
  /*__asm__ __volatile__("nop\n\t");
  __asm__ __volatile__("nop\n\t");
  __asm__ __volatile__("nop\n\t");*/
    //dClk(--clocks);
//}

int ADS1256::getRDYPin(){
  return RDY_PIN;
}
int ADS1256::getCSPin(){
  return CS_PIN;
}
int ADS1256::getRSTPin(){
  return RST_PIN;
}

void ADS1256::setDRDYLow()
{
  this->DRDY_state = LOW;
}

void ADS1256::startSPI()
{
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
  digitalWriteFast(CS_PIN, LOW);
}

void ADS1256::endSPI()
{
  digitalWriteFast(CS_PIN, HIGH);
    SPI.endTransaction();
}
/**
 * Set channel functions for ease of use
 */
void ADS1256::setChannel(byte pChan) 
{
  setChannel(pChan, AINCOM);
}
 
void ADS1256::setChannel_0() 
{
    setChannel(AIN0, AINCOM);
}
void ADS1256::setChannel_1() 
{
    setChannel(AIN1, AINCOM);
}
void ADS1256::setChannel_2() 
{
    setChannel(AIN2, AINCOM);
}
void ADS1256::setChannel_3() 
{
    setChannel(AIN3, AINCOM);
}
void ADS1256::setChannel_4() 
{
    setChannel(AIN4, AINCOM);
}
void ADS1256::setChannel_5() 
{
    setChannel(AIN5, AINCOM);
}
void ADS1256::setChannel_6() 
{
    setChannel(AIN6, AINCOM);
}
void ADS1256::setChannel_7() 
{
    setChannel(AIN7, AINCOM);
}
void ADS1256::setChannel_0_1() 
{
    setChannel(AIN0, AIN1);
}
void ADS1256::setChannel_1_2() 
{
    setChannel(AIN1, AIN2);
}
void ADS1256::setChannel_2_3() 
{
    setChannel(AIN2, AIN3);
}
void ADS1256::setChannel_3_4() 
{
    setChannel(AIN3, AIN4);
}
void ADS1256::setChannel_4_5() 
{
    setChannel(AIN4, AIN5);
}
void ADS1256::setChannel_5_6() 
{
    setChannel(AIN5, AIN6);
}
void ADS1256::setChannel_6_7() 
{
    setChannel(AIN6, AIN7);
}

int32_t ADS1256::readSingle_0() 
{
    return readDiff(AIN0, AINCOM);
}
int32_t ADS1256::readSingle_1() 
{
    return readDiff(AIN1, AINCOM);
}
int32_t ADS1256::readSingle_2() 
{
    return readDiff(AIN2, AINCOM);
}
int32_t ADS1256::readSingle_3() 
{
    return readDiff(AIN3, AINCOM);
}
int32_t ADS1256::readSingle_4() 
{
  return readDiff(AIN4, AINCOM);
}
int32_t ADS1256::readSingle_5() 
{
    return readDiff(AIN5, AINCOM);
}
int32_t ADS1256::readSingle_6() 
{
    return readDiff(AIN6, AINCOM);
}
int32_t ADS1256::readSingle_7() 
{
  return readDiff(AIN7, AINCOM);
}
int32_t ADS1256::readDiff_0_1() 
{
  return readDiff(AIN0, AIN1);
}
int32_t ADS1256::readDiff_1_2() 
{
    return readDiff(AIN1, AIN2);
}
int32_t ADS1256::readDiff_2_3() 
{
    return readDiff(AIN2, AIN3);
}
int32_t ADS1256::readDiff_3_4() 
{
    return readDiff(AIN3, AIN4);
}
int32_t ADS1256::readDiff_4_5() {
    return readDiff(AIN4, AIN5);
}
int32_t ADS1256::readDiff_5_6() 
{
    return readDiff(AIN5, AIN6);
}
int32_t ADS1256::readDiff_6_7() 
{
    return readDiff(AIN6, AIN7);
}
