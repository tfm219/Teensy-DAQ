//using the definitions in this library: https://github.com/Flydroid/ADS12xx-Library
#infnef _ads_1256_h
#define _ads_1256_h

#define SPI_SPEED 2500000


/* For information to the register and settings see manual page (p..) */

/* ADS1248 Register (see p42 for Register Map) */

#define    STATUS    0x00 //Status Control Register 0
#define   MUX     0x01 //Multiplexer Control Register 0
#define   ADCON     0x02 //A/D Control Register 0
#define   DRATE   0x03 //A/D Data Rate Control Register 0
#define   IO        0X04 //GPIO Control Register 0
#define   OFC0    0x05 //Offset Calibration Coefficient Register 1
#define   OFC1    0x06 //Offset Calibration Coefficient Register 2
#define   OFC2    0x07 //Offset Calibration Coefficient Register 2
#define   FSC0    0x08 //Full scale Callibration Coefficient Register 0
#define   FSC1    0x09 //Full scale Callibration Coefficient Register 1
#define   FSC2    0x0A //Full scale Callibration Coefficient REgister 2

/*STATUS - Status Control Register 0 ( see p30)*/
/* BIT7 - BIT6 -  BIT5   -  BIT4   -  BIT3   -  BIT2   -  BIT1   -  BIT0 */
/* ID   - ID   -  ID     -  ID     -  ORDER  -  ACAL   -  BUFEN  -  DRDY */
#define STATUS_RESET 0x01 // Reset STATUS Register
/*Bits 7 - 4 ID3, ID2, ID1, ID0 Factory Programmed Identification Bits(Read Only)*/
/*ORDER1:0  Data Output Bit Order*/
#define ORDER_MSB B00000000 // Most significant Bit first (default)
#define ORDER_LSB B00001000//Least significant Bit first
/*Input data is always shifted in most significant byte and bit first. Output data is always shifted out most significant
byte first. The ORDER bit only controls the bit order of the output data within the byte.*/
/*ACAL1:0 Auto Calibration*/
#define ACAL_OFF B00000000 // Auto Calibration Disabled (default)
#define ACAL_ON  B00000100 // Auto Calibration Enabled
/*When Auto-Calibration is enabled, self-calibration begins at the completion of the WREG command that changes
the PGA (bits 0-2 of ADCON register), DR (bits 7-0 in the DRATE register) or BUFEN (bit 1 in the STATUS register)
values.*/
/*BUFEN1:0 Analog Input Buffer Enable*/
#define BUFEN_OFF B00000000 //Buffer Disabled (default)
#define BUFEN_ON  B00000010 //BUffer Enabled
/*DRDY1:0 Data Ready (Read Only) Duplicates the state of the DRDY pin*/

/* MUX - Multiplexer Control Register 0 (see p31 - bring together with bitwise OR | */
/* BIT7  - BIT6  -  BIT5   -  BIT4   -  BIT3   -  BIT2   -  BIT1   -  BIT0 */
/* PSEL3 - PSEL2 -  PSEL1  -  PSEL0  -  NSEL3  -  NSEL2   - NSEL1   - NSEL0 */
#define MUX_RESET 0x01      // Reset MUX0 Register
/* PSEL3:0 Positive input channel selection bits */
#define P_AIN0 B00000000 //(default)
#define P_AIN1 B00010000
#define P_AIN2 B00100000
#define P_AIN3 B00110000
#define P_AIN4 B01000000
#define P_AIN5 B01010000
#define P_AIN6 B01100000
#define P_AIN7 B01110000
#define P_AINCOM B10000000
/* NSEL3:0 Negativ input channel selection bits */
#define N_AIN0 B00000000
#define N_AIN1 B00000001 //(default)
#define N_AIN2 B00000010
#define N_AIN3 B00000011
#define N_AIN4 B00000100
#define N_AIN5 B00000101
#define N_AIN6 B00000110
#define N_AIN7 B00000111
#define N_AINCOM B00001000

