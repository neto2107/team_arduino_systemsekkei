#define CALIBRATION_SAMPLES 70   // Number of compass readings to take when calibrating
#define CRB_REG_M_2_5GAUSS 0x60  // CRB_REG_M value for magnetometer +/-2.5 gauss full scale
#define CRA_REG_M_220HZ 0x1C     // CRA_REG_M value for magnetometer 220 Hz update rate
unsigned long compassTimaPrev = 0;
float accel_prev_value = 0;
float real_accel_offset = 0;

void setupIMU() {
  Wire.begin();
  // Initialize IMU
  imu.init();
  // Enables accelerometer and magnetometer
  imu.enableDefault();
  imu.configureForCompassHeading();                                           // 良く分からないが必要
}

//現在の角度に指定の角度を追加して返す。
int getAddedDirection(int addedValue) {
  int direction = heading_G2;
  direction += addedValue;
  if (direction < 0) direction += 360;
  if (direction > 360) direction -= 360;
  return direction;
}

//戻り値:進行方向の加速度mm/s
void setRealAccel() {
  float a = sqrtf(ax * ax + ay * ay) / abs(az) * 9800;
  float filterd_a = abs(rc_filter(0.8, accel_prev_value, a) - real_accel_offset);
  real_a = filterd_a;
  accel_prev_value = a;
}

int getAccelDirecton() {
  int direction = atan2(ay, ax) * 180 / M_PI;
  if (direction < 0) direction += 360;
  return direction;
}

void setAccelOffset() {
  float a = sqrtf(ax * ax + ay * ay) / abs(az) * 9800;
  real_accel_offset = a;
}


void calibrationCompass() {
  unsigned char index;
  ZumoIMU::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32767, -32767, -32767};
  button.waitForButton();

  Serial.println("starting calibration");
  int motorL, motorR;

  // To calibrate the magnetometer, the Zumo spins to find the max/min
  // magnetic vectors. This information is used to correct for offsets
  // in the magnetometer data.
  motors_G.setLeftSpeed(200);
  motors_G.setRightSpeed(-200);

  for(index = 0; index < CALIBRATION_SAMPLES; index ++)
  {
    // Take a reading of the magnetic vector and store it in compass.m
    imu.readMag();

    running_min.x = min(running_min.x, imu.m.x);
    running_min.y = min(running_min.y, imu.m.y);

    running_max.x = max(running_max.x, imu.m.x);
    running_max.y = max(running_max.y, imu.m.y);

    Serial.println(index);

    delay(50);
  }

  motors_G.setLeftSpeed(0);
  motors_G.setRightSpeed(0);
  Serial.print("max.x   ");
  Serial.print(running_max.x);
  Serial.println();
  Serial.print("max.y   ");
  Serial.print(running_max.y);
  Serial.println();
  Serial.print("min.x   ");
  Serial.print(running_min.x);
  Serial.println();
  Serial.print("min.y   ");
  Serial.print(running_min.y);
  Serial.println();
  // Set calibrated values to compass.m_max and compass.m_min

    // Store calibrated values in m_max and m_min
  m_max.x = running_max.x;
  m_max.y = running_max.y;
  m_min.x = running_min.x;
  m_min.y = running_min.y;

}

void getCompass() {
  if (timeNow_G - compassTimaPrev < 100) return;

  ax = imu.a.x;  //map(compass.a.x,-32768,32767,-1024,1023);
  ay = imu.a.y;  //map(compass.a.y,-32768,32767,-1024,1023);
  az = imu.a.z;  //map(compass.a.z,-32768,32767,-1024,1023);

  heading_G = averageHeadingLP();
  heading_G2 =relativeHeading(heading_G,start_heading_G);
  setRealAccel();


  compassTimaPrev = timeNow_G;
}


// Converts x and y components of a vector to a heading in degrees.
// This calculation assumes that the Zumo is always level.
template <typename T> float heading(ZumoIMU::vector<T> v)
{
  float x_scaled =  2.0*(float)(v.x - m_min.x) / (m_max.x - m_min.x) - 1.0;
  float y_scaled =  2.0*(float)(v.y - m_min.y) / (m_max.y - m_min.y) - 1.0;

  float angle = atan2(y_scaled, x_scaled)*180 / M_PI;
  if (angle < 0)
    angle += 360;
  return angle;
}

// Yields the angle difference in degrees between two headings
float relativeHeading(float heading_from, float heading_to)
{
  float relative_heading = heading_to - heading_from;

  // constrain to -180 to 180 degree range
  if (relative_heading > 180)
    relative_heading -= 360;
  if (relative_heading < -180)
    relative_heading += 360;

  return relative_heading;
}

// Average 10 vectors to get a better measurement and help smooth out
// the motors' magnetic interference.
float averageHeading()
{
  ZumoIMU::vector<int32_t> avg = {0, 0, 0};

  for(int i = 0; i < 10; i ++)
  {
    imu.readMag();
    avg.x += imu.m.x;
    avg.y += imu.m.y;
  }
  avg.x /= 10.0;
  avg.y /= 10.0;

  // avg is the average measure of the magnetic vector.
  return heading(avg);
}

float averageHeadingLP() {
  ZumoIMU::vector<int32_t> avg = {0, 0, 0};
  imu.readMag();
  avg.x = 0.8 * imu.m.x + 0.2 * avg.x;
  avg.y = 0.8 * imu.m.y + 0.2 * avg.y;


  // avg is the average measure of the magnetic vector.
  return heading(avg);
}
