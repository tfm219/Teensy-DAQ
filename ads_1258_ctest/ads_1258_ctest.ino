#include <SPI.h>
#include "ADS1258.h"

/** Pin Setup
 * ST (J6.1), 3
 * RST (J6.6), 8 NOT PDWN (J6.2), 8
 * CS (J6.7), 21
 * DRDY (J6.15), 22
 * DOUT (J6.13), 12
 * DIN (J6.11), 11
 * SCLK(J6.3), 13
 * PDWN (J6.2), 23
 * DGND(J6.4)
 * CLKSEL(J6.8) GND (Use Internal Clock)
 * 
 * +5v (J5.3)
 * GND(J5.5, J5.6)
 * 3.3v (j5.9
 */
 
#define ST     3 //ADS1258 start pin (CNTL, J6.1)
#define RST    8 //ADS1256 reset pin
#define CS    21 //ADS1256 chip select
#define DRDY  22 //ADS1256 data ready
#define PDWN  23 //PDWN pin

//this needs to match the setting in the ADC init function in the library tab


//we'll calculate this in setup
int channels[] = {0, 1, 2};
ADS1258 adc = ADS1258(ST, RST, CS, DRDY, PDWN);

volatile int DRDY_state = HIGH;
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("booting C++");
  //pinMode(DRDY,INPUT);
  
  attachInterrupt(DRDY, DRDY_Interrupt, FALLING);
  adc.addMultipleChannels(channels, 3);
  //adc.removeChannel(1);
  //adc.addMultipleDiffChannels(channels, 3);
  adc.beginADS();
  delay(2000);
}
byte stat,data1,data2,data3;
int32_t data;
int32_t conversion;
void loop() {
  
  data = adc.readData();
  Serial.print("Channel: ");
  Serial.println(adc.getChannel(data));
  Serial.print("Conversion: ");
  Serial.println(adc.getConversion(data), 7);
  Serial.println();
  //delay(2000);
}

void DRDY_Interrupt() {
    //noInterrupts();
    adc.DRDYLow();
    //DRDY_state = LOW;
    //interrupts();
}
