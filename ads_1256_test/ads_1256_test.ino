//#include <SPI.h>
#include "ADS1256.h"
//#include <ads_1256_stuff.ino>

//(other stuff for getting the ADS1526 to work is in the next tab
#define ADS_RST_PIN    8 //ADS1256 reset pin
#define ADS_RDY_PIN    22 //ADS1256 data ready
#define ADS_CS_PIN    21 //ADS1256 chip select
#define ADS_Gain
#define Acquisition
/* 
    DIN  - pin 11 (MOSI)
    DOUT - pin 12 (MISO)
    CLK  - pin 13 (SCK)
*/


//put the ADC constants here
double resolution = pow(2, 24);// - 1;// 2^23-1 //8388608.;
//this needs to match the setting in the ADC init function in the library tab
double Gain = 1.; //be sure to have a period 
double vRef = 5.0; //reference voltage
//we'll calculate this in setup
double bitToVolt = 0.;

void setup() {
  delay(1000);
  Serial.begin(115200); //2000000 has 0% error (pjrc.com/teensy/td_uart.html)(115200 has .02 with teensy 3.5, -0.02 with 3.2)
  Serial.println("booting");
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

double value = 0;
val1 = readDiff_0_1();//readSingle(P_AIN0);
//val1 = readSingle_0();//Value();
//val2 = readSingle_3();//readSingle(P_AIN1);
//val3 = readDiff_4_5();
//val4 = readDiff_6_7();
//for (int i = 0; i <1; i++){
  //read_three_values();
//  value += val1;
//}

value /=1.;
  Serial.println(val1 / bitToVolt, 6);
}
