
unsigned int moveTimePre = 0;
void mover() {
  if (button.isPressed()) {
    buzzer.play("L16 cdegreg4");        // ブザーにて音楽を鳴らす
    motors_G.setSpeeds(0, 0);
    button.waitForButton();
    moveTimePre = timeNow_G;

  }

  if (timeNow_G - moveTimePre < 2000) {
  motors_G.setSpeeds(100, 100);
  } else {
    motors_G.setSpeeds(0, 0);
  }
}
