# car


׷��С���ܹ���
Arduino�ײ���� <===> ����������ͨ��Э�飩 <====> �ֻ�ͨѶ����APP


Arduino�������룺
git@github.com:pkzju/car.git
�����ײ�������ͨ��Э����PIDģ��


Instruction

RoMeo BLE������V1.0:
http://wiki.dfrobot.com.cn/index.php?title=%28SKU:DFR0305%29RoMeo_BLE%E6%8E%A7%E5%88%B6%E5%99%A8V1.0_%E5%85%BC%E5%AE%B9Arduino

�������� Arduino IDE��
https://www.arduino.cc/en/Main/Software

Arduino IDE���ã�
ѡ��"����"->������ѡ��->"arduino/Genuino Uno"
ѡ��"����"->�˿�->��"COM4"                  
Ctrl+R ����
Ctrl+U �ϴ�

������ԣ�
http://www.arduino.cn/reference/


�����ܣ�

����
setup() ��ʼ������
{
	�����ʼ��
	
	�����ʼ��
	
	������ʼ��
	
	PID��ʼ��
}

loop()  ��ѭ������
{
	ͨ�Ŵ���  //Э�����
	
	���ƴ���  //100ms����
	
	��������  
}

�����Դ��ע��


���ԣ�
����PID��Ӧ����������Ŀ��ƣ�������������PID����

//x�� ���ҷ��� ���pid
double aggKp = 1.2, aggKi = 0.0, aggKd = 0.00;
double consKp = 1.3, consKi = 0.2, consKd = 0.00;

//y�� ���·���  ���pid
double aggKp_y = 0.1, aggKi_y = 0.20, aggKd_y = 0.005;
double consKp_y = 0.1, consKi_y = 0.20, consKd_y = 0.005;

//z�� ǰ���� ���pid
double aggKp_z = 1.3, aggKi_z = 0.0, aggKd_z = 0.0;
double consKp_z = 1.3, consKi_z = 0.0, consKd_z = 0.0;


���Ľ����飺
1. ����ϵͳ�ȶ��Բ���������ʹ����ݮ����ȡ���ֻ�������Ӳ������̶����ȶ�ϵͳ
2. ��ģ����������׷��״̬�£���Ҫ����׷�٣����滻����
3. ͼ�����ȶ��Բ���


