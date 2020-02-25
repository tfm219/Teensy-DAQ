#include "ADS1258.h"

//static uint8_t registerMap[NUM_REGISTERS];

/**DO FRIDAY
 * Get Channel from data function
 * get conversion from data function
 * 
 */

ADS1258::ADS1258(int ST, int RST, int CS, int DRDY, int PDWN)
{
    this->ST = ST;
    this->RST = RST;
    this->CS = CS;
    this->DRDY = DRDY;
    this->PDWN = PDWN;
    this->resolution =  pow(2.0, 23.0) - 1;
    this->vRef = 4.096;//5.0;//3.5854034;//5.0; //786432
    this->Gain = this->resolution / 0x780000;//1.0666665;//7864320;
    this->bitToVolt = (resolution*Gain)/(vRef);
    //this->bitToVolt = (786432 / Gain);// / (vRef);
    this->autoChans0 = 0x00;
    this->autoChans1 = 0x00;
    this->autoChansDiff = 0x00;
}


/**
 * \fn void adcStartupRoutine(void)
 * \brief Startup function to be called before communicating with the ADC
 */
void ADS1258::beginADS(void)
{
  DRDY_state = HIGH;
  pinMode (CS,OUTPUT);
  pinMode (RST, OUTPUT);
  pinMode (ST, OUTPUT);
  
  SPI.begin();
  
  delay(50);
  digitalWriteFast(PDWN, HIGH);
  delay(10);//delayMicroseconds(125);
  
  digitalWriteFast(ST, HIGH);
  delay(10);
  toggleReset();
  delay(10);

  resetADS();

  writeRegister(MUXSG0, autoChans0);
  writeRegister(MUXSG1, autoChans1);
  writeRegister(MUXDIF, autoChansDiff);
  //writeRegister(SYSRED, SYSRED_VCC_ENABLE | SYSRED_REF_ENABLE | SYSRED_GAIN_ENABLE);//SYSRED_REF_ENABLE | SYSRED_GAIN_ENABLE | SYSRED_VCC_ENABLE);
 
  //waitForDRDY();
  //Serial.println("Read Reg");
  Serial.print("CONFIG0(0x");
  Serial.print(CONFIG0_DEFAULT, HEX);
  Serial.print("): 0x");
  Serial.println(readRegister(CONFIG0), HEX);
  
  Serial.print("CONFIG1(0x");
  Serial.print(CONFIG1_DEFAULT, HEX);
  Serial.print("): 0x");
  Serial.println(readRegister(CONFIG1), HEX);
  
  Serial.print("MUXSCH(0x");
  Serial.print(MUXSCH_DEFAULT, HEX);
  Serial.print("): 0x");
  Serial.println(readRegister(MUXSCH), HEX);
  
  Serial.print("MUXDIF(0x");
  Serial.print(MUXDIF_DEFAULT, HEX);
  Serial.print("): 0x");
  Serial.println(readRegister(MUXDIF), HEX);
  
  Serial.print("MUXSG0(0x");
  Serial.print(MUXSG0_DEFAULT, HEX);
  Serial.print("): 0x");
  Serial.println(readRegister(MUXSG0), HEX);
  
  Serial.print("MUXSG1(0x");
  Serial.print(MUXSG1_DEFAULT, HEX);
  Serial.print("): 0x");
  Serial.println(readRegister(MUXSG1), HEX);
  
  Serial.print("SYSRED(0x");
  Serial.print(SYSRED_DEFAULT, HEX);
  Serial.print("): 0x");
  Serial.println(readRegister(SYSRED), HEX);
  
  Serial.print("GPIOC(0x");
  Serial.print(GPIOC_DEFAULT, HEX);
  Serial.print("): 0x");
  Serial.println(readRegister(GPIOC), HEX);
  
  Serial.print("GPIOD(0x");
  Serial.print(GPIOD_DEFAULT, HEX);
  Serial.print("): 0x");
  Serial.println(readRegister(GPIOD), HEX);
  
  Serial.print("ID(0x");
  Serial.print(ID_DEFAULT, HEX);
  Serial.print("): 0x");
  Serial.println(readRegister(ID), HEX);
  Serial.println();
}