/*ADCON - A/D Control Register 0 ( see p31)*/
/* BIT7 - BIT6   -  BIT5   -  BIT4   -  BIT3   -  BIT2   -  BIT1   -  BIT0 */
/* 0    - CLK1   -  CLK0   -  SDCS1  -  SDCS0  -  PGA2   -  PGA1   -  PAG0 */
#define ADCON_RESET 0x20 // Reset ADCON Register
/*CLK2:0 D0/CLKOUT Clock Out Rate Setting*/
#define CLK_OFF B00000000 //Clock Out off
#define CLK_1   B00100000 //Clock Out Frequency = fCLKIN (default)
#define CLK_2   B01000000 //Clock Out Frequency = fCLKIN/2
#define CLK_4   B01100000 //Clock Out Frequency = fCLKIN/4
/*When not using CLKOUT, it is recommended that it be turned off. These bits can only be reset using the RESET pin.*/
/*SDCS2:0 Sensor Detection Current Sources*/
#define SDCS_OFF B00000000//Sensor Detect Off (default)
#define SDCS_05  B00001000//Sensor Detect Current 0.5?A
#define SDCS_2   B00010000//Sensor Detect Current 2?A
#define SDCS_10  B00011000//Sensor Detect Current 10?A
/*The Sensor Detect Current Sources can be activated to verify the integrity of an external sensor supplying a signal to the
ADS1255/6. A shorted sensor produces a very small signal while an open-circuit sensor produces a very large signal.*/
/*PGA3:0 Programmable Gain Amplifier Setting*/
#define PGA_1 //(default)
#define PGA_2
#define PGA_4
#define PGA_8
#define PGA_16
#define PGA_32
#define PGA_64 B00100111

/*DRATE - A/D Data Rate Register 0 ( see p32)*/
/* BIT7 - BIT6   -  BIT5   -  BIT4   -  BIT3   -  BIT2   -  BIT1   -  BIT0 */
/* DR7  - DR6    -  DR5    -  DR4    -  DR3    -  DR2    -  DR1    -  DR0 */
#define DRATE_RESET 0xF0 // Reset DRATE Register
/*DR7:0 Data Rate Setting*/
#define DR_30000 B11110000 //30.000 SPS (default)
#define DR_15000 B11100000 //15.000 SPS
#define DR_7500  B11010000 //7.500 SPS
#define DR_3750  B11000000 //3.750 SPS
#define DR_2000  B10110000 //2.000 SPS
#define DR_1000  B10100001 //1.000 SPS
#define DR_500   B10010010 //500 SPS
#define DR_100   B10000010 //100 SPS
#define DR_60    B01110010 //60 SPS
#define DR_50    B01100011 //50 SPS
#define DR_30    B01010011 //30 SPS
#define DR_25    B01000011 //25 SPS
#define DR_15    B00110011 //15 SPS
#define DR_10    B00100011 //10 SPS
#define DR_5     B00010011 //5 SPS
#define DR2_5    B00000011 //2,5 SPS

/*IO - GPIO Control Register 0 ( see p32)*/
/* BIT7 - BIT6   -  BIT5   -  BIT4   -  BIT3   -  BIT2   -  BIT1   -  BIT0 */
/* DIR3 - DIR2   -  DIR1   -  DIR0   -  DIO3   -  DIO2   -  DIO1   -  DIO0 */
#define IO_RESET 0xE0 // Reset IO Register
/*DIR3 - Digital I/O Direction for Pin D3*/
#define DIR3_OUT B00000000 //D3 is an output
#define DIR_IN   B10000000 //D3 is an input (default)
/*DIR2 - Digital I/O Direction for Pin D3*/
#define DIR2_OUT B00000000 //D2 is an output
#define DIR2_IN   B01000000 //D2 is an input (default)
/*DIR1 - Digital I/O Direction for Pin D3*/
#define DIR1_OUT B00000000 //D1 is an output
#define DIR1_IN   B00100000 //D1 is an input (default)
/*DIR0 - Digital I/O Direction for Pin D3*/
#define DIR0_OUT B00000000 //D0/CLKOUT is an output
#define DIR0_IN   B00010000 //D0/CLKOUT is an input (default)
/*DIO3:0 Status of Digital I/O, Read Only*/

