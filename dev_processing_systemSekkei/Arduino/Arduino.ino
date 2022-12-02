//#include <MsTimer2.h>
#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <LSM303.h>
#include <ZumoBuzzer.h>               // ブザーライブラリの読み込み


//ここからグローバル変数-------------------------------------------------------------

//Arduino.ino
#define TRIG 2
#define ECHO 4
#define DEG_TO_ANG 
#define DEFAULT_SPEED 100
#define ROBOT_NUM 0

#define FIELD_SIZE_W 2500
#define FIELD_SIZE_H 1600

#define DEG_TO_RAD(degree) degree*(M_PI/180)

#define WHITE    0 //白
#define BLACK    1 //黒
#define RED      2 //赤
#define BLUE     3 //青
#define OTHER    4 //その他
#define INIT    10 //初期状態
#define FORWARD 11 //直進
#define SEARCH  12 //回転
#define CATCH   13 //お宝取りに行く
#define RETURN  14 //お宝取得後、帰る
#define DIST_LINE 15
#define ROTATE 16
#define STOP    24 //停止
#define BACK    25 //後退
#define BACK2   26 //カップを外に出したあと、中を向く
#define FORWARD2 27 //カップを検知しているときの前進処理
#define KEISOKU 28 //カップが移動しているかどうかを読み取る
#define ROTATE2 29 //真後ろを向く




const int buzzerPin = 3;              // ブザーは 3 番ピン


ZumoMotors motors_G;
Pushbutton button(ZUMO_BUTTON);
LSM303 compass;
ZumoBuzzer buzzer;                    // ZumoBuzzer クラスのインスタンスを生成

int timeNow_G;
int heading_G;//現在向いている角度(磁気センサー)
int heading_G2; //敵のゴールを0とした時の角度(左回りが正)
int start_heading_G; //敵側の角度
int MotorR_G=0,MotorL_G=0; //モーターにかけている速度
int dist_G =0;//超音波センサーで検知した距離
float r_G, g_G, b_G; //カラーセンサーの格納値
float mx=0, my=0, mz=0;//地磁気センサーの値
float ax=0, ay=0, az=0;//ロボットの加速度
float real_a;
float now_speed = 0;//ロボットの速度
float speed100 = 104; //speed100で1秒間に進める距離(mm/s)
float speed0=0; //現在の速度
unsigned int turnTimePrev = 0;

float now_Pos[2] = {0,1600/2};//x,y

//move.ino---------------------------------------------------------------------
unsigned int moveTimePre = 0;
float move_timeStart_G = -1;
float move_thetaPrev_G;
int Online_Mode_A = INIT;
int Online_Mode_B = INIT;
int Online_Mode_C = INIT;

bool online_change_flag = false;

unsigned int online_time_prev;

//color.ino--------------------------------------------------------------------
unsigned int r_min, g_min, b_min; // このグローバル変数はこのファイル内のみで使用
unsigned int r_max, g_max, b_max;

//NN関数のために新たに追加する項目
#define max_colors 4
unsigned int ave_colors[max_colors][3]={
  {4,6,11},  //黒色
  {255,255,255},//白色
  {178,0,0}, //赤
  {0,16,121} //青
};

//collision.ino
float collision_bf_angle; //前回の角度
float collision_rc;
float collision_prev_dist;

//linetrace2.ino----------------------------------------------------------------



int keisoku_flag;

int goal,start;//ゴールは自軍向き//スタートは敵軍向き
int color;
int move_time, rotate_time=3000, change_time;
float start_time;
int pre_dist;

//speed.ino============---------------------------------------------------------
unsigned int speed_pos_prev_time=0;





//グローバル変数ここまで----------------------------------------------------------
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
  setStartDirection();
  speed_pos_prev_time = millis();
}

void loop() {
  timeNow_G = millis();
  //カラーセンサーの値を取得
  //消さないでください
  dist_G = distance();
  if (timeNow_G-turnTimePrev<100) {
    return;
  }
  timeNow_G = millis();
  calPos();
  //カラーセンサーの値を取得
  getRGB(r_G, g_G, b_G);
  getCompass();
  recvTusin();
  mover();

  //task();
  //printMe();

  //消さないでください
  turnTimePrev= timeNow_G;
}
