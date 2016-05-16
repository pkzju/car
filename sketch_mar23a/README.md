# car

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



