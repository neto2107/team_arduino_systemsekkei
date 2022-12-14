//主に吉原作成



unsigned int mode=INIT;


void change_timing(){
  if(millis() - change_time > 5000){//10病経過したら移動時間、回転時間を変更する

    move_time = random(1000,3000);
    rotate_time = random(1000,3000);
    change_time = millis();
  }
}

//int identify_color(int r, int g, int b) {//色を見分ける関数
//  if (200 < r && 200 < g && 200 < b) {
//    return WHITE; //白
//  } else if (r < 50 && g < 50 && b < 50) {
//    return BLACK; //黒
//  } else if (100 < r && g < 50 && b < 50) {
//    return RED; //赤
//  } else if (r < 50 && g < 50 && 70 < b) {
//    return BLUE; //青
//  } else {
//    return OTHER; //その他
//  }
//}

void task() {//lineトレース関数
//  color = identify_color(r, g, b);
  color = Nearest_Neighbor();//k近傍法を用いて色を取得
  switch (mode) {
    case INIT: //初期状態
      mode = FORWARD;
      start_time = millis();
      start = heading_G;
      goal = heading_G+180;
      if (goal>360){
        goal-=360;
      }
      break;
      
    case FORWARD: //直進
      MotorL_G = MotorR_G = 150;
      if (waitfor(1000)==1) {
        mode = SEARCH;
        start_time = millis();
      }
      break;

    case SEARCH: //回転
      MotorL_G =  150;
      MotorR_G = -150;
      if (waitfor(1000)==1) {//1秒の回転のあとの処理
        if(waitfor(1000)==1) {//
          mode = FORWARD;
          start_time = millis();
        }
        else {
          turnTo(start);//1秒間で最初の向きに向き直す
        }
        
      }
      else {//回転中
        if(0<dist_G && dist_G<DIST_LINE) {
          mode=CATCH;
          start_time = millis();
        }
      }
      break;

        
     
    case CATCH:
      MotorR_G = MotorL_G = 150;
      if(0 < dist_G && dist_G < 7) {
        mode = RETURN;
        start_time = millis();
      }
      if(dist_G==0) {
        mode = SEARCH;
        start_time = millis();
      }
      
      break;

    case RETURN:
//      MotorR_G = MotorL_G = 150;
      if(waitfor(1000)==1) {//最初の方向いて帰る
        if (Nearest_Neighbor() == 2 || Nearest_Neighbor() == 3) {//赤or青を検知
          mode = BACK;
          start_time = millis();
        }
        else{//赤or青を検知するまで進む
          MotorR_G = MotorL_G = 150;
        }
      }
      else{
        turnTo(goal);
      }

      
      break;

    case BACK:
      if (waitfor(500)==1) {
        if (waitfor(1000)==1) {
          mode=FORWARD;
        }
        else {
          turnTo(start);
        }
        
      }
      else {
        MotorR_G = MotorL_G = -100;
      }
      
      break;

    case KEISOKU:
      
      break;

      
       case STOP: //停止
      
      
      break;

    case BACK2:
      
      break;
      
    case ROTATE2:
      
      break;

  }
  Serial.print("右=");
  Serial.println(MotorR_G);
  Serial.print("左=");
  Serial.println(MotorL_G);
  Serial.print("mode=");
  Serial.println(mode);
  motors_G.setSpeeds(MotorR_G, MotorL_G);
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
