
void modeChanger() {
  switch (Online_Mode_A) {

    case INIT:
      Online_Mode_A = SEARCH;
      break;

    case SEARCH:
      move_ditecting(30000);
      if (mode_B_IsFinished == true) {
        if (resultId_B == DISCOVERY) {
          Online_Mode_A = SEARCH2;
        } else if (resultId_B == NOT_DISCOVERY) {
          Online_Mode_A = SEARCH;
        }
      }
      break;
    case SEARCH2:
      move_detected();
      if (mode_B_IsFinished == true) {
        switch (resultId_B) {
          case DETACT_TRUE:
            Online_Mode_A = CATCH;

          case DETACT_FAILD:
            Online_Mode_A = SEARCH;
        }
      }
      break;

    case CATCH:
      move_catch();
      if (mode_B_IsFinished == true) {
        switch (resultId_B) {
          case CATCH_SUCCESS:
            Online_Mode_A = BACK_TO_GOAL;
            break;
          case CATCH_FAIED:
            Online_Mode_A = SEARCH;
        }
      }
      break;
    
    case BACK_TO_GOAL:
      back_to_goal();
      if(mode_B_IsFinished){
        Online_Mode_A = INIT;
      }

    case ROTATE:
      Online_Mode_A = STOP;
      break;

    case STOP:
      Online_Mode_A = INIT;
      break;
  }
  //mode_A_IsFinished = false;
}
//応用動作---------------------------------------------------------------------
//Mode B
int ditecting_direction;  //次に回転する角度
//宝物を探す動き
/*
*発見時 DISCOVERY
*未発見時 NOT_DISCOVERY
*/
void move_ditecting(unsigned long millis_time) {

  switch (Online_Mode_B) {
    case INIT:
      mode_B_timePrev = timeNow_G;
      Online_Mode_B = SEARCH;
      mode_B_IsFinished = false;
      break;
    case SEARCH:  //直進、回転を繰り返す。
      move_forward(2000);
      if (mode_C_IsFinished == true) {
        Online_Mode_B = SEARCH2;
      }
      break;

    case SEARCH2:
      ditecting_direction = getAddedDirection(60);
      Online_Mode_B = SEARCH3;
      break;

    case SEARCH3:
      move_rotate(ditecting_direction);
      if (mode_C_IsFinished == true) {
        Online_Mode_C = SEARCH;
      }
      break;
  }
  if (dist_G < SONIC_THRESHOLD) {  //発見したら
    resultId_B = DISCOVERY;
    Online_Mode_B = INIT;
    mode_B_IsFinished = true;
  }
  if (timeNow_G - mode_B_timePrev > millis_time) {  //見失ったら
    resultId_B = NOT_DISCOVERY;
    Online_Mode_B = INIT;
    mode_B_IsFinished = true;
  }
}

//超音波センサー感知時の動作//本当にあっているかどうかを確認する
//返り値 動作時の偏差
/*
*成功時 DETACT_TRUE
*失敗時 DETACT_FAILD
*/
int move2_predist;  //計測開始時のdistの値
void move_detected() {
  static bool move2_keisoku_flag = false;
  float diff;
  switch (Online_Mode_B) {
    case INIT:
      mode_B_timePrev = timeNow_G;
      Online_Mode_B = KEISOKU;
      move2_predist = dist_G;
      mode_B_IsFinished = false;
      move2_keisoku_flag = false;
      break;

    case KEISOKU:
      if (timeNow_G - mode_B_timePrev <= 1000) {  //1秒間計測
        move_stop(10000);
        if (dist_G < pre_dist - 3 && dist_G > pre_dist + 3) {  //distが+-3の範囲に収まっていないなら
          move2_keisoku_flag = true;                           //フラグを1にする
          buzzer.play("!L16 cde");
        }
      } else {                        //1秒経過後
        if (keisoku_flag == false) {  //カップが動いていないなら
          resultId_B = DETACT_TRUE;
            Online_Mode_B = INIT;
          mode_B_IsFinished = true;
        } else {  //動いているなら
          Online_Mode_B = INIT;
          resultId_B = DETACT_FAILD;
          mode_B_IsFinished = true;
        }
      }
  }
  return diff;
}

//宝物を実際に取りに行くときの動作
void move_catch() {
  switch (Online_Mode_B) {
    case INIT:
      mode_B_timePrev = timeNow_G;
      Online_Mode_B = CATCH;
      mode_B_IsFinished = false;
      break;

    case CATCH:
      if (distance <= SONIC_THRESHOLD) {
        move_forward(10000);
        if (distance <= 3) {  //キャッチしたと判断したときは
          move_stop(10000);   //停止する
          resultId_B = CATCH_SUCCESS;
          Online_Mode_B = INIT;      //キャッチが成功
          mode_B_IsFinished = true;  //遷移モードにする
        }
      } else {
        //途中で見失ってしまったときは
        move_stop(10000);  //停止させる
        resultId_B = CATCH_FAIED;
        Online_Mode_B = INIT;      //キャッチが失敗
        mode_B_IsFinished = true;  //遷移モードにする
      }
      break;
  }
}

