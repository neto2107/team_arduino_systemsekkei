//PCICR  |= (1 << PCIE0);


#define TRIG 2
#define ECHO 4

volatile unsigned long high_time = 0;
volatile unsigned long low_time = 0;
volatile int sonicTrig = 1;
float distance = 0;

void setupSonic(){
  PCICR  |= B00000100;
  PCMSK2 |= B00010000;
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
}

ISR(PCINT2_vect){
  unsigned long time = micros();
  if(digitalRead(ECHO) == 1){
    high_time = time;
  }else{
    low_time = time;
    sonicTrig = 1;
  }
  
}

void setup()
{
  setupSonic();
  Serial.begin(9600);
}

void loop()
{
  if(sonicTrig == 1){
    digitalWrite(TRIG,HIGH);
    sonicTrig = 0;
  }
  delayMicroseconds(10);
  digitalWrite(TRIG,LOW);
  float tempDistance = (low_time - high_time) * 0.0173375;
  if(tempDistance < 400.0){
    distance = tempDistance;
  }

  Serial.print("distance:");
  Serial.println(distance);
  delay(20);

}