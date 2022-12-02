
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
  online_change_flag = false;
}

float move_forward(unsigned int millis_time){
  if(Online_Mode_C != FORWARD){
    online_time_prev = timeNow_G;
    Online_Mode_C = FORWARD;
    move_front_init();
  }else if(timeNow_G - online_time_prev <= millis_time){
    return move_front();
  }else{
    online_change_flag = true;
  }
}

float rotate(int direction){
  if(Online_Mode_C != ROTATE){
    online_time_prev = timeNow_G;
    Online_Mode_C = ROTATE;
  }else if(heading_G2 <= direction -3 || heading_G2 >= direction+3){
    return turnTo(direction);
  }else{
    online_change_flag = true;
  }   
}

float move_stop(unsigned int millis_time){
  if(Online_Mode_C != STOP){
    online_time_prev = timeNow_G;
    Online_Mode_C = STOP;
    stop_init();
  }else if(timeNow_G - online_time_prev <= millis_time){
    return stop();
  }else{
    online_change_flag = true;
  }
}
