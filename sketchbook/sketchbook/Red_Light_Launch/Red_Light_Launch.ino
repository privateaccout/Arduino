#include <IRremote.h>                  // 引用 IRRemote 函式库
 
 
IRsend irsend;                          // 定义 IRsend 物件来发射红外线讯号
int RECV_PIN = 11;//定义红外接收器的引脚为11
IRrecv irrecv(RECV_PIN);
decode_results results;
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // 初始化红外接收器
  pinMode(3, OUTPUT);   
  digitalWrite(3, HIGH);   // 点亮LED测试（需通过手机摄像模式观察）
  delay(3000);             // 等待3秒
  digitalWrite(3, LOW);    // 结束测试
}
 
void loop()
{
  irsend.sendNEC(0xFF18E7, 32);   // 记得换成你遥控器的红外线编码
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);//以16进制换行输出接收代码
    Serial.println();//为了便于观看输出结果增加一个空行
    irrecv.resume(); // 接收下一个值
  }
  //delay(3000);             // 等待3秒
}