//ゴールに帰ってくるときの動作
void back_to_goal() {
  switch (Online_Mode_B) {
    case INIT:
      mode_B_timePrev = timeNow_G;
      Online_Mode_B = BACK_TO_GOAL;
      mode_B_IsFinished = false;
      break;
    case BACK_TO_GOAL:
      move_rotate(180);
      if (Online_Mode_C == ROTATE && mode_C_IsFinished == true) {  //指定角度に向いたら
        move_forward_of_direction(180, 10000);                     //直進する
        if (Online_Mode_C == FORWARD && now_color_id != WHITE) {   //線を検知したら
          Online_Mode_B = REACHED_GOAL;
        }
      }
      break;

    case REACHED_GOAL:  //ゴールに到達したら
      move_back2(500);  //500ms後退
      if (mode_C_IsFinished == true) {
        Online_Mode_B = REACHED_GOAL2;
      }

      break;

    case REACHED_GOAL2:
      move_rotate(0);
      if (mode_C_IsFinished == true) {
        Online_Mode_B = INIT;
        mode_B_IsFinished = true;  //動作終了
      }

      break;
  }
}

//基礎動作--------------------------------------------------------------
//Mode_C
//指定秒間ライントレースする
void move_linetrace(unsigned int millis_time, int mode) {
  if (Online_Mode_C != LINE_TRACE) {
    online_time_prev = timeNow_G;
    Online_Mode_C = LINE_TRACE;
    mode_C_IsFinished = false;
    linetrace_init();
  } else if (timeNow_G - online_time_prev <= millis_time) {
    speed_diff = linetrace_P(mode);
  } else {
    mode_C_IsFinished = true;
    Online_Mode_C = INIT;
  }
}

//指定時間前進する
void move_forward(unsigned int millis_time) {
  if (Online_Mode_C != FORWARD) {
    online_time_prev = timeNow_G;
    Online_Mode_C = FORWARD;
    mode_C_IsFinished = false;
    move_front_init();
  } else if (timeNow_G - online_time_prev <= millis_time) {
    speed_diff = move_front();
  } else {
    mode_C_IsFinished = true;
    Online_Mode_C = INIT;
  }
}

void move_forward_of_direction(int direction_radius, unsigned int millis_time) {
  if (Online_Mode_C != FORWARD) {
    online_time_prev = timeNow_G;
    Online_Mode_C = FORWARD;
    mode_C_IsFinished = false;
    move_front_init(direction_radius);
  } else if (timeNow_G - online_time_prev <= millis_time) {
    speed_diff = move_front();
  } else {
    mode_C_IsFinished = true;
    Online_Mode_C = INIT;
  }
}

//指定時間後退する
void move_back2(unsigned int millis_time) {
  if (Online_Mode_C != BACK) {
    online_time_prev = timeNow_G;
    Online_Mode_C = BACK;
    mode_C_IsFinished = false;
    move_back_init();
  } else if (timeNow_G - online_time_prev <= millis_time) {
    speed_diff = move_back();
  } else {
    mode_C_IsFinished = true;
    Online_Mode_C =INIT;
  }
}


//指定角度まで回転する
void move_rotate(int direction) {
  if (Online_Mode_C != ROTATE) {
    online_time_prev = timeNow_G;
    Online_Mode_C = ROTATE;
    mode_C_IsFinished = false;
  } else if (heading_G2 <= direction - 3 || heading_G2 >= direction + 3) {
    speed_diff = turnTo(direction);
  } else {
    mode_C_IsFinished = true;
    Online_Mode_C = INIT;
  }
}
//指定時間停止する
void move_stop(unsigned int millis_time) {
  if (Online_Mode_C != STOP) {
    online_time_prev = timeNow_G;
    Online_Mode_C = STOP;
    mode_C_IsFinished = false;
    stop_init();
  } else if (timeNow_G - online_time_prev <= millis_time) {
    speed_diff = stop();
  } else {
    mode_C_IsFinished = true;
  }
}
//----------------------------------------------------------------------------------

//ModeD 中間動作


//後退した後、指定角度足した向きを向く
void move_back_to_rotate(int added_direction){
  int after_direction = 0;
  switch(Online_Mode_D){
    case INIT:
      online_time_prev = timeNow_G;
      Online_Mode_D = BACK;
      mode_D_IsFinished = false;
      break;
    
    case BACK:
      move_back2(500);
      if(mode_C_IsFinished ==true){
        Online_Mode_D = ROTATE;
        after_direction = getAddedDirection(added_direction);
      }

    case ROTATE:
      move_rotate(after_direction);
      if(mode_C_IsFinished){
        Online_Mode_D = INIT;
        mode_D_IsFinished =true;
      }

  }

}
