#include <Wire.h>
#include <ZumoShield.h>

#define SPEED           200
#define TURN_BASE_SPEED 100


ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
ZumoIMU imu;

ZumoIMU::vector<int16_t> m_max; //maximum magnetometer values,use for calibration
ZumoIMU::vector<int16_t> m_min; //minimum magnetometer valuses,used for calibration

void setup(){
  // The highest possible magnetic value to read in any direction is 32767
  // The lowest possible magnetic value to read in any direction is -32767
  ZumoIMU::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32767, -32767, -32767};
  unsigned char index;
  Serial.begin(9600);

  // Initialize the Wire library and join the I2C bus as a master
  Wire.begin();

  // Initialize IMU
  imu.init();

  // Enables accelerometer and magnetometer
  imu.enableDefault();

  imu.configureForCompassHeading();

  button.waitForButton();
  Serial.println("starting calibration");
}

void loop()
{
  
}