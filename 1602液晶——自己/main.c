#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit RS = P1^0;   //����˿�
sbit RW = P1^1;
sbit EN = P2^5;
sbit DU = P2^6;
sbit WE = P2^7;
uint x,y;
uchar num,num_m;

void cmg88()//�������
{
		DU = 1;
		P0 = 0X00;
		DU = 0;
		WE = 0;
}
/*------------------------------------------------
 uS��ʱ����������������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��,������ʱ
 �������� T=tx2+5 uS
------------------------------------------------*/
void DelayUs2x(unsigned char t)
{
 	while(--t);
}
/*------------------------------------------------
 mS��ʱ����������������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���12M����ȷ��ʱ��ʹ�û��
------------------------------------------------*/
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

void delay(uint z)
{
	uint x,y;
	for(x=z;z>0;z--)
		for(y=112;y>0;y--);
}

#if 1
void main(void)
{
	cmg88();//������ܣ�������
	LCD_Init2();   //��̬��ʼ��
	LCD_Clear();//����
	while (1)
	{
	   	 //LCD_Write_Char(7,0,'o');
	 	 //LCD_Write_Char(8,0,'k');
	 	 //LCD_Write_String(1,1,"hello world");
		 for(num = 40;num > 0;num--)
		 {
		 	 LCD_Write_String(num,0," YYY AND YJG ");
		     LCD_Write_String(num+2,1," FOREVER ");
			 if(num == 24)
			 {
			 	LCD_Write_String(6,0," YYY AND YJG ");
		    	LCD_Write_String(8,1," FOREVER ");
				delay(5000);
				LCD_Write_Com(0x01);	  
			    for(num_m = 24;num_m > 0;num_m--)
		 		{		
					    LCD_Write_String(num_m,0," YYY AND YJG ");
						LCD_Write_String(num_m+2,1," FOREVER ");
						delay(500);
						LCD_Write_Com(0x01);
						delay(50);	  
				}
			 }
			 else
			 {
			 	 delay(500);
             	 LCD_Write_Com(0x01);
			 	 delay(50);
			 }
			 
		 }
	} 
}

#elif 0
void main(void)
{
	cmg88();//������ܣ�������
	LCD_Init();   //��̬��ʼ��
	LCD_Clear();//����
	while (1)
	{
	   	 LCD_Write_Char(7,0,'o');
	 	 LCD_Write_Char(8,0,'k');
	 	 LCD_Write_String(2,1,"hello world!");
	}
}

#endif