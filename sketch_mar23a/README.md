# car


追踪小车架构：
Arduino底层控制 <===> 蓝牙（串行通信协议） <====> 手机通讯处理APP


Arduino完整代码：
git@github.com:pkzju/car.git
包括底层驱动、通信协议与PID模块


Instruction

RoMeo BLE控制器V1.0:
http://wiki.dfrobot.com.cn/index.php?title=%28SKU:DFR0305%29RoMeo_BLE%E6%8E%A7%E5%88%B6%E5%99%A8V1.0_%E5%85%BC%E5%AE%B9Arduino

开发环境 Arduino IDE：
https://www.arduino.cc/en/Main/Software

Arduino IDE设置：
选择"工具"->开发板选择->"arduino/Genuino Uno"
选择"工具"->端口->例"COM4"                  
Ctrl+R 编译
Ctrl+U 上传

编程语言：
http://www.arduino.cn/reference/


程序框架：

函数
setup() 初始化程序
{
	电机初始化
	
	舵机初始化
	
	按键初始化
	
	PID初始化
}

loop()  主循环函数
{
	通信处理  //协议解析
	
	控制处理  //100ms周期
	
	按键处理  
}

具体见源码注释


调试：
三组PID对应三个坐标轴的控制，调整好这三组PID即可

//x轴 左右方向 电机pid
double aggKp = 1.2, aggKi = 0.0, aggKd = 0.00;
double consKp = 1.3, consKi = 0.2, consKd = 0.00;

//y轴 上下方向  舵机pid
double aggKp_y = 0.1, aggKi_y = 0.20, aggKd_y = 0.005;
double consKp_y = 0.1, consKi_y = 0.20, consKd_y = 0.005;

//z轴 前后方向 电机pid
double aggKp_z = 1.3, aggKi_z = 0.0, aggKd_z = 0.0;
double consKp_z = 1.3, consKi_z = 0.0, consKd_z = 0.0;


待改进建议：
1. 整体系统稳定性不够，可以使用树莓派来取代手机，构成硬件软件固定的稳定系统
2. 车模运行在慢速追踪状态下，若要快速追踪，需替换大电机
3. 图像处理稳定性不够


