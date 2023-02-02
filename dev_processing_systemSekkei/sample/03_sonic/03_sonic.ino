#include <MsTimer2.h>

void sonic(){
      digitalWrite(2, HIGH);
      delayMicroseconds(10);
      digitalWrite(2, LOW);
}

void  setup(){

  pinMode(1,OUTPUT);

  MsTimer2::set(1,sonic);
  MsTimer2::start();
}

void loop(){

}
