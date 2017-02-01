int photocellPin = 2;    //定义变量photocellsh=2，为电压读取端口
int ledPin = 12;   //定义变量ledPin=12，为led电平输出端口
int val = 0;       //定义val变量的起始值


void setup() { 
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  //使ledPin为输出模式
  digitalWrite(ledPin, HIGH);
} 

void loop() { 
  val = analogRead(photocellPin);    //从传感器读取值
  Serial.println(val);
//  if(val<=512){      //512=2.5V，想让传感器敏感一些的时候，把数值调高，想让传感器迟钝的时候把数值调低。
//     //当val小于512(2.5V)的时候，led亮。
//  }
//  else{
//    digitalWrite(ledPin, LOW);
//  }
}
