
//速度を計算mm/s
void calPos(){
  unsigned long now_time = millis();
  now_Pos[0]+=speed0*cos(DEG_TO_RAD(heading_G2))*(now_time-speed_pos_prev_time)*0.001;
  now_Pos[1]+=speed0*sin(DEG_TO_RAD(heading_G2))*(now_time-speed_pos_prev_time)*0.001;
  speed_pos_prev_time = now_time;
}

void resetPos(){
  now_Pos[0] = 0;
  now_Pos[1] = 800;
}

float setup_speed100(){

}

float rc_filter(float a,float prev_value,float now_value){
  return a * prev_value + (1-a) *now_value;
}