int32_t ADS1258::readData()
{
  //uint8_t Rx[4];
  int32_t statusByte = 0;
  int32_t upperByte;
  int32_t middleByte;
  int32_t lowerByte;
  int32_t conversion;// = 0;
  waitForDRDY();
  startSPI();
  SPI.transfer(READ_COMMAND);
  statusByte = SPI.transfer(0x00);
  /*conversion |= SPI.transfer(0x00);
  conversion <<= 8;
  conversion |= SPI.transfer(0x00);
  conversion <<= 8;
  conversion |= SPI.transfer(0x00);*/
  upperByte = SPI.transfer(0x00);
  middleByte = SPI.transfer(0x00);
  lowerByte = SPI.transfer(0x00);

  

  endSPI();

  upperByte = (upperByte & 0xFF) << 16;
  middleByte = (middleByte & 0xFF) << 8;
  lowerByte = (lowerByte & 0xFF) << 0;
  conversion =  upperByte | middleByte | lowerByte;

  
  if (conversion & 0x800000) {//0x7fffff) { //if MSB == 1
      conversion = conversion - 16777216; //do 2's complement, keep the sign this time!
  }
  
  /*Serial.print("Status: ");
  Serial.println(statusByte, BIN);
  Serial.print("Conversion: ");
  Serial.print(conversion);
  Serial.print(" | ");
  Serial.println(convertToVolts(conversion));*/
  statusByte <<= 24;
  return (statusByte | conversion);
}

void ADS1258::writeRegister(uint8_t address, uint8_t data)
{
  startSPI();
  SPI.transfer(WREG | (address & A_MASK));
  SPI.transfer(data);
  endSPI();
} 

int32_t ADS1258::readRegister(uint8_t address)
{
  startSPI();
  SPI.transfer(RREG | (address & A_MASK));
  uint32_t ret = SPI.transfer(0x00);
  endSPI();
  return ret;
} 

void ADS1258::DRDYLow()
{
  this->DRDY_state = LOW;
}

void ADS1258::waitForDRDY()
{
    while (DRDY_state) 
    {
      continue;
    }//while (DRDY_state);
    noInterrupts();
    DRDY_state = HIGH;
    interrupts();
}

void ADS1258::toggleReset()
{
  digitalWriteFast(RST, LOW);
  delayMicroseconds(125);
  digitalWriteFast(RST, HIGH);
}

void ADS1258::resetADS(void)
{
    digitalWriteFast(CS, HIGH);
    startSPI();
    SPI.transfer(RESET); //Reset
    endSPI();
}

void ADS1258::startSPI()
{
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE3)); // initialize SPI with  clock, MSB first, SPI Mode1
  digitalWriteFast(CS, LOW);
}

void ADS1258::endSPI()
{
  digitalWriteFast(CS, HIGH);
  SPI.endTransaction();
}

int32_t ADS1258::getChannelID(int32_t data)
{
  return ((data & 0x1F000000) >> 24);
}

double ADS1258::getConversion(int32_t data)
{
  return convertToVolts(data & 0x00FFFFFF); 
}

int32_t ADS1258::getConversionRaw(int32_t data)
{
  return (data & 0x00FFFFFF); 
}

double ADS1258::convertToVolts(double value) 
{
  return (value / bitToVolt);
}

void ADS1258::addMultipleChannels(int chans[], int numChans)
{
  for(size_t i = 0; i < 3; ++i)//sizeof(chans)/sizeof(chans[0]); ++i)
  {
    addChannel(chans[i]);    
  }
}

