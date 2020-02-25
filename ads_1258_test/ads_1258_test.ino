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

#define SPI_SPEED 4000000 //2500000
#define ST     3 //ADS1258 start pin (CNTL, J6.1)
#define RST    8 //ADS1256 reset pin
#define CS    21 //ADS1256 chip select
#define DRDY  22 //ADS1256 data ready
#define PDWN  23 //PDWN pin


double resolution = pow(2, 24);// - 1;// 2^23-1 //8388608.;

//this needs to match the setting in the ADC init function in the library tab
double Gain = 1.; //be sure to have a period 

double vRef = 5.0; //reference voltage

//we'll calculate this in setup
double bitToVolt = (resolution*Gain)/(2 * vRef);;

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("booting INO");
  //pinMode(DRDY,INPUT);
  pinMode(CS,OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode (ST, OUTPUT);
  digitalWrite(ST, HIGH); 
  attachInterrupt(DRDY, DRDY_Interrupt, FALLING);
  
  beginADS(); 
}

byte stat,data1,data2,data3;
uint32_t data;
void loop() {
  //Serial.println("LOOP");
  /*if (!digitalRead(DRDY))
  {
    //Serial.println("READY");
    startSPI();
    SPI.transfer(READ_COMMAND);
    stat = SPI.transfer(0x00);
    data = SPI.transfer(0x00);
    data <<= 8;
    data |= SPI.transfer(0x00);
    data <<= 8;
    data |= SPI.transfer(0x00);
    
    Serial.println(data  / bitToVolt, 6);
    endSPI();
  }*/
}

/*void DRDY_Interrupt() {
    DRDY_state = LOW;
}*/
