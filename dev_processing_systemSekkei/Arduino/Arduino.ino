

int timeNow_G;
int heading_G;//現在向いている角度(磁気センサー)
int r_G,g_G,b_G; //カラーセンサーの格納値
void setup(){
    Serial.begin(9600);
    heading_G = 120;
    r_G = 1;
    g_G = 2;
    b_G = 3;
}

void loop(){
    timeNow_G = millis();
    recvTusin();
}