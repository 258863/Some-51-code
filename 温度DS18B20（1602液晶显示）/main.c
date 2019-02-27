#include <reg52.h>
#include <intrins.h>
#include <math.h>
#define uchar unsigned char
#define uint unsigned int
sbit DS = P2^2;      //����     //define interface
sbit RS = P1^0;   //����˿�
sbit RW = P1^1;
sbit EN = P2^5;
sbit DU = P2^6;
sbit WE = P2^7;
uint temp;             // variable of temperature
//uchar flag1;            // sign of the result positive or negative

void delay(uint count)      //delay
{
  uint i;
  while(count)
  {
    i=200;
    while(i>0)
    i--;
    count--;
  }
}


void dsreset(void)       //send reset and initialization command
{
  uint i;
  DS=0;
  i=103;				   //����������480us~960us
  while(i>0)i--;
  DS=1;					   //Ȼ���������ߣ���DS18B20������Ӧ�Ὣ��15us~60us����������
  i=4;					   //15us~60us�ȴ�
  while(i>0)i--;
  //while(DS);
}

bit tmpreadbit(void)       //read a bit
{
   uint i;
   bit dat;
   DS=0;i++;          //i++ for delay
   DS=1;i++;i++;
   dat=DS;
   i=8;while(i>0)i--;
   return (dat);
}

uchar tmpread(void)   //read a byte date
{
  uchar i,j,dat;
  dat=0;
  for(i=1;i<=8;i++)
  {
    j=tmpreadbit();
    dat=(j<<7)|(dat>>1);   //�������������λ����ǰ�棬�����պ�һ���ֽ���DAT��
  }
  return(dat);
}

void tmpwritebyte(uchar dat)   //write a byte to ds18b20
{
  uint i;
  uchar j;
  bit testb;
  for(j=1;j<=8;j++)      //һ���ֽ�8λ���ݣ�ÿ��ֻ��дһ��λ
  {
    testb=dat&0x01;  //���ν�dat��ÿһλ��ֵ��testb
    dat=dat>>1;
    if(testb)     //write 1
    {
      DS=0;
      i++;i++;  //����1us
      DS=1;
      i=8;while(i>0)i--;  //����60us
    }
    else
    {
      DS=0;       //write 0
      i=8;while(i>0)i--;    //����60us
      DS=1;
      i++;i++;
    }

  }
}

void tmpchange(void)   //ת���¶ȵĺ�������һ���¶ȣ�  //DS18B20 begin change
{
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);   //����ROM����  // address all drivers on bus
  tmpwritebyte(0x44);  //��һ����(�¶�ת������))  //  initiates a single temperature conversion
  //delay(100);
}

uint tmp()               //get the temperature
{
  float tt;
  uchar a,b;
/*******************���Ͷ�ȡ����******************/
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);      //����ROM����
  tmpwritebyte(0xbe);     //��ȡ���ݴ�����
/************************************************/
  a=tmpread();           //��ȡ�¶�ֵ��16λ���ȶ����ֽ�
  b=tmpread();           //�ٶ����ֽڵ�8λ
  temp=b;
  temp<<=8;             //two byte  compose a int variable
  temp=temp|a;           //temp��uint��2���ֽڵ�16λ
  tt=temp*0.0625;      //оƬ����ļ��㹫ʽ
  //��������Ĭ��R0��R1Ϊ11��Ҳ����12λ�ֱ��ʣ�Ҳ����1λ����0.0625���϶�.
  temp=tt*10+0.5;
  return temp;
}

void readrom()          //read the serial
{
  uchar sn1,sn2;
  dsreset();
  delay(1);
  tmpwritebyte(0x33);
  sn1=tmpread();
  sn2=tmpread();
}


void delay10ms()            //delay
  {
    uchar a,b;
    for(a=10;a>0;a--)
      for(b=60;b>0;b--);
   }

/******************************Һ������*********/
void cmg88()//�������
{
		DU = 1;
		P0 = 0X00;
		DU = 0;
		WE = 0;
}

void DelayUs2x(unsigned char t)
{
 	while(--t);
}