/* SPI COMMAND DEFINITIONS (p34) */
/*SYSTEM CONTROL */
#define   WAKEUP    0x00  //Exit Sleep Mode
#define   STANDBY   0xFD  //Enter Sleep Mode
#define   SYNC    0xFC    //Synchornize the A/D Conversion
#define   RESET   0xFE  //Reset To Power UP values
#define   NOP     0xFF  //No operation
/*DATA READ*/
#define   RDATA   0x01  //Read data once
#define   RDATAC    0x03  //Read data continously
#define   SDATAC    0x0F  //Stop reading data continously
/*READ REGISTER */
#define   RREG    0x10  //Read From Register
#define   WREG    0x50  //Write To Register
/*Calibration */
#define   SYSOCAL   0xF3  //System Offset Calibration
#define   SYSGCAL   0xF2  //System Gain Calibration
#define   SELFCAL     0xF0  //Self Offset Calibration

//my code
//note...this is written for TEENSY meaning I am using DigitalWriteFast to speed things up.
//thus the CS pin must be hard coded. in my case, this is pin 21 but you will have to change it for yourself if needed
//see this post about how to use: https://forum.pjrc.com/threads/24573-Speed-of-digitalRead-and-digitalWrite-with-Teensy3-0

//built up on the work of:
//https://github.com/Flydroid/ADS12xx-Library
//https://gist.github.com/dariosalvi78/f2e990b4317199d235bbf5963c3486ae
//https://github.com/adienakhmad/ADS1256

class ADS_1256
{
  public:
    void initADS() {
      attachInterrupt(ADS_RDY_PIN, DRDY_Interuppt, FALLING);
    
      digitalWrite(ADS_RST_PIN, LOW);
      delay(10); // LOW at least 4 clock cycles of onboard clock. 100 microsecons is enough
      digitalWrite(ADS_RST_PIN, HIGH); // now reset to deafult values
    
      delay(1000);
    
      //now reset the ADS
      Reset();
    
      //let the system power up and stabilize (datasheet pg 24)
      delay(2000);
      //this enables the buffer which gets us more accurate voltage readings
     // SetRegisterValue(STATUS,B00110010);
    
      Serial.println(GetRegisterValue(STATUS));
    
      //next set the mux register
      //we are only trying to read differential values from pins 0 and 1. your needs may vary.
      //this is the default setting so we can just reset it
      SetRegisterValue(MUX,MUX_RESET); //set the mux register
       //B00001000 for single ended measurement
    
      //now set the ADCON register
      //set the PGA to 64x
      //you need to adjust the constants for the other ones according to datasheet pg 31 if you need other values
      SetRegisterValue(ADCON, PGA_64); //set the adcon register
    
      //next set the data rate
      SetRegisterValue(DRATE, DR_30000); //set the drate register
    
      //we're going to ignore the GPIO for now...
    
      //lastly, we need to calibrate the system
    
      //let it settle
      delay(2000);
    
      //then do calibration
      SendCMD(SELFCAL); //send the calibration command
    
      //then print out the values
      delay(5);
    
      Serial.print("OFC0: ");
      Serial.println(GetRegisterValue(OFC0));
      Serial.print("OFC1: ");
      Serial.println(GetRegisterValue(OFC1));
      Serial.print("OFC2: ");
      Serial.println(GetRegisterValue(OFC2));
      Serial.print("FSC0: ");
      Serial.println(GetRegisterValue(FSC0));
      Serial.print("FSC1: ");
      Serial.println(GetRegisterValue(FSC1));
      Serial.print("FSC2: ");
      Serial.println(GetRegisterValue(FSC2));
    }
    