void ADS1258::addChannel(int chan)
{

  switch (chan) {
    case 0:
      autoChans0 |= MUXSG0_AIN0_ENABLE;
      break;
    case 1:
      autoChans0 |= MUXSG0_AIN1_ENABLE;
      break;
    case 2:
      autoChans0 |= MUXSG0_AIN2_ENABLE;
      break;
    case 3:
      autoChans0 |= MUXSG0_AIN3_ENABLE;
      break;
    case 4:
      autoChans0 |= MUXSG0_AIN4_ENABLE;
      break;
    case 5:
      autoChans0 |= MUXSG0_AIN5_ENABLE;
      break;
    case 6:
      autoChans0 |= MUXSG0_AIN6_ENABLE;
      break;
    case 7:
      autoChans1 |= MUXSG0_AIN7_ENABLE;
      break;
    case 8:
      autoChans1 |= MUXSG1_AIN8_ENABLE;
      break;
    case 9:
      autoChans1 |= MUXSG1_AIN9_ENABLE;
      break;
    case 10:
      autoChans1 |= MUXSG1_AIN10_ENABLE;
      break;
    case 11:
      autoChans1 |= MUXSG1_AIN11_ENABLE;
      break;
    case 12:
      autoChans1 |= MUXSG1_AIN12_ENABLE;
      break;
    case 13:
      autoChans1 |= MUXSG1_AIN13_ENABLE;
      break;
    case 14:
      autoChans1 |= MUXSG1_AIN14_ENABLE;
      break;
    case 15:
      autoChans1 |= MUXSG1_AIN15_ENABLE;
      break;
  }
}

void ADS1258::removeMultipleChannels(int chans[], int numChans)
{
  for(size_t i = 0; i < 3; ++i)//sizeof(chans)/sizeof(chans[0]); ++i)
  {
    removeChannel(chans[i]);    
  }
}
//DOESNT WORK
void ADS1258::removeChannel(int chan)
{
  switch (chan) {
    case 0:
      autoChans0 &= ~MUXSG0_AIN0_ENABLE;
      break;
    case 1:
      autoChans0 &= ~MUXSG0_AIN1_ENABLE;
      break;
    case 2:
      autoChans0 &= ~MUXSG0_AIN2_ENABLE;
      break;
    case 3:
      autoChans0 &= ~MUXSG0_AIN3_ENABLE;
      break;
    case 4:
      autoChans0 &= ~MUXSG0_AIN4_ENABLE;
      break;
    case 5:
      autoChans0 &= ~MUXSG0_AIN5_ENABLE;
      break;
    case 6:
      autoChans0 &= ~MUXSG0_AIN6_ENABLE;
      break;
    case 7:
      autoChans1 &= ~MUXSG0_AIN7_ENABLE;
      break;
    case 8:
      autoChans1 &= ~MUXSG1_AIN8_ENABLE;
      break;
    case 9:
      autoChans1 &= ~MUXSG1_AIN9_ENABLE;
      break;
    case 10:
      autoChans1 &= ~MUXSG1_AIN10_ENABLE;
      break;
    case 11:
      autoChans1 &= ~MUXSG1_AIN11_ENABLE;
      break;
    case 12:
      autoChans1 &= ~MUXSG1_AIN12_ENABLE;
      break;
    case 13:
      autoChans1 &= ~MUXSG1_AIN13_ENABLE;
      break;
    case 14:
      autoChans1 &= ~MUXSG1_AIN14_ENABLE;
      break;
    case 15:
      autoChans1 &= !MUXSG1_AIN15_ENABLE;
      break;
  }
}

void ADS1258::addMultipleDiffChannels(int chans[], int numChans)
{
  for(size_t i = 0; i < 3; ++i)//sizeof(chans)/sizeof(chans[0]); ++i)
  {
    addDiffChannel(chans[i]);    
  }
}

