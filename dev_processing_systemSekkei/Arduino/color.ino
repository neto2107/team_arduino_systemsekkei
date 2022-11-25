#include <Adafruit_TCS34725.h> // カラーセンサライブラリのヘッダーファイル
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_60X);

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

void  CalibrationColorSensor()
{
  unsigned long timeInit;
  unsigned int r, g, b, clr;

  tcs.begin(); // カラーセンサーのsetup
  
  motors_G.setSpeeds(60, 60); 

  r_min = 30000;
  g_min = 30000;
  b_min = 30000;
  r_max = 0;
  g_max = 0;
  b_max = 0;

  timeInit = millis();

  while ( 1 ) {
    tcs.getRawData(&r, &g, &b, &clr); // rowdataの取得
  
    if ( r < r_min ) r_min = r;
    if ( g < g_min ) g_min = g;
    if ( b < b_min ) b_min = b;
    if ( r > r_max ) r_max = r;
    if ( g > g_max ) g_max = g;
    if ( b > b_max ) b_max = b;

    if ( millis() - timeInit > 2000 )
      break;
  }
   motors_G.setSpeeds(0, 0);
}


void getRGB( float& r0, float& g0, float& b0) {
  unsigned int r, g, b, clr;

  tcs.getRawData(&r, &g, &b, &clr); // rowdataの取得
 
  r0 = map(r, r_min, r_max, 0, 255);
  g0 = map(g, g_min, g_max, 0, 255);
  b0 = map(b, b_min, b_max, 0, 255);

  if ( r0 < 0.0 ) r0 = 0.0;
  if ( r0 > 255.0 ) r0 = 255.0;
  if ( g0 < 0.0 ) g0 = 0.0;
  if ( g0 > 255.0 ) g0 = 255.0;
  if ( b0 < 0.0 ) b0 = 0.0;
  if ( b0 > 255.0 ) b0 = 255.0;
}

//k近傍法による色判別
int Nearest_Neighbor(){
  int count =0;
  int color = -1;
  float minDistance = 999999;
  float distance;

  for(int i = 0; i<max_colors; i++){
    distance = sqrt((r_G - ave_colors[i][0])*(r_G - ave_colors[i][0])+
    (g_G - ave_colors[i][1])*(g_G - ave_colors[i][1])+(b_G - ave_colors[i][2])*(b_G - ave_colors[i][2]));

    if(distance < minDistance){
      minDistance = distance;
      color = i;
    }
  }
  return color;
}

////k近傍法の色を自動で算出する
//void auto_avg(){
//  String colorList[] = {"Black","White","Red","Blue"};
//  Serial.println("start_auto_avarege");
//  for(int i = 0; i < max_colors; i++){
//    Serial.print("start");Serial.println(colorList[i]);
//    int tmp[3][3];
//    for(int j = 0; j <3 ; j++){
//      delay(3000); //2秒待機
//      Serial.println("get_data");
//      //値の読み取り

//      tmp[j][0] = r_G;
//      tmp[j][1] = g_G;
//      tmp[j][2] = b_G; 
//    }
//    Serial.print("cal_average");
//    int sum[3]{0,0,0};
//    for(int k = 0; k < 3; k++){
//      //合計値を計算
//      sum[0]+= tmp[k][0];
//      sum[1]+= tmp[k][1];
//      sum[2]+= tmp[k][2];
//    }
//    //平均を求めて代入
//    ave_colors[i][0] = sum[0]/3;
//    ave_colors[i][1] = sum[1]/3;
//    ave_colors[i][2] = sum[2]/3;
//    //色ごとの結果を表示
//      Serial.print("end");Serial.print(colorList[i]);
//      Serial.print("(R,G,B)");
//      Serial.print(ave_colors[i][0]);Serial.print(",");
//      Serial.print(ave_colors[i][1]);Serial.print(",");
//      Serial.print(ave_colors[i][2]);Serial.println("");
//  }
//   Serial.println("end_auto_avarege");
//}
