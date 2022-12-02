unsigned cal_pos_prev_time=0;
//速度を計算mm/s
void calPos(){
  //now_Pos[0]=cos();
}

float setup_speed100(){

}

float rc_filter(float a,float prev_value,float now_value){
  return a * prev_value + (1-a) *now_value;
}
