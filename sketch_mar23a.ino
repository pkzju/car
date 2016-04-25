#include <Servo.h>
#include <MsTimer2.h>
#include "protocal.h"


bool STartFlag = 0;
int Speed_Last = 0;
Position_Data pData_Now = { 0, 0, 0};
Position_Data pData_Old = { 0, 0, 0};
Position_Data pData_Old_Old = { 0, 0, 0};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~华丽的分割线~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//按键与LED灯
int LED = 13;
int adc_key_val[5] = {50, 200, 400, 600, 800 }; //定义一个数组 存放模拟键值比较值
int NUM_KEYS = 5;
int adc_key_in;
int key = -1;
int oldkey = -1;
// 该函数判断是哪个按键被按下，返回该按键序号
int get_key(unsigned int input) {
  int k;
  if (input > 800)      //确认没有键按下k =-1
    return -1;
  for (k = 0; k < NUM_KEYS; k++) {
    if (input < adc_key_val[k]) {    //循环对比比较值，判断是否有键按下，有返回键号
      return k;
    }
  }
  if (k >= NUM_KEYS)k = -1;          //确认没有键按下k =-1
  return k;
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~华丽的分割线~~~~~~~~~~~~~~~~~~~~~~~~~~ */


/*~~~~~~~~~~~~~~~~~~~~~~~~~~华丽的分割线~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//舵机参数
Servo ServoOfDown;  //创建一个舵机控制对象 下方舵机
Servo ServoOfUp;    //上方舵机
const unsigned char ServoOfDown_MaxPos = 160;  //上舵机摆动最下角度
const unsigned char ServoOfDown_MinPos = 20;   //上舵机摆动最上角度
const unsigned char ServoOfDown_MidPos = 85;   //上舵机摆动中间角度
const unsigned char ServoOfUp_MaxPos = 80;     //下舵机摆动最左角度
const unsigned char ServoOfUp_MinPos = 10;     //下舵机摆动最右角度
const unsigned char ServoOfUp_MidPos = 55;     //上舵机摆动中间角度
unsigned char ServoOfDown_CurrentPos = ServoOfDown_MidPos;  // 下舵机当前角度
unsigned char ServoOfUp_CurrentPos = ServoOfUp_MidPos;      // 上舵机当前角度
const unsigned char ServODownNum = 9;
const unsigned char ServOUpNum = 10;
void setServoPos(const unsigned char num , unsigned char pos)
{
  if (num == 9) {
    if (pos > ServoOfDown_MaxPos) {
      pos = ServoOfDown_MaxPos;
    }
    if (pos < ServoOfDown_MinPos) {
      pos = ServoOfDown_MinPos;
    }
    ServoOfDown.write(pos);
  }
  if (num == 10) {
    if (pos > ServoOfUp_MaxPos) {
      pos = ServoOfUp_MaxPos;
    }
    if (pos < ServoOfUp_MinPos) {
      pos = ServoOfUp_MinPos;
    }
    ServoOfUp.write(pos);
  }

}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~华丽的分割线~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*~~~~~~~~~~~~~~~~~~~~~~~~~~华丽的分割线~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//电机参数
int PWM1 = 5;     //定义M1的PWM
int PWM2 = 6;     //定义M2的PWM
int M1 = 4;       //定义M1正反控制端
int M2 = 7;       //定义M2正反控制使能端
void stop(void) {                //停止
  digitalWrite(PWM1, LOW);
  digitalWrite(PWM2, LOW);
}
void back_off(char a, char b) {         //前进
  analogWrite (PWM1, a);            //PWM调速
  digitalWrite(M1, HIGH);
  analogWrite (PWM2, b);
  digitalWrite(M2, HIGH);
}
void  advance(char a, char b) {         //后退
  analogWrite (PWM1, a);
  digitalWrite(M1, LOW);
  analogWrite (PWM2, b);
  digitalWrite(M2, LOW);
}
void turn_L (char a, char b) {          //左转
  analogWrite (PWM1, a);
  digitalWrite(M1, LOW);
  analogWrite (PWM2, b);
  digitalWrite(M2, HIGH);
}
void turn_R (char a, char b) {          //右转
  analogWrite (PWM1, a);
  digitalWrite(M1, HIGH);
  analogWrite (PWM2, b);
  digitalWrite(M2, LOW);
}
void motorSet(int a, int b) {
  if (a > 255) a = 255;
  if (a < -255) a = -255;
  if (b > 255) b = 255;
  if (b < -255) b = -255;
  if (a >= 0) {
    analogWrite (PWM2, a);
    digitalWrite(M2, LOW);
  }
  else {
    analogWrite (PWM2, abs(a));
    digitalWrite(M2, HIGH);
  }
  if (b >= 0) {
    analogWrite (PWM1, b);
    digitalWrite(M1, LOW);
  }
  else {
    analogWrite (PWM1, abs(b));
    digitalWrite(M1, HIGH);
  }
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~华丽的分割线~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void control() {
  int speed_add, __speed;
  float motor_p = 2;
  float motor_i = 5;
  float motor_d = 0.5;
  speed_add = motor_p * (pData_Now.p_x - pData_Old.p_x) + motor_i * pData_Now.p_x - \
              motor_d * (pData_Now.p_x - 2 * pData_Old.p_x  + pData_Old_Old.p_x) - (motor_d * (pData_Now.p_x - pData_Old.p_x) / 2);
  __speed = Speed_Last + speed_add;
  if (abs(pData_Now.p_x) < 10 && abs(__speed) < 20)
    __speed = 0;
  //  motorSet(__speed, __speed);
  Speed_Last = __speed;
  pData_Old.p_x = pData_Now.p_x;
  pData_Old.p_y = pData_Now.p_y;
  pData_Old.p_z = pData_Now.p_z;

  pData_Old_Old.p_x = pData_Old.p_x;
  pData_Old_Old.p_y = pData_Old.p_y;
  pData_Old_Old.p_z = pData_Old.p_z;

}
void controlStart() {
  MsTimer2::start();
}
void controlStop() {
  MsTimer2::stop();
  motorSet(0, 0);
}

void setup(void) {
  pinMode(M1, OUTPUT);  //电机1
  pinMode(M2, OUTPUT);  //电机2
  pinMode(LED, OUTPUT);  // LED灯
  Serial.begin(115200); //串口初始化
  Serial.println("Hello World!");
  ServoOfDown.attach(9);  //舵机
  ServoOfUp.attach(10);   //舵机
  ServoOfDown.write(ServoOfDown_MidPos);
  ServoOfUp.write(ServoOfUp_MidPos);
  MsTimer2::set(500, control); // 50ms period
}

void loop(void) {

  /*~~~~~~~~~~~~~~~~~~~华丽的分割线~~~~~~~~~~~~~~~~~~~ */
  //通信
  if(STartFlag){
  while (Serial.available()) {
    unsigned char receive = Serial.read();
    unsigned char rx_buffer[20];
    if (FrameUnpack(receive, rx_buffer)) {
      tHeader* pHeader = (tHeader*)rx_buffer;
      if ( pHeader->type  == 0) {
        Position_Data *pData = (Position_Data *)&
                               rx_buffer[sizeof(tHeader)];
        pData_Now.p_x = pData->p_x;
        pData_Now.p_y = pData->p_y;
        pData_Now.p_z = pData->p_z;
      }
      else if (pHeader->type  == 2) {
        if (pHeader->cmd == 0) {
          motorSet(0, 0);
        }
        else if (pHeader->cmd == 1) {
          motorSet(80, 80);
        }
        else if (pHeader->cmd == 2) {
          motorSet(-80, -80);
        } else if (pHeader->cmd == 3) {
          motorSet(-80, 80);
        } else if (pHeader->cmd == 4) {
          motorSet(80, -80);
        }
      }
    }
  }
  }
  /*~~~~~~~~~~~~~~~~~~~华丽的分割线~~~~~~~~~~~~~~~~~~~ */

  /*~~~~~~~~~~~~~~~~~~~华丽的分割线~~~~~~~~~~~~~~~~~~~ */
  //按键
  adc_key_in = analogRead(7);        // 读取模拟口7的值
  // digitalWrite(LED, LOW);
  key = get_key(adc_key_in);         //调用判断按键程序
  if (key >= 0 && key != oldkey) {     // 判断是否有新键按下
    delay(50);  // 延时去抖
    adc_key_in = analogRead(7);      // 再次读模拟口7
    key = get_key(adc_key_in);       //调用判断按键程序
    if (key >= 0 && key != oldkey) {   // 确认有键按下
      oldkey = key;
      //    digitalWrite(13, HIGH);
      switch (key) {
        case 0:
          {
            STartFlag = 1;     //S1 OK
            controlStart();
            break;
          }
        case 1:                //S2 OK
          {
            STartFlag = 0;
            controlStop();
            break;
          }
        case 2: motorSet(0, 0); break;        //S3 OK
        case 3: motorSet(50, 50); break;        //S4 OK
        case 4: motorSet(150, -150); break;        //S5 OK
        default: break;
      }
    }
  }
  /*~~~~~~~~~~~~~~~~~~~华丽的分割线~~~~~~~~~~~~~~~~~~~ */


  delay(15);
  /*  char pos;
    for(pos = 0; pos >= -35; pos -= 1)
    {
     setServoPos(ServODownNum, pos + ServoOfDown_MidPos);
     setServoPos(ServOUpNum, ServoOfUp_MidPos);
     delay(15);                       // 等待15ms让舵机到达指定位置
    }
    for(pos = -35; pos <= 35; pos += 1)
    {
     setServoPos(ServODownNum, pos + ServoOfDown_MidPos);
     setServoPos(ServOUpNum, ServoOfUp_MidPos);
     delay(15);                       // 等待15ms让舵机到达指定位置
    }
    for(pos = 35; pos >= 0; pos -= 1)
    {
     setServoPos(ServODownNum, pos + ServoOfDown_MidPos);
     setServoPos(ServOUpNum, ServoOfUp_MidPos);
     delay(15);                       // 等待15ms让舵机到达指定位置
    }
    delay(500);
      for(pos = 0; pos >= -35; pos -= 1)
    {
     setServoPos(ServODownNum, ServoOfDown_MidPos);
     setServoPos(ServOUpNum, pos + ServoOfUp_MidPos);
     delay(15);                       // 等待15ms让舵机到达指定位置
    }
    for(pos = -35; pos <= 35; pos += 1)
    {
     setServoPos(ServODownNum, ServoOfDown_MidPos);
     setServoPos(ServOUpNum, pos + ServoOfUp_MidPos);
     delay(15);                       // 等待15ms让舵机到达指定位置
    }
    for(pos = 35; pos >= 0; pos -= 1)
    {
     setServoPos(ServODownNum, ServoOfDown_MidPos);
     setServoPos(ServOUpNum, pos + ServoOfUp_MidPos);
     delay(15);                       // 等待15ms让舵机到达指定位置
    }*/

}

