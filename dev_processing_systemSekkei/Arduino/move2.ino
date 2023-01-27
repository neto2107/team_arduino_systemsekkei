
void modeReseter() {
  Online_Mode_A = INIT;
  Online_Mode_B = INIT;
  Online_Mode_C = INIT;
  Online_Mode_D = INIT;

  keisoku_flag = false;
  mode_B_IsFinished = false;
  mode_C_IsFinished = false;
  mode_D_IsFinished = false;

  resultId_A = INIT;
  resultId_B = INIT;
  resultId_C = INIT;
  resultId_D = INIT;

  mode_A_time_set = 0;


  speed0 = 0;
  speed_diff = 0;
  moveTimePre = timeNow_G;
  resetPos();
  speed_reset();
  direction_time = 1000;
  //use_turnTo = false;
}

void modeChanger() {
  switch (Online_Mode_A) {

    case INIT:
      modeReseter();
      if (ROBOT_NUM == 0) {
        Online_Mode_A = FIRST_MOVING;
        mode_A_time_set = 10000;

      } else {
        Online_Mode_A = FIRST_MOVING;
      }
      break;

    case SEARCH:
      //buzzer.play("!L16 c");
      move_ditecting(10000);
      //buzzer.play("!L16 fg");
      //ラインを感知したら
      now_color_id = Nearest_Neighbor();
      if (now_color_id != WHITE) {          //ラインを感知したら
        set_back_with_return_fild(SEARCH);  //中断してライン内に戻す
        reset_Flag_B();
      }
      //Modebが終了したら
      if (mode_B_IsFinished == true) {
        if (resultId_B == DISCOVERY) {
          Online_Mode_A = SEARCH2;

        } else if (resultId_B == NOT_DISCOVERY) {
          Online_Mode_A = SEARCH;
          mode_A_time_set = 10000;
        }
      }
      break;

      // case MEANDERING_DRIVING: //蛇行運転をしながら探す

      //   meandering_driving(mode_A_time_set); //要ライン外に出たときのプログラム
      //   if (mode_B_IsFinished == true) {
      //     mode_A_time_set = 0; //リセット
      //     if (resultId_B == DISCOVERY) {
      //       Online_Mode_A = SEARCH2;

      //     } else if (resultId_B == NOT_DISCOVERY) {
      //       Online_Mode_A = MEANDERING_DRIVING;
      //       mode_A_time_set = 10000;
      //     }
      //   }
      //   break;

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
      if (now_color_id != WHITE) {  //ラインを感知したら
        set_back_with_return_fild(SEARCH);
      }
      break;

    case BACK_TO_GOAL:
      //buzzer.play("!L16 f");
      back_to_goal();
      if (mode_B_IsFinished == true) {
        Online_Mode_A = SEARCH;
      }
      break;

    case BACK_WIGH_RETURN_GOAL:
      move_back_to_rotate();
      if (mode_D_IsFinished) {
        modeReseter();
        Online_Mode_A = Online_Mode_A_pre;
        Online_Mode_A_pre = INIT;
      }
      break;

    case FIRST_MOVING:  //コート中央に行く //両サイド
      goToCenter();
      if (mode_B_IsFinished) {
        Online_Mode_A = FIRST_MOVING2;
      }
      break;

    case FIRST_MOVING2:             //中心に向く
      if (ROBOT_NUM == 1) {         //自ゴールから見て右
        move_rotate(270);           //左を向く
      } else if (ROBOT_NUM == 2) {  //自ゴールから見て左
        move_rotate(90);            //右をむく
      }
      if (mode_C_IsFinished == true) {
        Online_Mode_A = SEARCH;
      }
      break;


    default:
      buzzer.play("!L16 g");
      Online_Mode_A = INIT;
      break;
  }
  //mode_A_IsFinished = false;
}

void set_back_with_return_fild(int pre_mode) {
  Online_Mode_A_pre = pre_mode;
  Online_Mode_A = BACK_WIGH_RETURN_GOAL;
}
//応用動作---------------------------------------------------------------------
//Mode B
int ditecting_direction;  //次に回転する角度

