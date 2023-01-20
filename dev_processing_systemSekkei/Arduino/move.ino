
float value;
void mover() {

  modeChanger();

  MotorL_G = speed0 + speed_diff + LEFT_SPEED_OFFSET;
  MotorR_G = speed0 - speed_diff + RIGHT_SPEED_OFFSET;
  motors_G.setSpeeds(MotorL_G, MotorR_G);
}



//動きの動作
int move_one_flag = -1;
int move_direction = 0;




unsigned long move_meandering_driving_time_prev;
float move_meandering_driving_init() {
  now_speed = speed100;
  move_meandering_driving_time_prev = timeNow_G;
  speed0 = DEFAULT_SPEED;
}

//蛇行運転を行うプログラム
float move_meandering_driving() {
  move_direction = heading_G2;

  unsigned long timediff =timeNow_G - move_meandering_driving_time_prev;
  if(timediff > 6283/2) move_meandering_driving_time_prev = timeNow_G;

  float spd = speed0 * 0.3* sin((timediff/ 500.0)); //スピードの差分
  return spd;
}

float move_front_init(bool high_speed_flag) {
  move_direction = heading_G2;
  if (high_speed_flag) {
    now_speed = speed100 * 2;  //要改善
  } else {
    now_speed = speed100;
  }
}

float move_front(bool high_speed_flag) {  //使わない
  if (high_speed_flag == true) {
    speed0 = HIGH_SPEED;
  } else {
    speed0 = DEFAULT_SPEED;
  }
  return 0;  //turnTo(move_direction);
}

//指定の角度に進み続けるときに使う
float move_front_init(int direction) {
  move_direction = direction;
  now_speed = speed100;
}

float move_back_init() {
  move_direction = heading_G2;
  now_speed = -speed100;
  speed_diff = 0;
}

float move_back() {
  speed0 = -DEFAULT_SPEED;
  return 0;  //turnTo(move_direction);
}

float stop_init() {
  move_direction = heading_G2;
  now_speed = 0;
  speed0 = 0;
  speed_diff = 0;
  return 0;
}
float stop() {
  now_speed = 0;
  speed0 = 0;
  return 0;
}

float speed_reset() {
  move_direction = heading_G2;
}




//指定の方角に移動する
float sum_e = 0;
float turnTo(float theta_r) {

  float u;
  float KP = 4.0;
  float TIinv = 2 / 1000.0;

  if (move_thetaPrev_G != theta_r) {
    move_timeStart_G = -1;
  }
  if (move_timeStart_G < 0) {
    move_timeStart_G = millis();
  }


  float e = theta_r - heading_G2;
  if (e < -180) e += 360;
  if (e > 180) e -= 360;
  if (abs(e) > 45.0) {  // |e|>45のときはP制御
    u = KP * e;         // P制御
  } else {              // |e|<=45 の時はPI制御
    sum_e += TIinv * e * (timeNow_G - turnTimePrev);
    u = KP * (e + sum_e);  // PI 制御
  }
  if (u > 180) u = 120;    // 飽和
  if (u < -180) u = -120;  // 飽和

  //   //位置の補正が利かないときは最速でコースアウトする
  // if(-10 < heading_G2-theta_r && heading_G2-theta_r < 10){//ほぼ理想の方向を向いてるのでこのまま
  // }
  // else if (timeNow_G - move_timeStart_G > 3000) {//3秒以上方向の転換が起こらない
  //   motors_G.setSpeeds(1000, 1000);//変な方向を向いたまま戻らなければ最速でコースアウトしてスタートからやり直す
  // }
  move_thetaPrev_G = theta_r;

  return u;
}
