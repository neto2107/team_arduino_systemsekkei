//基礎動作--------------------------------------------------------------
//Mode_C
//指定秒間ライントレースする
void pre_reset_Flag_C() {
  mode_C_IsFinished = false;
  resultId_C = INIT;
  mode_C_timePrev = timeNow_G;
}

void reset_Flag_C() {
  mode_C_IsFinished = false;
  resultId_C = INIT;
  mode_C_timePrev = timeNow_G;
  Online_Mode_C = INIT;
}



//指定時間前進する
void move_forward(unsigned long millis_time) {
  if (Online_Mode_C != FORWARD) {
    pre_reset_Flag_C();
    Online_Mode_C = FORWARD;
    move_front_init(false);
  } else if (timeNow_G - mode_C_timePrev <= millis_time) {
    speed_diff = move_front(false);
  } else {
    mode_C_IsFinished = true;
    Online_Mode_C = INIT;
  }
}

void move_forward_high_speed(unsigned long millis_time) {
  if (Online_Mode_C != FORWARD) {
    pre_reset_Flag_C();
    Online_Mode_C = FORWARD;
    move_front_init(true);
  } else if (timeNow_G - mode_C_timePrev <= millis_time) {
    speed_diff = move_front(true);
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
    speed_diff = move_front(false);
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
  } else if (timeNow_G - mode_C_timePrev < 1000 && timeNow_G - mode_C_timePrev > 0) {
    speed_diff = turnTo(direction);
  } else {
    mode_C_IsFinished = true;
    Online_Mode_C = INIT;
    use_turnTo = false;
  }
}

//何秒回転するか
void move_rotate_with_millis(unsigned long millis_time, bool right_direction) {
  if (Online_Mode_C != ROTATE2) {
    pre_reset_Flag_C();
    Online_Mode_C = ROTATE2;

    speed0 = 0;
  } else if (timeNow_G - mode_C_timePrev < millis_time && timeNow_G - mode_C_timePrev > 0) {
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


//一時停止しながら回転する
void move_stop_and_rotate_with_millis(unsigned long millis_time, bool isRight, unsigned long rotate_time, unsigned long stop_time) {
  static unsigned long judge_time;
  switch (Online_Mode_C) {
    case ROTATE:
      if (isRight) {
        speed_diff = ROTATE_SPEED;
      } else {
        speed_diff = -ROTATE_SPEED;
      }
      if (timeNow_G - mode_C_timePrev > judge_time) {
        Online_Mode_C = STOP;
        judge_time += stop_time;
      }
      break;

    case STOP:
      stop_init();
      if (timeNow_G - mode_C_timePrev > judge_time) {
        Online_Mode_C = ROTATE;
        judge_time += rotate_time;
      }
      break;
    default:
      pre_reset_Flag_C();
      Online_Mode_C = ROTATE;
      judge_time = rotate_time;
      break;
  }
  if (timeNow_G - mode_C_timePrev > millis_time) {
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
    Online_Mode_C = INIT;
  }
}
//----------------------------------------------------------------------------------