    //function to read a value
    //this assumes that we are not changing the mux action
    int32_t read_Value() {
      int32_t adc_val;
      waitforDRDY(); // Wait until DRDY is LOW
      SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
      digitalWriteFast(21, LOW); //Pull SS Low to Enable Communications with ADS1247
      //delayMicroseconds(5); // RD: Wait 25ns for ADC12xx to get ready
      SPI.transfer(RDATA); //Issue RDATA
      delayMicroseconds(7);
      adc_val |= SPI.transfer(NOP);
      //delayMicroseconds(10);
      adc_val <<= 8;
      adc_val |= SPI.transfer(NOP);
      //delayMicroseconds(10);
      adc_val <<= 8;
      adc_val |= SPI.transfer(NOP);
      //delayMicroseconds(5);
      digitalWriteFast(21, HIGH);
      SPI.endTransaction();
    
      if (adc_val > 0x7fffff) { //if MSB == 1
        adc_val = adc_val - 16777216; //do 2's complement, keep the sign this time!
      }
    
      return adc_val;
    }
    
    //this function will go through the process of resetting the system and cycling through the inputs
    //it assumes that whatever was called before left the mux at the default (0,1)
    //this means it will treat the first value it receives as from (0,1)
    //and the second from (2,3).
    
    void read_two_values() {
      //datasheet page 21 at the bottom gives the timing
      int32_t adc_val1;
      int32_t adc_val2;
    
      waitforDRDY(); // Wait until DRDY is LOW
      SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
      digitalWriteFast(21, LOW); //Pull SS Low to Enable Communications with ADS1247
      //delayMicroseconds(5); // RD: Wait 25ns for ADC12xx to get ready
    
      //now change the mux register
      SPI.transfer(WREG | MUX); // send 1st command byte, address of the register
      SPI.transfer(0x00);   // send 2nd command byte, write only one register
      SPI.transfer(0x23);     //pins registers 2 and 3
    
      //now we need to sync
      //need to delay by 4x SPI clock = 2.35 uS (t1)
      //to be safe 5 uS
      delayMicroseconds(2);
      SPI.transfer(SYNC);
    
      //again delay by t1
      delayMicroseconds(5);
      //send wakeup
      SPI.transfer(WAKEUP);
    
      //then delay one more time by t1 before rdata
      delayMicroseconds(1);
    
      SPI.transfer(RDATA); //Issue RDATA
      delayMicroseconds(7);
    
      //This is the reading in the Data register from whatever the mux was set to the last
      //time this function was called. By default, it is configured to leave that value at 0
      adc_val1 |= SPI.transfer(NOP);
      //delayMicroseconds(10);
      adc_val1 <<= 8;
      adc_val1 |= SPI.transfer(NOP);
      //delayMicroseconds(10);
      adc_val1 <<= 8;
      adc_val1 |= SPI.transfer(NOP);
      //delayMicroseconds(5);
    
      //now wait for the next dataready
      waitforDRDY(); // Wait until DRDY is LOW
      //delayMicroseconds(5);
    
      //now change the mux register back to 0 so we left things how we found them
      SPI.transfer(WREG | MUX); // send 1st command byte, address of the register
      SPI.transfer(0x00);   // send 2nd command byte, write only one register
      SPI.transfer(MUX_RESET);     //pins registers 2 and 3
    
      //now we need to sync
      //need to delay by 4x SPI clock = 2.35 uS (t1)
      //to be safe 5 uS
      delayMicroseconds(2);
      SPI.transfer(SYNC);
    
      //again delay by t1
      delayMicroseconds(5);
      //send wakeup
      SPI.transfer(WAKEUP);
    
      //then delay one more time by t1 before rdata
      delayMicroseconds(1);
    
      SPI.transfer(RDATA); //Issue RDATA
      delayMicroseconds(7);
      //this should now be the value from the mux change we just did (0,1 to 2,3)
      adc_val2 |= SPI.transfer(NOP);
      //delayMicroseconds(10);
      adc_val2 <<= 8;
      adc_val2 |= SPI.transfer(NOP);
      //delayMicroseconds(10);
      adc_val2 <<= 8;
      adc_val2 |= SPI.transfer(NOP);
      //delayMicroseconds(5);
      //this is the value for the
    
      digitalWriteFast(21, HIGH);
      SPI.endTransaction();
    
      if (adc_val1 > 0x7fffff) { //if MSB == 1
        adc_val1 = adc_val1 - 16777216; //do 2's complement, keep the sign this time!
      }
    
      if (adc_val2 > 0x7fffff) { //if MSB == 1
        adc_val2 = adc_val2 - 16777216; //do 2's complement, keep the sign this time!
      }
    
      val1 = adc_val1;
    
      val2 = adc_val2;
    }
    