void ADS1258::addDiffChannel(int chan)
{
  switch (chan) {
    case 0:
      autoChansDiff |= DIFF0_ENABLE;
      break;
    case 1:
      autoChansDiff |= DIFF1_ENABLE;
      break;
    case 2:
      autoChansDiff |= DIFF2_ENABLE;
      break;
    case 3:
      autoChansDiff |= DIFF3_ENABLE;
      break;
    case 4:
      autoChansDiff |= DIFF4_ENABLE;
      break;
    case 5:
      autoChansDiff |= DIFF5_ENABLE;
      break;
    case 6:
      autoChansDiff |= DIFF6_ENABLE;
      break;
    case 7:
      autoChansDiff |= DIFF7_ENABLE;
      break;
  }
}

void ADS1258::removeMultipleDiffChannels(int chans[], int numChans)
{
  for(size_t i = 0; i < 3; ++i)//sizeof(chans)/sizeof(chans[0]); ++i)
  {
    removeDiffChannel(chans[i]);    
  }
}

void ADS1258::removeDiffChannel(int chan)
{
  switch (chan) {
    case 0:
      autoChansDiff &= ~DIFF0_ENABLE;
      break;
    case 1:
      autoChansDiff &= ~DIFF1_ENABLE;
      break;
    case 2:
      autoChansDiff &= ~DIFF2_ENABLE;
      break;
    case 3:
      autoChansDiff &= ~DIFF3_ENABLE;
      break;
    case 4:
      autoChansDiff &= ~DIFF4_ENABLE;
      break;
    case 5:
      autoChansDiff &= ~DIFF5_ENABLE;
      break;
    case 6:
      autoChansDiff &= ~DIFF6_ENABLE;
      break;
    case 7:
      autoChansDiff &= ~DIFF7_ENABLE;
      break;
  }
}

String ADS1258::getChannel(int32_t data)
{
  data = ((data & 0x1F000000) >> 24);
  switch (data) {
    case 0:
      return "DIFF0";
      break;
    case 1:
      return "DIFF1";
      break;
    case 2:
      return "DIFF2";
      break;
    case 3:
      return "DIFF3";
      break;
    case 4:
      return "DIFF4";
      break;
    case 5:
      return "DIFF5";
      break;
    case 6:
      return "DIFF6";
      break;
    case 7:
      return "DIFF7";
      break;
    case 8:
      return "AIN0";
      break;
    case 9:
      return "AIN1";
      break;
    case 10:
      return "AIN2";
      break;
    case 11:
      return "AIN3";
      break;
    case 12:
      return "AIN4";
      break;
    case 13:
      return "AIN5";
      break;
    case 14:
      return "AIN6";
      break;
    case 15:
      return "AIN7";
      break;
    case 16:
      return "AIN8";
      break;
    case 17:
      return "AIN9";
      break;
    case 18:
      return "AIN10";
      break;
    case 19:
      return "AIN11";
      break;
    case 20:
      return "AIN12";
      break;
    case 21:
      return "AIN13";
      break;
    case 22:
      return "AIN14";
      break;
    case 23:
      return "AIN15";
      break;
    case 24:
      return "OFFSET";
      break;
    case 26:
      return "VCC";
      break;
    case 27:
      return "TEMP";
      break;
    case 28:
      return "GAIN";
      break;
    case 29:
      return "REF";
      break;
  }
  return "";
}
/*uint8_t ADS1258::getRegisterValue(uint8_t address)
{
    assert(address < NUM_REGISTERS);
    return registerMap[address];
}*/

/* Retrieves the ADC conversion result 
 * NOTE: Call this function after /DRDY goes low and specify the 
 * the number of bytes to read and the starting position of data
 * (based on whe)
 *
 * This function implements the read command
*/

/**
 * \fn int32_t readData(uint8_t status[], uint8_t data[], readMode mode)
 * \brief Sends the read command and retrieves STATUS (if enabled) and data
 * \param status[] pointer to address where STATUS byte will be stored
 * \param data[] pointer to starting address where data bytes will be stored
 * \param mode readMode typedef to select which read operation to use
 * \return 32-bit sign-extended conversion result (data only)
 */
