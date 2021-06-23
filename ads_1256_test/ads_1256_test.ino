//#include <SPI.h>
#include "ADS1256.h"
//#include <ads_1256_stuff.ino>

//(other stuff for getting the ADS1526 to work is in the next tab
#define ADS_RST_PIN    8 //ADS1256 reset pin
#define ADS_CS_PIN    21 //ADS1256 chip select
#define ADS_RDY_PIN   22 //ADS1256 data ready                                                   //ADS1256 chip select
#define ADS_Gain
#define Acquisition
/* 
    DIN  - pin 11 (MOSI)
    DOUT - pin 12 (MISO)
    CLK  - pin 13 (SCK)
*/


//put the ADC constants here
<<<<<<< HEAD

double resolution = pow(2, 24) - 1;// 2^23-1 //8388608.;

=======
double resolution = pow(2, 24);// - 1;// 2^23-1 //8388608.;
>>>>>>> 90dbd9f8736c24492330471de2f9fdb7a68108eb
//this needs to match the setting in the ADC init function in the library tab
double Gain = 1.; //be sure to have a period 
double vRef = 5.0; //reference voltage
//we'll calculate this in setup
double bitToVolt = 0.;

void setup() {
<<<<<<< HEAD
  byte ADS_gain = 1;
=======
>>>>>>> 90dbd9f8736c24492330471de2f9fdb7a68108eb
  delay(1000);
  Serial.begin(115200); //2000000 has 0% error (pjrc.com/teensy/td_uart.html)(115200 has .02 with teensy 3.5, -0.02 with 3.2)
  Serial.println("\nbooting ARDUINO");
  //initialize the ADS
  pinMode(ADS_CS_PIN, OUTPUT);
  pinMode(ADS_RDY_PIN, INPUT);
  pinMode(ADS_RST_PIN, OUTPUT);
  initADS();
  Serial.println("done init");
  //determine the conversion factor
    //do some calculations for the constants
  bitToVolt = (resolution*Gain)/(2 * vRef);
}

int32_t val1;
int32_t val2;
int32_t val3;
int32_t val4;
void loop() {
  // put your main code here, to run repeatedly:
  //val1 = readDiff_0_1();//Single_2();
  //val2 = readDiff_2_3();//Single_0(); 
  val1 = readSingle_0();//readDiff_0_1(); 
  //val2 = readSingle_2();
  //val3 = readSingle_4();
  
  
  Serial.println(val1 / bitToVolt, 6);
  //Serial.print(" | ");
  //Serial.print(val2 / bitToVolt, 6);
  //Serial.print(" | ");
  //Serial.println(val3 / bitToVolt, 6);
}
