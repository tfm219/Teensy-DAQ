
#ifndef AD7771_H_
#define AD7771_H_

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <SPI.h>
#include <Arduino.h>
#include "gpio.h"
#include "spi.h"

#define SPI_SPEED 4000000

//*****************************************************************************
//
// Register definitions
// Summary pp. 62-65
//
//*****************************************************************************


/* Register 0x00-0x07 (CHX_CONFIG) definition P. 66-69
 * ---------------------------------------------------------------------------------
 * |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
 * ---------------------------------------------------------------------------------
 * |       Gain        | REF_MON |   RX    |           RESERVED [3:0]              |
 * ---------------------------------------------------------------------------------
 */

    /** CH0_CONFIG-CH7_CONFIG register addresses */
    #define CHO_CONFIG                    (0x00)
    #define CH1_CONFIG                    (0x01)
    #define CH2_CONFIG                    (0x02)
    #define CH3_CONFIG                    (0x03)
    #define CH4_CONFIG                    (0x04)
    #define CH5_CONFIG                    (0x05)
    #define CH6_CONFIG                    (0x06)
    #define CH7_CONFIG                    (0x07)
    /** CONFIG0 default (reset) value */
    #define CH_CONFIG_DEFAULT             (0x00)

/* Register 0x08 (CH_DISABLE) definition P. 66
 * ---------------------------------------------------------------------------------
 * |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
 * ---------------------------------------------------------------------------------
 * |   CH7   |   CH6   |   CH5   |   CH4   |   CH3   |   CH2   |   CH1   |   CH0   |
 * ---------------------------------------------------------------------------------
 */

    /** CH0_CONFIG-CH7_CONFIG register addresses */
    #define CHO_DISABLE                   (0x01)
    #define CH1_DISABLE                   (0x02)
    #define CH2_DISABLE                   (0x04)
    #define CH3_DISABLE                   (0x08)
    #define CH4_DISABLE                   (0x10)
    #define CH5_DISABLE                   (0x20)
    #define CH6_DISABLE                   (0x40)
    #define CH7_DISABLE                   (0x80)
    /** CONFIG0 default (reset) value */
    #define CH_DISABLE_DEFAULT            (0x00)
  
/* Register 0x09-0x10 (CHX_SYNC_OFFSET) definition P. 66
 * ---------------------------------------------------------------------------------
 * |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |  Bit 2  |  Bit 1  |  Bit 0  |
 * ---------------------------------------------------------------------------------
 * |                            CHX_SYNC_OFFSET [7:0]                              |
 * ---------------------------------------------------------------------------------
 */

    /** CH0_CONFIG-CH7_CONFIG register addresses */
    #define CHO_SYNC_OFFSET               (0x00)
    #define CH1_SYNC_OFFSET               (0x01)
    #define CH2_SYNC_OFFSET               (0x02)
    #define CH3_SYNC_OFFSET               (0x03)
    #define CH4_SYNC_OFFSET               (0x04)
    #define CH5_SYNC_OFFSET               (0x05)
    #define CH6_SYNC_OFFSET               (0x06)
    #define CH7_SYNC_OFFSET               (0x07)
    /** CONFIG0 default (reset) value */
    #define SYNC_OFFSET_DEFAULT           (0x00)


/* Register 0x11 (CONFIG_1) definition P. 72
 * ----------------------------------------------------------------------------------
 * |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |   Bit 2  |  Bit 1  |  Bit 0  |
 * ----------------------------------------------------------------------------------
 * |   MCLK  | P_MODE  |   VCM   | REFOUT  |   SAR   |  RC_OSC  |  SOFT_RESET[1:0]  |
 * ----------------------------------------------------------------------------------
 */

    #define CONFIG_1                      (0x11)
    
    /** CONFIG0 default (reset) value */
    #define CONFIG_1_DEFAULT              (0x24)

/* Register 0x12 (CONFIG_2) definition P. 73
 * ----------------------------------------------------------------------------------
 * |  Bit 7  |  Bit 6  |  Bit 5  |  Bit 4  |  Bit 3  |   Bit 2  |  Bit 1  |  Bit 0  |
 * ----------------------------------------------------------------------------------
 * |    0    |  FILTER |SAR_DIAG |  SD0_DR_STR [4:3] |  DOUT_DR_STR [2:1] |SPI_SYNC |
 * ----------------------------------------------------------------------------------
 */

    #define CONFIG_2                      (0x11)
    
    /** CONFIG0 default (reset) value */
    #define CONFIG_2_DEFAULT              (0x09)

//*****************************************************************************
//
// Function Prototypes
//
//*****************************************************************************
class AD7771 {
  public: 
    AD7771(int ST, int RST, int CS, int DRDY, int PDWN);
    void    beginADS(void);
    int32_t readRegister(uint8_t address);
    void writeRegister(uint8_t address, uint8_t data);
    void startSPI();
    void endSPI();
    void resetADS();
    void toggleReset();
    void waitForDRDY();
    void DRDYLow();
    uint8_t getRegisterValue(uint8_t address);
    double convertToVolts(double value);
    int32_t readData();
    void addMultipleChannels(int chans[], int numChans);
    void addChannel(int chan);
    void removeMultipleChannels(int chans[], int numChans);
    void removeChannel(int chan);
    void addMultipleDiffChannels(int chans[], int numChans);
    void addDiffChannel(int chan);
    void removeMultipleDiffChannels(int chans[], int numChans);
    void removeDiffChannel(int chan);
    int32_t getChannelID(int32_t data);
    String getChannel(int32_t data);
    double getConversion(int32_t data);
    int32_t getConversionRaw(int32_t data);
    //int32_t readData(/*uint8_t status[], uint8_t data[],*/ readMode mode);
    /*uint8_t readSingleRegister(uint8_t address);
    void    readMultipleRegisters(uint8_t startAddress, uint8_t count);
    void    sendCommand(uint8_t op_code);
    void    startConversions(void);
    
    void    writeSingleRegister(uint8_t address, uint8_t data);
    void    writeMultipleRegisters(uint8_t startAddress, uint8_t count, const uint8_t regData[]);
    */
    
    // Internal variable getters
    
    // Internal variable setters
    //void    restoreRegisterDefaults(void);
    
  private:
    volatile int DRDY_state;
    int ST;
    
    int CS;
    int RST;
    int DRDY;
    int PDWN;
    int dataRate;
    double Gain;
    int vRef;
    double resolution;
    double clockMult;
    double bitToVolt;
    uint8_t autoChans0;
    uint8_t autoChans1;
    uint8_t autoChansDiff;
};