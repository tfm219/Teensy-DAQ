/** Pin Setup
 * ST, 3
 * RST, 8
 * CS, 21 (4.0: 10)
 * DRDY, 22
 * DOUT, 12 (SDO?)
 * DIN, 11
 * SCLK, 13
 * SYNC (PDWN) (sync in), 23
 * 
 * +5v (J5.3)
 * GND(J5.5, J5.6)
 * 3.3v (j5.9
 */
// #include "ad7779.h"
#include "tensorflow/lite/micro/examples/person_detection/image_provider.h"
#include <SPI.h>
#define ST     3 //ADS1258 start pin (CNTL, J6.1)
#define RST    8 //ADS1256 reset pin
#define CS    21 //ADS1256 chip select
#define DRDY  22 //ADS1256 data ready
#define SYNC  23 // (sync_in) PDWN pin

#define SPI_SPEED 30000000

volatile int count = 0;
void setup() {
  delay(1000);
  Serial.begin(115200);
  
  pinMode(DRDY, INPUT);
  pinMode(CS, OUTPUT);
  pinMode (ST, OUTPUT);
  pinMode(SYNC, OUTPUT);
  pinMode(RST, OUTPUT);
  SPI.begin();
  Serial.println("Starting");
  Serial.println("LOOP");
  digitalWrite(ST, HIGH);
  digitalWrite(RST, HIGH);
  digitalWrite(SYNC, HIGH);
  delay(50);

  
  digitalWrite(ST, LOW);
  delay(10);
  digitalWrite(ST, HIGH);
  
  digitalWrite(RST, LOW);
  delay(10);
  digitalWrite(RST, HIGH);
  
  digitalWrite(SYNC, LOW);
  delay(10);
  digitalWrite(SYNC, HIGH);
}
//1 read
//0 write
void loop() {
  Serial.println();
  SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0)); // initialize SPI with  clock, MSB first, SPI Mode1
  digitalWriteFast(CS, LOW);
  //delayMicroseconds(1);
  Serial.println(count);
  Serial.println(SPI.transfer(0x80 | 0x11), HEX);
  Serial.println(SPI.transfer(0x00), HEX);
  //Serial.println(SPI.transfer(0x00), HEX);
  digitalWriteFast(CS, HIGH);
  SPI.endTransaction();
  count++;
  delay(50);
}
