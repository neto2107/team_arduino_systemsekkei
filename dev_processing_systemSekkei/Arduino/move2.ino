
void modeReseter() {
  Online_Mode_A = INIT;
  Online_Mode_A_pre = INIT;
  Online_Mode_B = INIT;
  Online_Mode_C = INIT;
  Online_Mode_D = INIT;

  resultId_A = INIT;
  resultId_B = INIT;
  resultId_C = INIT;
  resultId_D = INIT;

  

  //use_turnTo = false;
}

void modeChanger() {
  switch (Online_Mode_A) {

    case INIT:
      modeReseter();
      Online_Mode_A = SEARCH;

      break;

    case SEARCH:
      buzzer.play("!L16 c");
      move_ditecting(30000);
      //buzzer.play("!L16 fg");
      //ラインを感知したら
      now_color_id = Nearest_Neighbor();
      if(now_color_id != WHITE){ //ラインを感知したら
        set_back_with_return_fild(Online_Mode_A);
      }
      if (mode_B_IsFinished == true) {
        if (resultId_B == DISCOVERY) {
          Online_Mode_A = SEARCH2;

        } else if (resultId_B == NOT_DISCOVERY) {
          Online_Mode_A = SEARCH;
        }
      }
      break;
    case SEARCH2:
      buzzer.play("!L16 d");
      move_detected();
      if (mode_B_IsFinished == true) {
        switch (resultId_B) {
          case DETACT_TRUE:
            Online_Mode_A = CATCH;
            break;
          case DETACT_FAILD:
            Online_Mode_A = SEARCH;
            break;
        }
      }
      break;

    case CATCH:
      buzzer.play("!L16 e");
      move_catch();
      //buzzer.play("!L16 cdefeg");
      if (mode_B_IsFinished == true) {
        switch (resultId_B) {
          case CATCH_SUCCESS:
            Online_Mode_A = BACK_TO_GOAL;
            break;
          case CATCH_FAIED:
            Online_Mode_A = SEARCH;
        }
      }
      now_color_id = Nearest_Neighbor();
      if(now_color_id != WHITE){ //ラインを感知したら
        set_back_with_return_fild(SEARCH);
      }
      break;

    case BACK_TO_GOAL:
      //buzzer.play("!L16 f");
      back_to_goal();
      if (mode_B_IsFinished) {
        Online_Mode_A = INIT;
      }
      now_color_id = Nearest_Neighbor();
      if(now_color_id != WHITE){ //ラインを感知したら
        set_back_with_return_fild(SEARCH);
      }
      break;

    case BACK_WIGH_RETURN_GOAL:
      move_back_to_rotate();
      if(mode_D_IsFinished){
        Online_Mode_A = Online_Mode_A_pre;
        Online_Mode_A_pre = INIT;

      }
      break;
    default:
      buzzer.play("!L16 g");
      Online_Mode_A = INIT;
      break;
  }
  //mode_A_IsFinished = false;
}

void set_back_with_return_fild(int pre_mode){
  Online_Mode_A_pre = pre_mode;
  Online_Mode_A = BACK_WIGH_RETURN_GOAL;
}
//応用動作---------------------------------------------------------------------
//Mode B
int ditecting_direction;  //次に回転する角度

void reset_Flag_B() {
  mode_B_timePrev = timeNow_G;
  mode_B_IsFinished = false;
  Online_Mode_C = INIT;
  resultId_B = INIT;
  resultId_C = INIT;
}

