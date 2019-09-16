//#include <SPI.h>
#include "ADS1256.h"

#define ADS_RST_PIN    8 //ADS1256 reset pin
#define ADS_CS_PIN    21 //ADS1256 chip select
#define ADS_RDY_PIN   22 //ADS1256 data ready
/* 
    DIN  - pin 11 (MOSI)
    DOUT - pin 12 (MISO)
    CLK  - pin 13 (SCK)
    
    #define ADS_RST_PIN_2    4 //ADS1256 reset pin
    #define ADS_CS_PIN_2    20 //ADS1256 chip select
    #define ADS_RDY_PIN_2   19 //ADS1256 data ready
*/
double Gain = 1; //be sure to have a period 
ADS1256 adc = ADS1256(ADS_RST_PIN, ADS_RDY_PIN, ADS_CS_PIN, Gain);

void setup() {
  delay(1000);
  Serial.begin(115200); //250000 has 0% error (pjrc.com/teensy/td_uart.html)(115200 has .02 with teensy 3.5, -0.02 with 3.2)
  Serial.println("booting");
  
  //attachInterrupt(ADS_RDY_PIN, DRDY_Interrupt, FALLING);
  //attachInterrupt(adc2.getRDYPin(), DRDY_Interrupt, FALLING);
  
  adc.begin();
  //adc2.begin();
  
}

int32_t val1;
int32_t val2;
int32_t val3;
int32_t val4;

void loop() {
  //adc.delayClocks(125);
  //val1 = adc.readSingle_2();//Single_2();
  //val2 = adc.readSingle_0();//Single_0();
  val2 = adc.readDiff_0_1();
  
  //Serial.print((adc.convertToVolts(val1)), 6);
  //Serial.print(" | ");
  Serial.println((adc.convertToVolts(val2)), 6);
  
  
}

//void DRDY_Interrupt() {
  ///Serial.println("INTERRUPT");
    //adc.setDRDYState(LOW);
//}
/*void DRDY_Interupt() {
    adc2.setDRDYState(LOW);
}*/