    void read_three_values() {
      //datasheet page 21 at the bottom gives the timing
      int32_t adc_val1;
      int32_t adc_val2;
      int32_t adc_val3;
    
      waitforDRDY(); // Wait until DRDY is LOW
      SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
      digitalWriteFast(21, LOW); //Pull SS Low to Enable Communications with ADS1247
      //delayMicroseconds(5); // RD: Wait 25ns for ADC12xx to get ready
    
      //now change the mux register
      SPI.transfer(WREG | MUX); // send 1st command byte, address of the register
      SPI.transfer(0x00);   // send 2nd command byte, write only one register
      SPI.transfer(0x23);     //pins registers 2 and 3
    
      //now we need to sync
      //need to delay by 4x SPI clock = 2.35 uS (t1)
      //to be safe 5 uS
      delayMicroseconds(2);
      SPI.transfer(SYNC);
    
      //again delay by t1
      delayMicroseconds(5);
      //send wakeup
      SPI.transfer(WAKEUP);
    
      //then delay one more time by t1 before rdata
      delayMicroseconds(1);
    
      SPI.transfer(RDATA); //Issue RDATA
      delayMicroseconds(7);
    
      //This is the reading in the Data register from whatever the mux was set to the last
      //time this function was called. By default, it is configured to leave that value at 0
      adc_val1 |= SPI.transfer(NOP);
      //delayMicroseconds(10);
      adc_val1 <<= 8;
      adc_val1 |= SPI.transfer(NOP);
      //delayMicroseconds(10);
      adc_val1 <<= 8;
      adc_val1 |= SPI.transfer(NOP);
      //delayMicroseconds(5);
    
      //now wait for the next dataready
      waitforDRDY(); // Wait until DRDY is LOW
      //delayMicroseconds(5);
    
      //now change the mux register
      SPI.transfer(WREG | MUX); // send 1st command byte, address of the register
      SPI.transfer(0x00);   // send 2nd command byte, write only one register
      SPI.transfer(0x45);     //pins registers 4 and 5
    
      //now we need to sync
      //need to delay by 4x SPI clock = 2.35 uS (t1)
      //to be safe 5 uS
      delayMicroseconds(2);
      SPI.transfer(SYNC);
    
      //again delay by t11
      delayMicroseconds(5);
      //send wakeup
      SPI.transfer(WAKEUP);
    
      //then delay one more time by t1 before rdata
      delayMicroseconds(1);
    
      SPI.transfer(RDATA); //Issue RDATA
      delayMicroseconds(7);
    
      //this is the reading from pins 2,3
      adc_val2 |= SPI.transfer(NOP);
      //delayMicroseconds(10);
      adc_val2 <<= 8;
      adc_val2 |= SPI.transfer(NOP);
      //delayMicroseconds(10);
      adc_val2 <<= 8;
      adc_val2 |= SPI.transfer(NOP);
      //delayMicroseconds(5);
    
      //now wait for the next dataready
      waitforDRDY(); // Wait until DRDY is LOW
      //delayMicroseconds(5);
    
      //now change the mux register back to 0 so we left things how we found them
      SPI.transfer(WREG | MUX); // send 1st command byte, address of the register
      SPI.transfer(0x00);   // send 2nd command byte, write only one register
      SPI.transfer(MUX_RESET);     //pins registers 2 and 3
    
      //now we need to sync
      //need to delay by 4x SPI clock = 2.35 uS (t1)
      //to be safe 5 uS
      delayMicroseconds(2);
      SPI.transfer(SYNC);
    
      //again delay by t1
      delayMicroseconds(5);
      //send wakeup
      SPI.transfer(WAKEUP);
    
      //then delay one more time by t1 before rdata
      delayMicroseconds(1);
    
      SPI.transfer(RDATA); //Issue RDATA
      delayMicroseconds(7);
      //this should now be the value from the mux change we just did (2,3 to 4,5)
      adc_val3 |= SPI.transfer(NOP);
      //delayMicroseconds(10);
      adc_val3 <<= 8;
      adc_val3 |= SPI.transfer(NOP);
      //delayMicroseconds(10);
      adc_val3 <<= 8;
      adc_val3 |= SPI.transfer(NOP);
     // delayMicroseconds(5);
      //this is the value for the
    
      digitalWriteFast(21, HIGH);
      SPI.endTransaction();
    
      if (adc_val1 > 0x7fffff) { //if MSB == 1
        adc_val1 = adc_val1 - 16777216; //do 2's complement, keep the sign this time!
      }
    
      if (adc_val2 > 0x7fffff) { //if MSB == 1
        adc_val2 = adc_val2 - 16777216; //do 2's complement, keep the sign this time!
      }
    
      if (adc_val3 > 0x7fffff) { //if MSB == 1
        adc_val3 = adc_val3 - 16777216; //do 2's complement, keep the sign this time!
      }
    
    
      val1 = adc_val1;
    
      val2 = adc_val2;
    
      val3 = adc_val3;
    }
    
    
    //library files
    