//宝物を探す動き
/*
*発見時 DISCOVERY
*未発見時 NOT_DISCOVERY
*/
void move_ditecting(unsigned long millis_time) {

  switch (Online_Mode_B) {
    case INIT:
      reset_Flag_B();
      Online_Mode_B = SEARCH;
      break;
    case SEARCH:  //直進、回転を繰り返す。
      //serachSonicSensor = false;
      move_forward(2000);
      if (mode_C_IsFinished == true) {
        Online_Mode_B = SEARCH2;
      }
      break;

    case SEARCH2:
      move_rotate_with_millis(1000, true);
      if (mode_C_IsFinished == true) {
        Online_Mode_B = SEARCH;
      }
      break;
  }
  dist_G = distance();
  if (dist_G < SONIC_THRESHOLD && dist_G > 0) {  //発見したら
    move_stop(10000);
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
      reset_Flag_B();
      Online_Mode_B = KEISOKU_ROTATE_BACK;
      move2_keisoku_flag = false;
      break;
    

    case KEISOKU_ROTATE_BACK:
      //move_rotate_with_millis(0, false);
      //if(mode_C_IsFinished == true){
        Online_Mode_B = KEISOKU;
        dist_G = distance();
        move2_predist = dist_G;
      //}
      break;
    case KEISOKU:
      if (timeNow_G - mode_B_timePrev <= 2000) {  //1秒間計測
        move_stop(10000);
        dist_G = distance();
        if (dist_G < pre_dist - 5 && dist_G > pre_dist + 5) {  //distが+-3の範囲に収まっていないなら
          move2_keisoku_flag = true;                           //フラグを1にする
        }
      } else {                              //1秒経過後
        if (move2_keisoku_flag == false) {  //カップが動いていないなら

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
      reset_Flag_B();
      Online_Mode_B = CATCH;
      break;

    case CATCH:
      dist_G = distance();
      if (dist_G <= SONIC_THRESHOLD) {
        move_forward(10000);

        if (dist_G <= 4) {  //キャッチしたと判断したときは
          move_stop(0);     //停止する
          resultId_B = CATCH_SUCCESS;
          Online_Mode_B = INIT;      //キャッチが成功
          mode_B_IsFinished = true;  //遷移モードにする
        }
      } else {
        //途中で見失ってしまったときは
        move_stop(0);  //停止させる
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

      reset_Flag_B();
      Online_Mode_B = BACK_TO_GOAL;
      break;
    case BACK_TO_GOAL:
      move_rotate(180);
      if (mode_C_IsFinished == true) {  //指定角度に向いたら
        Online_Mode_B = BACK_TO_GOAL2;
      }
      break;

    case BACK_TO_GOAL2:
      move_forward(10000L);
      //Serial.println("back_to_goal2");
      if (now_color_id != WHITE) {
        Online_Mode_B = REACHED_GOAL;
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
  dist_G = distance();
  if (dist_G == 0 || dist_G > SONIC_THRESHOLD) {
    mode_B_IsFinished = true;
    Online_Mode_B = INIT;
  }
}

//基礎動作--------------------------------------------------------------
//Mode_C
//指定秒間ライントレースする
void pre_reset_Flag_C() {
  mode_C_IsFinished = false;
  resultId_C = INIT;
  mode_C_timePrev = timeNow_G;
}

void move_linetrace(unsigned long millis_time, int mode) {
  if (Online_Mode_C != LINE_TRACE) {
    Online_Mode_C = LINE_TRACE;
    pre_reset_Flag_C();
    linetrace_init();
  } else if (timeNow_G - mode_C_timePrev <= millis_time) {
    speed_diff = linetrace_P(mode);
  } else {
    mode_C_IsFinished = true;
    Online_Mode_C = INIT;
  }
}

//指定時間前進する
void move_forward(unsigned long millis_time) {
  if (Online_Mode_C != FORWARD) {
    pre_reset_Flag_C();
    Online_Mode_C = FORWARD;
    move_front_init();
  } else if (timeNow_G - mode_C_timePrev <= millis_time) {
    speed_diff = move_front();
  } else {
    mode_C_IsFinished = true;
    Online_Mode_C = INIT;
  }
}

void move_forward_of_direction(int direction_radius, unsigned long millis_time) {
  if (Online_Mode_C != FORWARD) {
    Online_Mode_C = FORWARD;
    pre_reset_Flag_C();
    move_front_init(direction_radius);
  } else if (timeNow_G - mode_C_timePrev <= millis_time) {
    speed_diff = move_front();
  } else {
    mode_C_IsFinished = true;
    Online_Mode_C = INIT;
  }
}

//指定時間後退する
void move_back2(unsigned long millis_time) {
  if (Online_Mode_C != BACK) {
    pre_reset_Flag_C();
    Online_Mode_C = BACK;

    move_back_init();
  } else if (timeNow_G - mode_C_timePrev <= millis_time) {
    speed_diff = move_back();
  } else {
    mode_C_IsFinished = true;
    Online_Mode_C = INIT;
  }
}


//指定角度まで回転する
void move_rotate(int direction) {
  if (Online_Mode_C != ROTATE) {
    pre_reset_Flag_C();
    Online_Mode_C = ROTATE;
    use_turnTo = true;
    speed0 = 0;
  } else if (timeNow_G - mode_C_timePrev < 1000) {
    speed_diff = turnTo(direction);
  } else {
    mode_C_IsFinished = true;
    Online_Mode_C = INIT;
    use_turnTo=false;
  }
}

//何秒回転するか
void move_rotate_with_millis(unsigned long millis_time, bool right_direction) {
  if (Online_Mode_C != ROTATE) {
    pre_reset_Flag_C();
    Online_Mode_C = ROTATE;

    speed0 = 0;
  } else if (timeNow_G - mode_C_timePrev < millis_time) {
    if (right_direction) {
      speed_diff = ROTATE_SPEED;
    } else {
      speed_diff = -ROTATE_SPEED;
    }
  } else {
    mode_C_IsFinished = true;
    Online_Mode_C = INIT;
  }
}


//指定時間停止する
void move_stop(unsigned long millis_time) {
  if (Online_Mode_C != STOP) {
    speed_diff = 0;
    pre_reset_Flag_C();
    Online_Mode_C = STOP;


    stop_init();
  } else if (timeNow_G - mode_C_timePrev <= millis_time) {
    speed_diff = stop();
  } else {
    mode_C_IsFinished = true;
  }
}
//----------------------------------------------------------------------------------

//ModeD 中間動作

int after_direction = 0;
//後退した後、指定角度足した向きを向く
void move_back_to_rotate() {

  switch (Online_Mode_D) {
    case INIT:
      mode_D_timePrev = timeNow_G;
      Online_Mode_D = BACK;
      mode_D_IsFinished = false;
      break;

    case BACK:
      move_back2(500);
      if (mode_C_IsFinished == true) {
        Online_Mode_D = ROTATE;
      }
      break;

    case ROTATE:
      move_rotate_with_millis(500, true);
      if (mode_C_IsFinished) {
        Online_Mode_D = INIT;
        mode_D_IsFinished = true;
      }
      break;

    default:
      Online_Mode_D = INIT;
      break;
  }
}
