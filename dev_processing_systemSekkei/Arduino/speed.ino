//速度を計算mm/s
void calSpeed(){
  static unsigned int prevSpeedTime = 0;
  unsigned int diff = timeNow_G - prevSpeedTime;//時間の差分
  if(diff > 100){
    vx = vx + real_a * (diff*0.001);
    prevSpeedTime = timeNow_G;
  }
}

float rc_filter(float a,float prev_value,float now_value){
  return a * prev_value + (1-a) *now_value;
}
