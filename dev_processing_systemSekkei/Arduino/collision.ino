
void collisionCheck() {//前進してるはずなのに何らかの理由で前進できていないときにブザーがなる
  heading_G = atan2(my,mx) * 180 / M_PI;

  if(speed0>=100 && collision_prev_dist-dist_G <1&&dist_G != -1) {
    buzzer.play(">c32");
  }
  collision_prev_dist=dist_G;
}

float calDiffAngle(){
    heading_G = atan2(my,mx) * 180 / M_PI;

    if (heading_G<0) {
      heading_G += 360;
    }
    if(waitfor(100) == 1){
      float df = heading_G - collision_bf_angle;

      Serial.println(df);
      if(df <1 &&df > -100){
        buzzer.play(">c32");
      }
      collision_bf_angle = heading_G;
    }
    return 0;

}