void reset_Flag_B() {
  mode_B_timePrev = timeNow_G;
  mode_C_timePrev = timeNow_G;
  mode_B_IsFinished = false;
  mode_C_IsFinished = false;
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
  static unsigned long time_dist;
  switch (Online_Mode_B) {
    case INIT:
      reset_Flag_B();
      Online_Mode_B = SEARCH;
      break;
    case SEARCH:  //直進、回転を繰り返す。
      //serachSonicSensor = false;
      move_forward(1000);
      if (mode_C_IsFinished == true) {
        Online_Mode_B = SEARCH2;
        direction_time = random(2000, 3500);
      }
      break;

    case SEARCH2:

      //move_rotate_with_millis(direction_time, true);
      move_stop_and_rotate_with_millis(direction_time, true, 150, 150);
      if (mode_C_IsFinished == true) {
        Online_Mode_B = SEARCH;
      }
      break;

    case SEARCH3:
      time_dist = move_ditect_center(1000);
      if (time_dist == 0) {  //タイムアウトしたとき
        stop_init();
        resultId_B = NOT_DISCOVERY;
        Online_Mode_B = INIT;
        mode_B_IsFinished = true;
      }
      if (time_dist > 1) {  //カップのセンシング時間を取得できたとき
        stop_init();
        Online_Mode_B = SEARCH4;
      }
      break;

    case SEARCH4:
      move_rotate_with_millis(time_dist, true);
      if (mode_C_IsFinished == true) {
        resultId_B = DISCOVERY;
        mode_B_IsFinished = true;
      }
      break;
  }
  if (dist_G < SONIC_THRESHOLD && dist_G > 0 && Online_Mode_B == SEARCH2) {  //発見したら
    resultId_B = DISCOVERY;
    mode_B_IsFinished = true;
    Online_Mode_B = INIT;
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
      if (timeNow_G - mode_B_timePrev <= 1000) {  //1秒間計測
        stop_init();
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
        move_forward(2000);

        if (dist_G <= 4) {  //キャッチしたと判断したときは
          move_forward_low_speed(200); //少し前進して
          if (mode_C_IsFinished) {
            stop_init();  //停止する
            resultId_B = CATCH_SUCCESS;
            Online_Mode_B = INIT;      //キャッチが成功
            mode_B_IsFinished = true;  //遷移モードにする
          }
        }
      } else {
        //途中で見失ってしまったときは
        stop_init();  //停止させる
        resultId_B = CATCH_FAIED;
        Online_Mode_B = INIT;      //キャッチが失敗
        mode_B_IsFinished = true;  //遷移モードにする
      }
      break;
  }
}


//ゴールに帰ってくるときの動作
void back_to_goal() {
  static bool isRight;  //回転方向のフラグ
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
      now_color_id = Nearest_Neighbor();
      if (now_color_id != WHITE) {
        stop_init();
        reset_Flag_C();
        Online_Mode_B = BACK_TO_GOAL3;
        delay(100);
      }
      break;

    case BACK_TO_GOAL3:
      now_color_id = Nearest_Neighbor();
      if (now_color_id == RED || now_color_id == BLUE) {
        Online_Mode_B = REACHED_GOAL3;

      } else if (now_color_id == BLACK) {
        if (heading_G2 >= 0 && heading_G2 < 180) isRight = true;
        else if (heading_G2 >= 180, heading_G2 < 360) isRight = false;
        Online_Mode_B = LINE_TRACE;
      }

      break;

    case REACHED_GOAL:  //ゴールに到達したら
      //2秒間ライントレース
      move_linetrace(1500, isRight);
      if (mode_D_IsFinished == true) {
        Online_Mode_B = REACHED_GOAL2;
        end_reset_flag_D();
      }

      break;

    case REACHED_GOAL2:
      move_rotate_with_millis(1500, !isRight);
      buzzer.play("!L16 c");
      if (mode_C_IsFinished == true) {
        Online_Mode_B = REACHED_GOAL3;
      }
      break;

    case REACHED_GOAL3:
      move_forward(250);
      if (mode_C_IsFinished == true) {
        stop_init();
        Online_Mode_B = REACHED_GOAL4;
      }
      break;

    case REACHED_GOAL4:
      move_back2(1000);
      if (mode_C_IsFinished == true) {
        Online_Mode_B = INIT;
        mode_B_IsFinished = true;  //動作終了
      }
      break;

    case LINE_TRACE:
      buzzer.play("!L16 d");
      move_linetrace(20000, isRight);

      if (identifyColor(190, 120, 120) || identifyColor(80, 140, 150)) {
        Online_Mode_B = REACHED_GOAL;
        end_reset_flag_D();
      }
      break;

    default:
      break;
  }
  dist_G = distance();
  if (dist_G == 0 || dist_G > SONIC_THRESHOLD) {
    mode_B_IsFinished = true;
    Online_Mode_B = INIT;
  }
}