/*int32_t ADS1258::readData(uint8_t status[], uint8_t data[], readMode mode)
{
  waitForDRDY();
    uint8_t DataTx[5] = { 0 };    // Initialize all array elements to 0
    uint8_t DataRx[5] = { 0 };    // Relies on C99 [$6.7.8/10], [$6.7.8/21]
    uint8_t byteLength;
    uint8_t dataPosition;
    bool    status_byte_enabled = false;

    // Build TX array and send it
    if (mode == DIRECT)
    {
        DataTx[0] = READ_DIRECT;

        // STATUS byte enable depends on MUXMOD and STAT (see table 17 in datasheet)
        status_byte_enabled = IS_STAT_SET & !IS_MUXMOD_SET;
        byteLength = (status_byte_enabled ? 4 : 3);
        dataPosition = (status_byte_enabled ? 1 : 0);
    }
    else if (mode == COMMAND)
    {
        DataTx[0] = READ_COMMAND | MUL_MASK;

        //- STATUS byte always enabled, but undefined in fixed-channel mode (see table 17 in datasheet)
        status_byte_enabled = true;
        byteLength = 5;
        dataPosition = 2;
    }
    //spiSendReceiveArrays(DataTx, DataRx, byteLength);
    startSPI();
    SPI.transfer(DataTx);//[i]);
    //delayMicroseconds(10);
    
    for (int i = 0; i < byteLength; i++)
    {
        DataRx[i] = SPI.transfer(0x00);//DataTx[i]);
        Serial.println(DataRx[i]);
    }
    endSPI();

    //
    // Parse returned SPI data
    //

    // Check if STATUS byte is enabled and if we have a valid "status" memory pointer 
     //if (status_byte_enabled)// && status)
    {
        status[0] = DataRx[dataPosition - 1];
    }*/

    // Return the 32-bit sign-extended conversion result 
/*    int32_t signByte;
    if (DataRx[dataPosition] & 0x80u)   { signByte = 0xFF000000; }
    else                                { signByte = 0x00000000; }

    int32_t upperByte   = ((int32_t) DataRx[dataPosition + 0] & 0xFF) << 16;
    int32_t middleByte  = ((int32_t) DataRx[dataPosition + 1] & 0xFF) << 8;
    int32_t lowerByte = ((int32_t) DataRx[dataPosition + 2] & 0xFF) << 0;
  
    return (signByte | upperByte | middleByte | lowerByte);
}*/


/**
 * \fn void restoreRegisterDefaults(void)
 * \brief Updates the registerMap[] array to its default values.
 *
 * NOTES:
 * - If the MCU keeps a copy of the ADC register settings in memory,
 * then it is important to ensure that these values remain in sync with the
 * actual hardware settings. In order to help facilitate this, this function
 * should be called after powering up or resetting the device (either by
 * hardware pin control or SPI software command).
 *
 * - Reading back all of the registers after resetting the device will
 * accomplish the same result.
 */
/*void ADS1258::restoreRegisterDefaults(void)
{
    registerMap[REG_ADDR_CONFIG0]   =   CONFIG0_DEFAULT;
    registerMap[REG_ADDR_CONFIG1]   =   CONFIG1_DEFAULT;
    registerMap[REG_ADDR_MUXSCH]    =   MUXSCH_DEFAULT;
    registerMap[REG_ADDR_MUXDIF]    =   DIF_DEFAULT;
    registerMap[REG_ADDR_MUXSG0]    =   MUXSG0_DEFAULT;
    registerMap[REG_ADDR_MUXSG1]    =   MUXSG1_DEFAULT;
    registerMap[REG_ADDR_SYSRED]    =   SYSRED_DEFAULT;
    registerMap[REG_ADDR_GPIOC]     =   GPIOC_DEFAULT;
    registerMap[REG_ADDR_GPIOD]     =   GPIOD_DEFAULT;
    registerMap[REG_ADDR_ID]        =   0x00;               // Value of 0x00 indicates that we have not yet read the ID register
}*/

