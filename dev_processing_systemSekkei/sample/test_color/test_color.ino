#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 tcs =Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_60X);
void setup () {
Serial.begin(9600);
tcs.begin();
}
// The commented-out code in loop is example of getRawData with

void loop() {
float red, green, blue;
tcs.getRGB(&red, &green, &blue);
 //uint16_t red, blue, green, clr;
 //tcs.getRawData(&red, &green, &blue, &clr);
 //Serial.print("C:\t"); Serial.print(int(clr));
Serial.print("R:\t"); Serial.print(int(red));
Serial.print("\tG:\t"); Serial.print(int(green));
Serial.print("\tB:\t"); Serial.print(int(blue));
Serial.print("\n");
 delay(100);
}
