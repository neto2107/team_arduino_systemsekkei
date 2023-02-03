//speed100の初期化
void calibration(){
  float speed  = -1;
  float speed_diff;
  buzzer.play("L16 cdegreg4");
  while (speed <=0) {

    timeNow_G = millis();
    getRGB(r_G, g_G, b_G);
    dist_G = distance();
    getCompass();
    recvTusin();
    if(timeNow_G-turnTimePrev>100){
          //buzzer.play("L16 cdegreg4");
      
      speed = calibrationSpeed();
      MotorL_G=DEFAULT_SPEED;
      MotorR_G=DEFAULT_SPEED;
      motors_G.setSpeeds(MotorL_G, MotorR_G);
      turnTimePrev = timeNow_G;
    }
  }
  
  MotorL_G=0;
  MotorR_G=0;
  motors_G.setSpeeds(MotorL_G, MotorR_G);
  Serial.println(speed);
  speed100 = speed;

}

float calibrationSpeed(){
  unsigned long speedPrev;
  
  if(Nearest_Neighbor()==3){
    speedPrev = millis();
  }
  if(Nearest_Neighbor()==2){
    float diff = timeNow_G - speedPrev;
    float speed = 200/(diff*0.001); //速度(mm/s)
    return speed;
  }
  return -1;
}

float setStartDirection(){
  getCompass();
  switch (ROBOT_NUM) {
  case 1:
    start_heading_G = heading_G - 90; //自ゴールから見て右に置く
  break;
  case 2:
      start_heading_G = heading_G + 90;//自ゴールから見て左に置く
  break;
  default:
    start_heading_G = heading_G;
  break;

  }
  if (start_heading_G<0) start_heading_G += 360;

}

