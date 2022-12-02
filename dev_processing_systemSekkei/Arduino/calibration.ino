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
  speed100 = speed;
}

float calibrationSpeed(){
  unsigned int speedPrev;
  
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