void DelayMs(unsigned char t)
{

 while(t--)
 {
     //������ʱ1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 }
}

 bit LCD_Check_Busy(void)
 {
	 P0 = 0xff;
	 RS = 0;
	 RW = 1;
	 EN = 0;
	 _nop_();
	 EN = 1;
	 return (bit)(P0 & 0x80);	  //ǿ��ȡ���λ
 }

 void LCD_Write_Com(unsigned char com)
 {
	 while(LCD_Check_Busy()); //æ��ȴ�
	 RS=0;
	 RW=0;
	 EN=1;
	 P0 = com;
	 _nop_();
	 EN=0;
 }

 void LCD_Write_Data(unsigned char Data)
 {
	 while(LCD_Check_Busy()); //æ��ȴ�
	 RS=1;
	 RW=0;
	 EN=1;
	 P0 = Data;
	 _nop_();
	 EN=0;
 }

 void LCD_Clear(void)
 {
	 LCD_Write_Com(0x01);
	 DelayMs(5);
 }

 void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s)
 {
	 if (y == 0)
	 {
		 	LCD_Write_Com(0x80 + x);     //��ʾ��һ��
	 }
	 else
	 {
	 		LCD_Write_Com(0xC0 + x);      //��ʾ�ڶ���
	 }
	 while (*s)
	 {
			LCD_Write_Data( *s);
			s ++;
	 }
 }

 void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data)
 {
	 if (y == 0)
	 {
	 	LCD_Write_Com(0x80 + x);
	 }
	 else
	 {
	 	LCD_Write_Com(0xC0 + x);
	 }
	 LCD_Write_Data( Data);
 }

 void LCD_Init(void)		//��̬��ʼ��
 {
   LCD_Write_Com(0x38);    /*��ʾģʽ����*/
   DelayMs(5);
   LCD_Write_Com(0x38);
   DelayMs(5);
   LCD_Write_Com(0x38);
   DelayMs(5);
   LCD_Write_Com(0x38);
   LCD_Write_Com(0x08);    /*��ʾ�ر�*/
   LCD_Write_Com(0x01);    /*��ʾ����*/
   LCD_Write_Com(0x06);    /*��ʾ����ƶ�����*/
   DelayMs(5);
   LCD_Write_Com(0x0C);    /*��ʾ�����������*/
   //LCD_Write_Com(0x0f);
 }

 void LCD_Init2(void)	//��̬��ʼ��
 {
	   LCD_Write_Com(0x38);    /*��ʾģʽ����*/
	   DelayMs(5);
	   LCD_Write_Com(0x38);
	   DelayMs(5);
	   LCD_Write_Com(0x38);
	   DelayMs(5);
	   LCD_Write_Com(0x38);
	   LCD_Write_Com(0x08);    /*��ʾ�ر�*/
	   LCD_Write_Com(0x01);    /*��ʾ����*/
	   LCD_Write_Com(0x06);    /*��ʾ����ƶ�����*/
	   DelayMs(5);
	   LCD_Write_Com(0x0C);    /*��ʾ�����������*/
	   LCD_Write_Com(0x07);
	   //LCD_Write_Com(0x80+0x10);
 }

void display(uint temp)			//1602Һ����ʾ����
{
   uchar q,w,e;
   uint yy = abs(temp);
   q = temp / 100;
   w = temp %100 / 10;
   e = temp % 10;
   /*LCD_Write_Char(6,1,q+'0');
   LCD_Write_Char(7,1,w+'0');
   LCD_Write_Char(8,1,'.');
   LCD_Write_Char(9,1,e+'0');*/
   LCD_Write_Char(6,1,q+48);   //ASCII����� 1�͡�1��������
   LCD_Write_Char(7,1,w+48);
   LCD_Write_Char(8,1,'.');
   LCD_Write_Char(9,1,e+48);	
}

void main()
{
  cmg88();//������ܣ�������
  LCD_Init();   //��̬��ʼ��
  LCD_Clear();//����
  LCD_Write_String(3,0,"temperature");	
  while(1)
  {
      tmpchange();
      display(tmp());
  }
}