/**
 * \fn uint8_t readSingleRegister(uint8_t addr)
 * \brief Reads contents of a single register at the specified address
 * \param addr address of the register to read
 * \return 8-bit register read result
 */
/*uint8_t ADS1258::readSingleRegister(uint8_t address)
{
    / Check that the register address is in range
    assert(address < NUM_REGISTERS);
    
    // Initialize arrays
    uint8_t DataTx[2] = { 0 };
    uint8_t DataRx[2] = { 0 };
    
    // Build TX array and send it 
    DataTx[0] = RREG | (address & A_MASK);
    startSPI();
    //spiSendReceiveArrays(DataTx, DataRx, 2);
    
    for (int i = 0; i < 2; i++)
    {
        DataRx[i] = SPI.transfer(DataTx[i]);
    }
    // Update register array and return read result
    registerMap[address] = DataRx[1];
    return DataRx[1];
}*/



/**
 * \fn void readMultipleRegisters(uint8_t startAddress, uint8_t count)
 * \brief Reads a group of registers starting at the specified address
 * \param startAddress register address from which we start reading
 * \param count number of registers we want to read
 * NOTE: Use getRegisterValue() to retrieve the read values
 */
/*void ADS1258::readMultipleRegisters(uint8_t startAddress, uint8_t count)
{
    // Check that the register address and count are in range
    assert(startAddress + count <= NUM_REGISTERS);


    //
    // SPI communication
    //

    //setCS(LOW);
    startSPI();
    
    uint8_t dataTx = OPCODE_RREG | OPCODE_MUL_MASK | (startAddress & OPCODE_A_MASK);
    //spiSendReceiveByte(dataTx);
    SPI.transfer(dataTx);
    
    uint8_t i;
    for (i = startAddress; i < startAddress + count; i++)
    {
        // Read register data bytes
        registerMap[i] = SPI.transfer(0x00);//spiSendReceiveByte(0x00);
    }
    endSPI();
    //setCS(HIGH);
}*/



/**
 * \fn void writeSingleRegister(uint8_t address, uint8_t data)
 * \brief Write data to a single register at the specified address
 * \param address The address of the register to write
 * \param data The 8-bit data to write to the register
 */
/*void ADS1258::writeSingleRegister(uint8_t address, uint8_t data)
{
    // Check that the register address is in range
    assert(address < NUM_REGISTERS);

    // Initialize arrays
    uint8_t DataTx[2];
    uint8_t DataRx[2] = { 0 };

    // Build TX array and send it
    DataTx[0] = ( WREG | (address & A_MASK) );
    DataTx[1] = data;
    startSPI();
    SPI.transfer(DataTx);
    //spiSendReceiveArrays(DataTx, DataRx, 2);
    endSPI();
    // Update register array
    registerMap[address] = DataTx[1];
}*/


/**
 * \fn void sendCommand(uint8_t op_code)
 * \brief Sends the specified SPI command to the ADC
 * \param op_code SPI command byte
 */
/*void ADS1258::sendCommand(uint8_t op_code)
{
    // Assert if this function is used to send any of the following commands
    assert(RREG         != op_code);    // Use "readSingleRegister()"  or "readMultipleRegisters()"
    assert(WREG         != op_code);    // Use "writeSingleRegister()" or "writeMultipleRegisters()"
    assert(READ_DIRECT  != op_code);    // Use "readData()"
    assert(READ_COMMAND != op_code);    // Use "readData()"

    // SPI communication
    startSPI();
    SPI.transfer(op_code);
    endSPI();

    // Check for RESET command
    if (RESET == op_code)
    {
        // Update register array to keep software in sync with device
        restoreRegisterDefaults();
    }
}*/