    volatile int DRDY_state = HIGH;
    
    void waitforDRDY() {
      while (DRDY_state) {
        continue;
      }
      noInterrupts();
      DRDY_state = HIGH;
      interrupts();
    }
    
    //Interrupt function
    void DRDY_Interuppt() {
      DRDY_state = LOW;
    }
    
    long GetRegisterValue(uint8_t regAdress) {
      uint8_t bufr;
      digitalWriteFast(21, LOW);
      delayMicroseconds(10);
      SPI.transfer(RREG | regAdress); // send 1st command byte, address of the register
      SPI.transfer(0x00);     // send 2nd command byte, read only one register
      delayMicroseconds(10);
      bufr = SPI.transfer(NOP); // read data of the register
      delayMicroseconds(10);
      digitalWriteFast(21, HIGH);
      //digitalWrite(_START, LOW);
      SPI.endTransaction();
      return bufr;
    
    }
    
    void SendCMD(uint8_t cmd) {
      waitforDRDY();
      SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with 4Mhz clock, MSB first, SPI Mode0
      digitalWriteFast(21, LOW);
      delayMicroseconds(10);
      SPI.transfer(cmd);
      delayMicroseconds(10);
      digitalWriteFast(21, HIGH);
      SPI.endTransaction();
    }
    
    void Reset() {
      SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with  clock, MSB first, SPI Mode1
      digitalWriteFast(21, LOW);
      delayMicroseconds(10);
      SPI.transfer(RESET); //Reset
      delay(2); //Minimum 0.6ms required for Reset to finish.
      SPI.transfer(SDATAC); //Issue SDATAC
      delayMicroseconds(100);
      digitalWriteFast(21, HIGH);
      SPI.endTransaction();
    }
    
    void SetRegisterValue(uint8_t regAdress, uint8_t regValue) {
    
      uint8_t regValuePre = GetRegisterValue(regAdress);
      if (regValue != regValuePre) {
        //digitalWrite(_START, HIGH);
        delayMicroseconds(10);
        waitforDRDY();
        SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1)); // initialize SPI with SPI_SPEED, MSB first, SPI Mode1
        digitalWriteFast(21, LOW);
        delayMicroseconds(10);
        SPI.transfer(WREG | regAdress); // send 1st command byte, address of the register
        SPI.transfer(0x00);   // send 2nd command byte, write only one register
        SPI.transfer(regValue);         // write data (1 Byte) for the register
        delayMicroseconds(10);
        digitalWriteFast(21, HIGH);
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
    private:
};
#endif
