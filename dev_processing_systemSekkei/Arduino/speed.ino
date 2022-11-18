//速度を計算mm/s
void calSpeed(){
  static unsigned int prevSpeedTime = 0;
  unsigned int diff = timeNow_G - prevSpeedTime;//時間の差分
  int direction = abs(getAccelDirecton());
  if(diff >100){
    vx = vx + (real_a * (diff))*((direction-90)/90);
    prevSpeedTime = timeNow_G;
  }
    
}

float setup_speed(){
  button.waitForButton(); // Zumo buttonが押されるまで待機
  motors_G.setSpeeds(100);
  
  
}

float setup_speed100(){

}

float rc_filter(float a,float prev_value,float now_value){
  return a * prev_value + (1-a) *now_value;
}
