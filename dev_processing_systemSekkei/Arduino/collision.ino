float bf_angle; //前回の角度
float rc;
float prev_dist;
void collisionCheck() {//前進してるはずなのに何らかの理由で前進できていないときにブザーがなる
  heading_G = atan2(my,mx) * 180 / M_PI;

  if(vx>100 && prev_dist-dist_G <1) {
    buzzer.play(">c32");
  }
  prev_dist=dist_G;
}

float calDiffAngle(){
    heading_G = atan2(my,mx) * 180 / M_PI;

    if (heading_G<0) {
      heading_G += 360;
    }
    if(waitfor(100) == 1){
      float df = heading_G - bf_angle;

      Serial.println(df);
      if(df <1 &&df > -100){
        buzzer.play(">c32");
      }
      bf_angle = heading_G;
    }
    return 0;

}

int waitfor( unsigned long period )
{
  static int flagStart = 0; // 0:待ち状態, 1:現在計測中
  static unsigned long startTime = 0;

  if ( flagStart == 0 ) {
    startTime = timeNow_G;
    flagStart = 1; // 現在計測中にしておく
  }

  if ( timeNow_G - startTime > period ) { // 計測開始からの経過時間が指定時間を超えた
    flagStart = 0;  // 待ち状態に戻しておく
    startTime = 0;  // なくてもよいが, 形式的に初期化
    return 1;
  }
  return 0;
}
