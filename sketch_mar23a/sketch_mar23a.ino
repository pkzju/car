#include <Servo.h>

#include "protocal.h" 
#include "PID_v1.h"

bool STartFlag = 0;//控制状态
int Lost_Count = 0;//位置丢失次数

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
const unsigned char ServoOfDown_MaxPos = 160;  //下舵机摆动最左角度
const unsigned char ServoOfDown_MinPos = 20;   //下舵机摆动最右角度
const unsigned char ServoOfDown_MidPos = 85;   //下舵机摆动中间角度
const unsigned char ServoOfUp_MaxPos = 50;     //上舵机摆动最上角度
const unsigned char ServoOfUp_MinPos = 10;     //上舵机摆动最上角度 10
const unsigned char ServoOfUp_MidPos = 40;     //上舵机摆动中间角度 55
unsigned char ServoOfDown_CurrentPos = ServoOfDown_MidPos;  // 下舵机当前角度
unsigned char ServoOfUp_CurrentPos = ServoOfUp_MidPos;      // 上舵机当前角度
const unsigned char ServODownNum = 9;
const unsigned char ServOUpNum = 10;
void setServoPos(const unsigned char num , unsigned char pos)//num = 9 :下面的舵机 ; num = 10 :上面的舵机
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
const int Motor_Max = 100;
const int Motor_Min = -100;

void motorSet(int a, int b) {

  if (a >= 0) {
    if (a > 0)
      a += 30;
    digitalWrite(M2, LOW);
  }
  else {
    a -= 30;
    digitalWrite(M2, HIGH);
  }
  if (b >= 0) {
    if (b > 0)
      b += 30;
    digitalWrite(M1, LOW);
  }
  else {
    b -= 30;
    digitalWrite(M1, HIGH);
  }
  if (a > Motor_Max) a = Motor_Max;
  if (a < Motor_Min) a = Motor_Min;
  if (b > Motor_Max) b = Motor_Max;
  if (b < Motor_Min) b = Motor_Min;
  analogWrite (PWM2, abs(a));
  analogWrite (PWM1, abs(b));
}



//Define Variables we'll be connecting to
double Setpoint_x = 0, Input_x, Output_x;
//Define the aggressive and conservative Tuning Parameters
double aggKp = 1.2, aggKi = 0.0, aggKd = 0.00;
double consKp = 1.3, consKi = 0.2, consKd = 0.00;
//Specify the links and initial tuning parameters
PID myPID_x(&Input_x, &Output_x, &Setpoint_x, consKp, consKi, consKd, DIRECT);

//Define Variables we'll be connecting to
double Setpoint_y = 0, Input_y, Output_y;
//Define the aggressive and conservative Tuning Parameters
double aggKp_y = 0.1, aggKi_y = 0.20, aggKd_y = 0.005;
double consKp_y = 0.1, consKi_y = 0.20, consKd_y = 0.005;
//Specify the links and initial tuning parameters
PID myPID_y(&Input_y, &Output_y, &Setpoint_y, consKp_y, consKi_y, consKd_y, DIRECT);

//Define Variables we'll be connecting to
double Setpoint_z = 0, Input_z, Output_z;
//Define the aggressive and conservative Tuning Parameters
//double aggKp_z = 0.5, aggKi_z = 0.0, aggKd_z = 0.2;
//double consKp_z = 0.5, consKi_z = 2.5, consKd_z = 0.2;
double aggKp_z = 1.3, aggKi_z = 0.0, aggKd_z = 0.0;
double consKp_z = 1.3, consKi_z = 0.0, consKd_z = 0.0;
//Specify the links and initial tuning parameters
PID myPID_z(&Input_z, &Output_z, &Setpoint_z, consKp_z, consKi_z, consKd_z, REVERSE);

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
  digitalWrite(LED, LOW);

  //turn the PID on
  myPID_x.SetMode(AUTOMATIC);
  myPID_y.SetMode(AUTOMATIC);
  myPID_z.SetMode(AUTOMATIC);
}

double pwm_l, pwm_r;