//蛇行運転のプログラム
void meandering_driving(unsigned long millis_time) {
  switch (Online_Mode_B) {
    case INIT:
      reset_Flag_B();
      Online_Mode_B = MEANDERING_DRIVING;
      move_meandering_driving_init();
      Serial.println("INIT");
      break;

    case MEANDERING_DRIVING:
      speed_diff = move_meandering_driving();
      dist_G = distance();
      now_color_id = Nearest_Neighbor();
      if (now_color_id != WHITE) {
        stop_init();
        stop();
        Online_Mode_B = BACK;
      }
      break;

    case BACK:  //ラインに触れたときにバックして回転する
      move_back_to_rotate();
      if (mode_D_IsFinished) {
        reset_Flag_C();
        Online_Mode_B = INIT;
      }
      break;
    default:
      reset_Flag_B();
      Online_Mode_B = INIT;
      break;
  }
  //buzzer.play("!L16 c");

  //Serial.print("distance:");
  //Serial.println(dist_G);
  if (dist_G > 0) {  //発見したら
    move_stop(10000);
    resultId_B = DISCOVERY;
    Online_Mode_B = INIT;
    mode_B_IsFinished = true;
  }
  if (timeNow_G - mode_B_timePrev > millis_time) {  //発見できなかったら
    resultId_B = NOT_DISCOVERY;
    Online_Mode_B = INIT;
    mode_B_IsFinished = true;
  }
}

void goToCenter() {  ////////////////////////////////////////////////////////////////////////////////////////////////
  switch (Online_Mode_B) {
    case INIT:
      reset_Flag_B();
      Online_Mode_B = ROTATE;


    case ROTATE:
      move_rotate(0);
      if (mode_C_IsFinished) {
        Online_Mode_B = FORWARD;
      }
      break;


    case FORWARD:
      if (ROBOT_NUM == 1) {
        
      } else if(ROBOT_NUM == 2) {
        move_forward_high_speed(2000);  //時間は要調整
      }
      if (mode_C_IsFinished == true) {
        stop_init();
        stop();
        Online_Mode_B = INIT;
        mode_B_IsFinished = true;
      }
      break;
  }
}



//ModeD 中間動作

int after_direction = 0;
void pre_reset_flag_D() {
  mode_D_timePrev = timeNow_G;
  mode_D_IsFinished = false;
}
void end_reset_flag_D() {
  Online_Mode_D = INIT;
  mode_D_IsFinished = false;
}
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

void move_linetrace(unsigned long millis_time, bool right_rotate) {

  switch (Online_Mode_D) {
    case INIT:
      pre_reset_Flag_C();
      pre_reset_flag_D();

      Online_Mode_D = ROTATE;
      speed_diff = stop_init();
      break;
    case ROTATE:
      move_rotate_with_millis(10000, right_rotate);
      now_color_id = Nearest_Neighbor();
      if (now_color_id == WHITE) {
        speed_diff = stop_init();
        linetrace_init();
        Online_Mode_D = LINE_TRACE;
      }
      break;
    case LINE_TRACE:
      speed_diff = linetrace_P(right_rotate);
      if (timeNow_G - mode_D_timePrev > millis_time) {
        mode_D_IsFinished = true;
        Online_Mode_D = INIT;
        reset_Flag_C();
        //speed_diff = stop_init();
      }
      break;
  }
}

unsigned long move_ditect_center(unsigned long time_out) {
  static unsigned long dist_time = 0;  //センサーが検知してから見失うまでの時間
  dist_G = distance();
  switch (Online_Mode_D) {
    case SEARCH:
      if (dist_G >= 100 || dist_G == 0) {
        dist_time = timeNow_G - mode_D_timePrev;
        return dist_time;
      }
      //タイムアウト処理
      if (timeNow_G - mode_D_timePrev >= time_out) return 0;
      break;

    default:

      mode_D_IsFinished = false;
      Online_Mode_D = SEARCH;
      break;
  }
  return 1;
}
