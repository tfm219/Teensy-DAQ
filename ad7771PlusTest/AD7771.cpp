#include "AD7771.h" 

AD7771::AD7771(int ST, int RST, int CS, int DRDY, int PDWN);
{
    this->ST = ST;
    this->RST = RST;
    this->CS = CS;
    this->DRDY = DRDY;
    this->PDWN = PDWN;
    this->resolution =  pow(2.0, 23.0) - 1;
    this->vRef = 3.3;
    this->Gain = this->resolution / 0x780000;//1.0666665;//7864320;
    this->bitToVolt = (resolution*Gain)/(vRef);
    //this->bitToVolt = (786432 / Gain);// / (vRef);
    this->autoChans0 = 0x00;
    this->autoChans1 = 0x00;
    this->autoChansDiff = 0x00;
}

void AD7771::init()
{
    startSPI();
    this->ctrl_mode = ctrl_mode;
    this->spi_crc_en = CH_DISABLE_DEFAULT;
    this->spi_op_mode = INT_REG;
    this->sar_state = CH_DISABLE_DEFAULT;
    this->sar_mux =     AUXAINP_AUXAINN;

    uint8_t reg_data;
    for (uint8_t reg_addr = 0; reg addr <= REG_SRC_UPDATE; ++reg_addr)
    {
        SPI.write(ERR_REG_1_EN, this->cached_reg_val)
        this->cached_reg_val[reg_addr] = reg_data;
    }
  
    if ((this->ctrl_mode == SPI_CTRL_ && (spi_crc_en == ENABLE))
    {
        SPI.read(ERR_REG_1_EN, cached_reg_val);
        SPI.write(ERR_REG_1_EN, SPI_CRC_TEST_EN);
        spi_crc_en = ENABLE;
    }
    
}

void AD7771::startSPI()
{
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE3)); // initialize SPI with  clock, MSB first, SPI Mode1
  digitalWriteFast(CS, LOW);
}

void AD7771::endSPI()
{
  digitalWriteFast(CS, HIGH);
  SPI.endTransaction();
}

double AD7771::getConversion(int32_t data)
{
  return convertToVolts(data & 0x00FFFFFF); 
}

int32_t AD7771::getConversionRaw(int32_t data)
{
  return (data & 0x00FFFFFF); 
}

double AD7771::convertToVolts(double value) 
{
  return (value / bitToVolt);
}
