#include <Servo.h> 
Servo myservo;

// IS_Right = 1 -----> Right
#define MAX_RECV 20
#define TURN_VALUE 60      //cm
#define AJUST_VALUE 2       //cm
#define STAY_VALUE 15
#define Init_Value_L 90
#define Init_Value_R 15
#define TURN_LEN 4
int Split(double *A,int Low,int High);
void Sort(double *A,int Low,int High);
double Get_MidNum(double *distance);
void Init_Data();

const int LED_R = 12;
const int LED_L = 10;

const int TrigPin_H = 7;
const int EchoPin_H = 8;

const int TrigPin_L = 5;
const int EchoPin_L = 6;

const int TrigPin_R = 3;
const int EchoPin_R = 4;

const int Servo_Pin = 9;


int Turn_Right[TURN_LEN] = {80,90,100,90};
int Turn_Left[TURN_LEN] = {100,90,80,90};
int Ajust_Right[4] = {80,90,100,90};
int Ajust_Left[4] = {100,90,80,90};



double distance_L;
double distance_R;
double distance_Wall;
double distance_Block;

double New_Mid_distance_L;
double New_Mid_distance_R;

double Old_Mid_distance;

double Old_Mid_distance_L;
double Old_Mid_distance_R;

double Delta_distance_L;
double Delta_distance_R;
boolean Is_Right = true;
//boolean Block_Is_Right[14] = {0,1,1,0,1,0,1,0,1,1,1,0,1,0};


//double debug;


void setup() {
  Serial.begin(9600);
  myservo.attach(Servo_Pin);
  pinMode(Servo_Pin,OUTPUT);
  pinMode(TrigPin_L,OUTPUT);
  pinMode(EchoPin_L,INPUT);
  pinMode(TrigPin_R,OUTPUT);
  pinMode(EchoPin_R,INPUT);
  pinMode(LED_R,OUTPUT);
  pinMode(LED_L,OUTPUT);
  digitalWrite(LED_R,HIGH);
  digitalWrite(LED_L,HIGH);
  delay(100);
  digitalWrite(LED_R,LOW);
  digitalWrite(LED_L,LOW);  
  delay(100);
  Init_Data();
}

void Init_Data(){
  myservo.write(90);
  if(Is_Right){
    distance_L  = Init_Value_L;
    distance_R = Init_Value_R;
  }else{
    distance_L  = Init_Value_R;
    distance_R = Init_Value_L;
  }
  Old_Mid_distance_L = 90;
  Old_Mid_distance_R = STAY_VALUE;
  Serial.println("Init Successful !");
}


void Get_Data(int Trig_Pin,int Echo_Pin,double* Save_Num){
  digitalWrite(Trig_Pin,LOW);
  delayMicroseconds(2);
  digitalWrite(Trig_Pin,HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_Pin,LOW);
  *Save_Num = pulseIn(Echo_Pin,HIGH)/58.00;//cm
}

void Take_Turn(){
  int Turn_Array[TURN_LEN] = {0,0,0,0};
  if(Is_Right){
    for(int i=0;i<TURN_LEN;i++){
      Turn_Array[i] = Turn_Left[i];
    }
  }else{
    for(int i=0;i<TURN_LEN;i++){
      Turn_Array[i] = Turn_Right[i];
    }
  }
  for(int i=0;i<TURN_LEN;i++){
    myservo.write(Turn_Array[i]);
    Serial.println("one step");
    delay(1000);
  }
}




void Handle_Data(){
  New_Mid_distance_L = distance_L;
  New_Mid_distance_R = distance_R;
  Serial.print("R :");
  Serial.print(New_Mid_distance_R);
  Serial.print(" --- L :");
  Serial.println(New_Mid_distance_L);
  if(Is_Right){
    distance_Block = New_Mid_distance_L;                              // distance_Block 靠近桩一边传感器的距离变量
    distance_Wall = New_Mid_distance_R;                               // distance_Wall  靠近墙一边传感器的距离变量
  }else{
    distance_Wall = New_Mid_distance_L;
    distance_Block = New_Mid_distance_R;
  }
  if (abs(distance_Block - Old_Mid_distance_L) >= TURN_VALUE){          // abs(x) 获取x的绝对值
    Serial.print("prior :");
    Serial.print(Old_Mid_distance_L);
    Serial.print(" --- now : ");
    Serial.println(distance_Block);
    
    Serial.println("Turn");
    Take_Turn();
    digitalWrite(LED_L,HIGH);
    delay(3000);
    digitalWrite(LED_L,LOW);
    Init_Data();
    
    return;
  }
  if (distance_Wall - STAY_VALUE >= AJUST_VALUE){
      Serial.println("Ajust");
      digitalWrite(LED_R,HIGH);
      delay(1000);
      digitalWrite(LED_R,LOW);
      //Ajust(true,distance_Wall);
  }
  if (distance_Wall - STAY_VALUE <= -1*AJUST_VALUE){
      Serial.println("Ajust");
      digitalWrite(LED_R,HIGH);
      delay(1000);
      digitalWrite(LED_R,LOW);
      //Ajust(false,distance_Wall);
  }
  Old_Mid_distance_L = New_Mid_distance_L;
  Old_Mid_distance_R = New_Mid_distance_R;
}



void Control_Pin(){

    unsigned long S_time = millis();
    
    Get_Data(TrigPin_L,EchoPin_L,&distance_L);
    Get_Data(TrigPin_R,EchoPin_R,&distance_R);
    unsigned long E_time = millis();
//    Serial.print("left : ");
//    Serial.print(distance_L);
//    Serial.print("    right : ");
//    Serial.print(distance_R);
//    Serial.print("    time : ");
//    Serial.println(E_time-S_time);
//  Serial.print("left : ");
//  Serial.print(distance_L[i]);
//  Serial.print(" ----- right : ");
//  Serial.println(distance_R[i]);
}
void loop() {
  //digitalWrite(LED_R,LOW);
  //digitalWrite(LED_L,LOW); 
  unsigned long start_time = millis();

  Control_Pin();
    
  Handle_Data();
  unsigned long end_time = millis();
  //Serial.println(end_time-start_time);
  
  //digitalWrite(LED_R,HIGH);
  //digitalWrite(LED_L,HIGH);
}
