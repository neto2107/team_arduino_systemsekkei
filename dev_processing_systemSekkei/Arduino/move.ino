
unsigned int moveTimePre = 0;
void mover() {
  if (button.isPressed()) {

    motors_G.setSpeeds(0, 0);   
    buzzer.play("L16 cdegreg4");        // ブザーにて音楽を鳴らす
    button.waitForButton();
    moveTimePre = timeNow_G;
  }

  if(timeNow_G-moveTimePre<1000){
    motors_G.setSpeeds(0, 0);
    vx = 0;
    setAccelOffset();
  }else if (timeNow_G - moveTimePre < 3000) {
  motors_G.setSpeeds(200, 200);
  } else if(timeNow_G - moveTimePre < 4000) {
    motors_G.setSpeeds(0, 0);
    vx = 0;
    setAccelOffset();
  } else if (timeNow_G - moveTimePre < 9000) {
    motors_G.setSpeeds(100, 100);
  }else{
    motors_G.setSpeeds(0, 0);
    // vx = 0;
    // setAccelOffset();
  }
}
