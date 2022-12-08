
void modeChanger(){
  switch (Online_Mode_A) {
  
    case INIT:
      Online_Mode_A = FORWARD;
      break;
    
    case FORWARD:
      Online_Mode_A = ROTATE;
      break;
    
    case ROTATE:
      Online_Mode_A = STOP;
      break;

    case STOP:
      Online_Mode_A = INIT;
      break;
  }
  //online_change_flag = false;
}
//応用動作---------------------------------------------------------------------
//超音波センサー感知時の動作//本当にあっているかどうかを確認する
//返り値 動作時の偏差
int move2_predist; //計測開始時のdistの値
void move_detected(){
  static bool move2_keisoku_flag = false;
  float diff;
  switch(Online_Mode_B){
    case INIT:
      online_time_prev2 = timeNow_G;
      Online_Mode_B = KEISOKU;
      move2_predist = dist_G;
      online_change_flag_B = false;
      move2_keisoku_flag = false;
      break;
    
    case KEISOKU:
      if(timeNow_G - online_time_prev2<= 1000){ //1秒間計測
        move_stop(10000);
        if(dist_G < pre_dist -3 && dist_G > pre_dist + 3){//distが+-3の範囲に収まっていないなら
          move2_keisoku_flag = true; //フラグを1にする
          buzzer.play("!L16 cde");
        }
      }else{ //1秒経過後
        if(keisoku_flag==false){//カップが動いていないなら
          Online_Mode_B = DETACT_TRUE;
          online_change_flag_B = true;
        }else{//動いているなら
          Online_Mode_B = DETACT_FAILD;
          online_change_flag_B = false;
        }
      }
  }
  return diff;
}

//宝物を実際に取りに行くときの動作
void move_catch(){
  switch(Online_Mode_B){
    case INIT:
      online_time_prev2 = timeNow_G;
      Online_Mode_B = CATCH;
      online_change_flag_B = false;
      break;
    
    case CATCH:
      if(distance  <= SONIC_THRESHOLD){
        move_forward(10000);
        if(distance <= 3){//キャッチしたと判断したときは
          move_stop(10000);//停止する
          Online_Mode_B = CATCH_SUCCESS; //キャッチが成功
          online_change_flag_B = true; //遷移モードにする
        }
      }else{
        //途中で見失ってしまったときは
        move_stop(10000);//停止させる
        Online_Mode_B = CATCH_FAIED; //キャッチが失敗
        online_change_flag_B = true; //遷移モードにする
      }
    break;
  }
}

//ゴールに帰ってくるときの動作
void back_to_goal(){
 switch(Online_Mode_B){
   case INIT:
    online_time_prev2 = timeNow_G;
    Online_Mode_B = BACK_TO_GOAL;
    online_change_flag_B = false;
    break;
    case BACK_TO_GOAL:
      move_rotate(180);
      if(online_change_flag_C==true){//指定角度に向いたら

      }

 }
}

//基礎動作--------------------------------------------------------------
//指定秒間ライントレースする
void move_linetrace(unsigned int millis_time,int mode){
  if(Online_Mode_C != LINE_TRACE){
    online_time_prev = timeNow_G;
    Online_Mode_C = LINE_TRACE;
    online_change_flag_C = false;
    linetrace_init();
  }else if(timeNow_G - online_time_prev <= millis_time){
    speed_diff  = linetrace_P(mode);
  }else{
    online_change_flag_C = true;
  }
}

//指定時間前進する
void move_forward(unsigned int millis_time){
  if(Online_Mode_C != FORWARD){
    online_time_prev = timeNow_G;
    Online_Mode_C = FORWARD;
    online_change_flag_C = false;
    move_front_init();
  }else if(timeNow_G - online_time_prev <= millis_time){
    speed_diff= move_front();
  }else{
    online_change_flag_C = true;
  }
}

//指定時間後退する
void move_back2(unsigned int millis_time){
  if(Online_Mode_C!= BACK){
    online_time_prev = timeNow_G;
    Online_Mode_C = BACK;
    online_change_flag_C = false;
    move_back_init();
  }else if(timeNow_G - online_time_prev <= millis_time){
    speed_diff = move_back();
  }else{
    online_change_flag_C = true;
  }
}
//指定角度まで回転する
void move_rotate(int direction){
  if(Online_Mode_C != ROTATE){
    online_time_prev = timeNow_G;
    Online_Mode_C = ROTATE;
    online_change_flag_C = false;
  }else if(heading_G2 <= direction -3 || heading_G2 >= direction+3){
    speed_diff =turnTo(direction);
  }else{
    online_change_flag_C = true;
  }   
}
//指定時間停止する
void move_stop(unsigned int millis_time){
  if(Online_Mode_C != STOP){
    online_time_prev = timeNow_G;
    Online_Mode_C = STOP;
    online_change_flag_C = false;
    stop_init();
  }else if(timeNow_G - online_time_prev <= millis_time){
    speed_diff= stop();
  }else{
    online_change_flag_C = true;
  }
}
//----------------------------------------------------------------------------------