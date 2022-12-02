//#include <MsTimer2.h>
#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <LSM303.h>
#include <ZumoBuzzer.h>               // ブザーライブラリの読み込み

#define TRIG 2
#define ECHO 4
#define DEG_TO_ANG 
#define DEFAULT_SPEED 100

#define FIELD_SIZE_W 2500
#define FIELD_SIZE_H 1600


const int buzzerPin = 3;              // ブザーは 3 番ピン


ZumoMotors motors_G;
Pushbutton button(ZUMO_BUTTON);
LSM303 compass;
ZumoBuzzer buzzer;                    // ZumoBuzzer クラスのインスタンスを生成


int timeNow_G;
int heading_G;//現在向いている角度(磁気センサー)
int MotorR_G=0,MotorL_G=0; //モーターにかけている速度
int dist_G;//超音波センサーで検知した距離
float r_G, g_G, b_G; //カラーセンサーの格納値
float mx=0, my=0, mz=0;//地磁気センサーの値
float ax=0, ay=0, az=0;//ロボットの加速度
float real_a;
float now_speed = 0;//ロボットの速度
float speed100 = 1; //speed100で1秒間に進める距離(mm/s)
float speed0=0; //現在の速度
unsigned int turnTimePrev = 0;
int now_Pos[2] = {0,1600/4};//x,y


void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  //コンパスのセットアップ
  setupCompass();
  
  heading_G = 120;
  r_G = 1;
  g_G = 2;
  b_G = 3;

  //超音波センサーのセットアップ
  setupSonic();
  
  compass.m_min.x = 32767;  compass.m_min.y = 32767;  compass.m_min.z = 32767;
  compass.m_max.x = -32768;  compass.m_max.y = -32768;  compass.m_max.z = -32768;
  button.waitForButton(); // Zumo buttonが押されるまで待機
  calibrationCompass();
  buzzer.play("L16 cdegreg4");        // ブザーにて音楽を鳴らす
  //カラーセンサーのキャリブレーション
  button.waitForButton(); // Zumo buttonが押されるまで待機
  CalibrationColorSensor(); // カラーセンサーのキャリブレーション
  buzzer.play("L16 cdegreg4");        // ブザーにて音楽を鳴ら

  button.waitForButton(); // Zumo buttonが押されるまで待機

  calibration();
  buzzer.play("L16 cdegreg4");        // ブザーにて音楽を鳴ら

  button.waitForButton(); // Zumo buttonが押されるまで待機
//  MsTimer2::set(500,calSpeed); //タイマー割り込みの間隔設定
//  MsTimer2::start();//タイマー割り込み開始
}

void loop() {
  timeNow_G = millis();
  //カラーセンサーの値を取得
  if (timeNow_G-turnTimePrev<100) {
    return;
  }
  timeNow_G = millis();
  //カラーセンサーの値を取得
  getRGB(r_G, g_G, b_G);
  dist_G = distance();
  getCompass();
  recvTusin();
  mover();

  //printMe();
  turnTimePrev= timeNow_G;
}
