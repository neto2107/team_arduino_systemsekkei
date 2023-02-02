//#include <MsTimer2.h>
#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <ZumoBuzzer.h>  // ブザーライブラリの読み込み
#include <ZumoIMU.h>
//黒　41,58,1
//赤 157,79,30
//青 41,75,66
//白 255,255,255
//黒2 98,127,75
//青2 109,150,129
//

//赤2 187,140,96 170,114,68

//ここからグローバル変数-------------------------------------------------------------

//Arduino.ino
#define TRIG 2
#define ECHO 4
#define DEFAULT_SPEED 100
#define LOW_SPEED 150
#define HIGH_SPEED 400
#define ROTATE_SPEED 100
#define ROBOT_NUM 1 //0 自ゴールから見て正面 , 1:自ゴールから見て右 2:自ゴールから見て左
#define LEFT_SPEED_OFFSET 1
#define RIGHT_SPEED_OFFSET 1


#define FIELD_SIZE_W 2500
#define FIELD_SIZE_H 1600

#define DEG_TO_RAD(degree) degree*(M_PI / 180)

#define SONIC_THRESHOLD 200  //超音波センサーの閾値

//色の定義
#define WHITE 0  //白
#define BLACK 1  //黒
#define RED 2    //赤
#define BLUE 3   //青
#define OTHER 4  //その他

//Modeの定義
//共通
#define INIT 10  //初期状態
//MODE C
#define FORWARD 11  //直進
#define SEARCH 12
#define CATCH 13   //お宝取りに行く
#define RETURN 14  //お宝取得後、帰る
#define DIST_LINE 15
#define ROTATE 16  //回転
#define LINE_TRACE 17
#define STOP 24      //停止
#define BACK 25      //後退
#define BACK2 26     //カップを外に出したあと、中を向く
#define FORWARD2 27  //カップを検知しているときの前進処理
#define KEISOKU 28   //カップが移動しているかどうかを読み取る
#define ROTATE2 29   //真後ろを向く
//MODE_B
#define BACK_TO_GOAL 30
#define REACHED_GOAL 31
#define REACHED_GOAL2 32
#define SEARCH2 33
#define SEARCH3 34


#define DISCOVERY 35
#define NOT_DISCOVERY 36
#define BACK_WIGH_RETURN_GOAL 37
#define KEISOKU_ROTATE_BACK 38
#define MEANDERING_DRIVING 39
#define FIRST_MOVING 40
#define FIRST_MOVING2 41
#define SEARCH4 42
#define REACHED_GOAL3 43
#define REACHED_GOAL4 44
#define BACK_TO_GOAL3 45
#define TURN1 46
#define END 47
#define BACK_TO_GOAL4 48
#define TURN_CUP 49
#define TURN_CUP2 50
#define DETACT_FAILD 100  //カップの計測が失敗した
#define DETACT_TRUE 101   //カップの計測が成功した
#define CATCH_SUCCESS 102
#define CATCH_FAIED 103
#define BACK_TO_GOAL2 104


//MODE A

//デバッグモードフラグ
//#define DEBUG_MODE




const int buzzerPin = 3;  // ブザーは 3 番ピン


ZumoMotors motors_G;
Pushbutton button(ZUMO_BUTTON);
ZumoIMU imu;
ZumoBuzzer buzzer;  // ZumoBuzzer クラスのインスタンスを生成

unsigned long timeNow_G;
int heading_G;                   //現在向いている角度(磁気センサー)
int heading_G2;                  //敵のゴールを0とした時の角度(左回りが正)
int start_heading_G;             //敵側の角度
int MotorR_G = 0, MotorL_G = 0;  //モーターにかけている速度
int dist_G = 0;                  //超音波センサーで検知した距離
float r_G, g_G, b_G;             //カラーセンサーの格納値
unsigned int now_color_id;       //現在の色の判定結果
float mx = 0, my = 0, mz = 0;    //地磁気センサーの値
float ax = 0, ay = 0, az = 0;    //ロボットの加速度
float real_a;
float now_speed = 0;   //ロボットの速度
float speed100 = 104;  //speed100で1秒間に進める距離(mm/s)
float speed0 = 0;      //現在の速度
unsigned long turnTimePrev = 0;
bool use_turnTo = false;
bool moving_flag=false;

bool serachSonicSensor = false;

float now_Pos[2] = { 0, 1600 / 2 };  //x,y

