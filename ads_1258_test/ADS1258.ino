#include "ADS1258.h"


volatile int DRDY_state;// = HIGH;

void beginADS()
{
  DRDY_state = HIGH;
  SPI.begin();
  delay(50);
  digitalWrite(PDWN, HIGH);
  delay(10);//delayMicroseconds(125);
  
  digitalWriteFast(ST, HIGH);
  delay(10);//5
  toggleReset();
  delay(10);

  resetADS();

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
}




int32_t readRegister(uint8_t address)
{
  startSPI();
  SPI.transfer(RREG | (address & A_MASK));//Tx); // send 1st command byte, address of the register
  //delayMicroseconds(80);//138);//160);
  uint32_t ret = SPI.transfer(0x00);
  endSPI();
  return ret;
} 


/*uint8_t sendReceiveByte(uint8_t Tx)
{
    uint8_t Rx;
    SPI.transfer(Tx);
    return SPI.transfer(0x00);
}*/

void toggleReset()
{
  digitalWriteFast(RST, LOW);
  delayMicroseconds(125);
  digitalWriteFast(RST, HIGH);
}


void writeSingleRegister(uint8_t address, uint8_t data)
{
    /* Check that the register address is in range */
    assert(address < NUM_REGISTERS);

    /* Initialize arrays */
    uint8_t DataTx[2];
    uint8_t DataRx[2] = { 0 };

    /* Build TX array and send it */
    DataTx[0] = ( WREG | (address & A_MASK) );
    DataTx[1] = data;
    uint8_t sendReceiveArrays(DataTx, DataRx, 2);
}

void resetADS()
{
  digitalWriteFast(CS, HIGH);
  startSPI();
  SPI.transfer(RESET); //Reset
  endSPI();
}

/*uint8_t readRegister(uint8_t address);
    setCS(LOW);

    // Remove any residual or old data from the receive FIFO
    uint32_t junk;
    while (SSIDataGetNonBlocking(SSI3_BASE, &junk));

    // SSI TX & RX
    uint8_t i;
    for (i = 0; i < byteLength; i++)
    {
        DataRx[i] = spiSendReceiveByte(DataTx[i]);
    }

    // Set the nCS pin HIGH
    setCS(HIGH);
}*/

uint8_t sendReceiveArrays(uint8_t Tx[], uint8_t Rx[], uint8_t byteLength)
{

    /* Set the nCS pin LOW */
    startSPI();
    /* Remove any residual or old data from the receive FIFO */
    //uint32_t junk;
    //while (SSIDataGetNonBlocking(SSI3_BASE, &junk));
    
    /* SSI TX & RX */
    uint8_t i;
    for (i = 0; i < byteLength; i++)
    {
        Rx[i] = sendReceiveByte(Tx[i]);
    }

    /* Set the nCS pin HIGH */
    endSPI();
    return Rx[1];
}


/**
 * \fn uint8_t spiSendReceiveByte(uint8_t dataTx)
 * \brief Sends SPI command to ADC and returns a response, one byte at a time.
 * \param dataTx data to send over SPI
 * \return Returns SPI response byte
 */
uint8_t sendReceiveByte(uint8_t Tx)
{
    /* SSI TX & RX */
    //uint8_t Rx;
    SPI.transfer(Tx);
    return SPI.transfer(0x00);
    
    //return Rx;
}

void startSPI()
{
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE3)); // initialize SPI with  clock, MSB first, SPI Mode1
  digitalWriteFast(CS, LOW);
}

void endSPI()
{
  digitalWriteFast(CS, HIGH);
  SPI.endTransaction();
}

void DRDY_Interrupt() {
    DRDY_state = LOW;
}

void waitForDRDY() {
    while (DRDY_state) 
    {
      continue;
    }//while (DRDY_state);
    noInterrupts();
    DRDY_state = HIGH;
    interrupts();
}

/*int8_t readSingleRegister(uint8_t address)
{
    // Check that the register address is in range 
    assert(address < NUM_REGISTERS);

    // Initialize arrays
    uint8_t DataTx[2] = { 0 };
    uint8_t DataRx[2] = { 0 };

    // Build TX array and send it 
    DataTx[0] = RREG | (address & A_MASK);
    DataRx[1] = sendReceiveArrays(DataTx, DataRx, 2);

    // Update register array and return read result
    
    return DataRx[1];
}*/