void loop(void) {

  /*~~~~~~~~~~~~~~~~~~~华丽的分割线~~~~~~~~~~~~~~~~~~~ */
  //通信
  while (Serial.available()) {
    unsigned char receive = Serial.read();
    unsigned char rx_buffer[10];
    unsigned char* pHeader;
    pHeader = ScanHeader(receive);
    if (( unsigned char*)0 != pHeader)
    {
      // Find Header Success!
      memcpy(rx_buffer, pHeader, sizeof(tHeader));
      tHeader* pData = (tHeader* ) rx_buffer;
      digitalWrite(13, HIGH);
      if ( pData->cmd  == CMD_ID_POSITION_INFO) {//位置有效
        Input_x = pData->p_x;
        Input_y = pData->p_y;
        Input_z = pData->p_z;
        STartFlag = 1;
        Lost_Count = 0;
      }
      else if (pData->cmd  == CMD_ID_POSITION_NO) {//位置丢失
        Lost_Count++;
        if (Lost_Count > 5) {//丢失次数过大，关闭控制
          STartFlag = 0;
          Lost_Count = 0;
        }
      }

    }
  }

  /*~~~~~~~~~~~~~~~~~~~华丽的分割线~~~~~~~~~~~~~~~~~~~ */
  //控制 周期100ms
  if (STartFlag) {

    //x轴 左右方向 电机pid
    double gap_x = abs(Setpoint_x - Input_x); //distance away from setpoint
    if (gap_x < 30)
    { 
      //we're close to setpoint, use conservative tuning parameters
      myPID_x.SetTunings(consKp, consKi, consKd);
    }
    else
    {
      //we're far from setpoint, use aggressive tuning parameters
      myPID_x.SetTunings(aggKp, aggKi, aggKd);
    }
    myPID_x.Compute();

    //y轴 上下方向  舵机pid
    double gap_y = abs(Setpoint_y - Input_y); //distance away from setpoint
    if (gap_y < 20)
    { 
      //we're close to setpoint, use conservative tuning parameters
      myPID_y.SetTunings(consKp_y, consKi_y, consKd_y);
    }
    else
    {
      //we're far from setpoint, use aggressive tuning parameters
      myPID_y.SetTunings(aggKp_y, aggKi_y, aggKd_y);
    }
    myPID_y.Compute();

    //z轴 前后方向 电机pid
    double gap_z = abs(Setpoint_z - Input_z); //distance away from setpoint
    if (gap_z < 20)
    { 
      //we're close to setpoint, use conservative tuning parameters
      myPID_z.SetTunings(consKp_z, consKi_z, consKd_z);
    }
    else
    {
      //we're far from setpoint, use aggressive tuning parameters
      myPID_z.SetTunings(aggKp_z, aggKi_z, aggKd_z);
    }
    myPID_z.Compute();

    //输出量限幅
    if (Output_z > 60)
      Output_z = 60;
    if (Output_z < -60)
      Output_z = -60;
      
    pwm_l = (Output_x + Output_z);  //左轮pwm  x轴与z轴的叠加
    pwm_r = (-Output_x + Output_z); //右轮pwm  x轴与z轴的叠加
    motorSet(pwm_l, pwm_r);
    
    setServoPos(10, Output_y + ServoOfUp_MidPos);//舵机输出
  }
  else {
    //关闭状态
    motorSet(0, 0);
  }

  /*~~~~~~~~~~~~~~~~~~~华丽的分割线~~~~~~~~~~~~~~~~~~~ */
  //按键
  adc_key_in = analogRead(7);        // 读取模拟口7的值
  key = get_key(adc_key_in);         //调用判断按键程序
  if (key >= 0 && key != oldkey)     // 判断是否有新键按下
  {
    delay(50);  // 延时去抖
    adc_key_in = analogRead(7);      // 再次读模拟口7
    key = get_key(adc_key_in);       //调用判断按键程序
    if (key >= 0 && key != oldkey)   // 确认有键按下
    {
      oldkey = key;
      switch (key)
      {
        case 0://S1 OK
          {
            if (!STartFlag) 
            {
              STartFlag = 1;     
            }
            else 
            {
              STartFlag = 0;
            }
            break;
          }
        case 1:  motorSet(45, 45); break;     //S2 OK
        case 2:  motorSet(50, 50); break;     //S3 OK
        case 3:  motorSet(-80, 80); break;     //S4 OK
        case 4:  motorSet(85, -85); break;     //S5 OK
        default: break;
      }
    }
  }
  /*~~~~~~~~~~~~~~~~~~~华丽的分割线~~~~~~~~~~~~~~~~~~~ */


}