ZumoIMU::vector<int16_t> m_max; // maximum magnetometer values, used for calibration
ZumoIMU::vector<int16_t> m_min; // minimum magnetometer values, used for calibration

//move.ino---------------------------------------------------------------------
unsigned long moveTimePre = 0;
float move_timeStart_G = -1;
float move_thetaPrev_G;
int Online_Mode_A = INIT;
int Online_Mode_A_pre = INIT;
int Online_Mode_B = INIT;
int Online_Mode_C = INIT;
int Online_Mode_D = INIT;

int resultId_A = INIT;
int resultId_B = INIT;
int resultId_C = INIT;
int resultId_D = INIT;

unsigned long mode_A_time_set = 0;

bool mode_C_IsFinished = false;
bool mode_B_IsFinished = false;
bool mode_D_IsFinished = false;

bool ditect_flag = false; //カップを見つけたときのフラグ

//基本動作で使う時間変数

//応用動作で使う時間変数
unsigned long mode_B_timePrev;
unsigned long mode_C_timePrev;
unsigned long mode_D_timePrev;

unsigned long direction_time = 1000;



float speed_diff = 0;

//color.ino--------------------------------------------------------------------
unsigned int r_min, g_min, b_min;  // このグローバル変数はこのファイル内のみで使用
unsigned int r_max, g_max, b_max;

//NN関数のために新たに追加する項目
#define max_colors 5
//changed
// unsigned int ave_colors[max_colors][3] = {
//   { 255, 255, 255 },  //白色
//   { 4, 6, 11 },       //黒色
//   { 178, 0, 0 },      //赤
//   { 0, 16, 121 }      //青
// };
unsigned int ave_colors[max_colors][3] = {
  { 255, 255, 255 },  //白色
  { 37, 44, 2 },       //黒色
  { 179, 77, 35 },      //赤
  { 38, 55, 62 } ,     //青
  { 160,160,160} //OTHER
};
//collision.ino
float collision_bf_angle;  //前回の角度
float collision_rc;
float collision_prev_dist;

//linetrace2.ino----------------------------------------------------------------



int keisoku_flag;

int goal, start;  //ゴールは自軍向き//スタートは敵軍向き
int color;
unsigned long move_time, rotate_time = 3000, change_time;
float start_time;
int pre_dist;

//speed.ino============---------------------------------------------------------
unsigned long speed_pos_prev_time = 0;





//グローバル変数ここまで----------------------------------------------------------
void setup() {

  Serial.begin(9600);
  Wire.begin();

  //コンパスのセットアップ
  setupIMU();

  heading_G = 120;
  r_G = 1;
  g_G = 2;
  b_G = 3;


  //超音波センサーのセットアップ
  setupSonic();


#ifndef DEBUG_MODE
  calibrationCompass();
  buzzer.play("L16 cdegreg4");  // ブザーにて音楽を鳴らす
  //カラーセンサーのキャリブレーション
  button.waitForButton();       // Zumo buttonが押されるまで待機
  CalibrationColorSensor();     // カラーセンサーのキャリブレーション
  buzzer.play("L16 cdegreg4");  // ブザーにて音楽を鳴ら

  button.waitForButton();  // Zumo buttonが押されるまで待機

  // calibration();
  // buzzer.play("L16 cdegreg4");  // ブザーにて音楽を鳴ら

  // button.waitForButton();  // Zumo buttonが押されるまで待機
//  MsTimer2::set(500,calSpeed); //タイマー割り込みの間隔設定
//  MsTimer2::start();//タイマー割り込み開始
#endif
  speed_pos_prev_time = millis();
}

void loop() {
  timeNow_G = millis();
  //カラーセンサーの値を取得
  //消さないでください
  

  //リセット用
  if (button.isPressed()) {
    speed_reset();
    buzzer.play("L16 cdegreg4");  // ブザーにて音楽を鳴らす

    button.waitForButton();
    moveTimePre = timeNow_G;
    resetPos();
    setStartDirection();
    modeReseter();
    moving_flag =true;
  }

  if (timeNow_G - turnTimePrev < 50 && use_turnTo==true) {  //turnto に必要
    return;
  }
  if(moving_flag==false){
    return;
  }
  timeNow_G = millis();
  calPos();
  //カラーセンサーの値を取得
  getRGB(r_G, g_G, b_G);
  now_color_id = Nearest_Neighbor();
  dist_G = distance();
  getCompass();
  recvTusin();
  mover();
  //task();
  //printMe();

  //消さないでください
  turnTimePrev = timeNow_G;
}
