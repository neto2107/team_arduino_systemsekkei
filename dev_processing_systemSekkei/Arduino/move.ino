

void mover() {
  float speed_diff=0;

  if (button.isPressed()) {
    speed_reset();
    buzzer.play("L16 cdegreg4");        // ブザーにて音楽を鳴らす
    button.waitForButton();
    moveTimePre = timeNow_G;
  }
  if(timeNow_G-moveTimePre<1000){
    speed_diff =stop();
  }else if (timeNow_G - moveTimePre < 3000) {
    speed_diff = move_front();
  } else if(timeNow_G - moveTimePre < 4000) {
    speed_diff = stop();
  } else if (timeNow_G - moveTimePre < 9000) {
    speed_diff = move_front();
  }else{
    speed_diff = stop();
  }
  MotorL_G=speed0+speed_diff;
  MotorR_G=speed0-speed_diff;
  motors_G.setSpeeds(MotorL_G, MotorR_G);
}

//動きの動作
int move_one_flag=-1;
int move_direction=0;
float move_front(){
  if(move_one_flag!=1){//これまで前進していなかったら
    move_direction = heading_G;
    move_one_flag = 1;
    now_speed = speed100;
  }
  speed0 = DEFAULT_SPEED;
  return turnTo(move_direction);


}

float stop(){
  if(move_one_flag!=0){
    move_one_flag = 0;
    move_direction = heading_G;
    now_speed = 0;
  }
  
  speed0 = 0;
  return turnTo(move_direction);
}

float speed_reset(){
  move_one_flag = -1;
  move_direction = heading_G;
}



//指定の方角に移動する
float sum_e = 0;
float turnTo(float theta_r) {
  float u;
  float KP = 4.0;
  float TIinv = 2/1000.0;

  if(move_thetaPrev_G!=theta_r) {
    move_timeStart_G=-1;
  }
  if (move_timeStart_G < 0) {
    move_timeStart_G=millis();
  }

  heading_G = atan2(my,mx) * 180 / M_PI;
  if (heading_G<0) heading_G += 360;
  float e = theta_r-heading_G;
  if (e<-180) e+=360;
  if (e>180)  e-=360;
  if (abs(e) > 45.0 ) { // |e|>45のときはP制御
    u = KP*e;           // P制御
  } else {              // |e|<=45 の時はPI制御
    sum_e += TIinv*e*(timeNow_G-turnTimePrev);
    u = KP*(e+sum_e);   // PI 制御
  }
  if ( u> 180 ) u = 180;  // 飽和
  if ( u<-180 ) u = -180; // 飽和

    //位置の補正が利かないときは最速でコースアウトする
  if(-10 < heading_G-theta_r && heading_G-theta_r < 10){//ほぼ理想の方向を向いてるのでこのまま
  }
  else if (timeNow_G - move_timeStart_G > 3000) {//3秒以上方向の転換が起こらない
    motors_G.setSpeeds(1000, 1000);//変な方向を向いたまま戻らなければ最速でコースアウトしてスタートからやり直す
  }
  move_thetaPrev_G = theta_r;

  return u;
}
