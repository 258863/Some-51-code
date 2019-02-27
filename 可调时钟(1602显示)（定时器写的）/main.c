#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit RS = P1^0;   //����˿�
sbit RW = P1^1;
sbit EN = P2^5;
sbit DU = P2^6;
sbit WE = P2^7;
sbit S1 = P3^0;		 //����ֹͣ+�����˸
sbit S2 = P3^1;
sbit S3 = P3^2;
sbit rd = P3^7;  //ʹ�����Ҷ˵�ǰ������������
uint x,y;
uchar num,num_m;
uchar count,shi,fen,miao;
uchar S1num;

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
 	   cmg88();//������ܣ������� 
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
   	   	LCD_Clear();//����
	   //	while (1)
	//	{

	 	 	LCD_Write_String(0,0,"   2017-08-11");
		 	LCD_Write_String(0,1,"    00:00:00");
	//	}
   TMOD = 0x01;
   TH0 = (65536 - 50000) / 256;
   TL0 = (65536 - 50000) % 256;
   EA = 1;
   ET0 = 1;
   TR0 = 1;
 }
 
void delay(uint z)
{
	uint x,y;
	for(x=z;z>0;z--)
		for(y=112;y>0;y--);
}

void write_sfm(uchar add,uchar date)
{
	uchar shi,ge;
	shi = date / 10;
	ge = date % 10;
	LCD_Write_Com(0x80+0x40+add);
	LCD_Write_Data(0x30+shi);
	LCD_Write_Data(0x30+ge);
}

void key()
{
	 rd = 0;
	 if(S1 == 0)
	 {
	 	delay(5);
		if(S1 == 0)
		{
		    S1num++;
		    while(!S1);		   //������
			if(S1num == 1)
			{
				TR0 = 0;  //��ʱ���رգ�ʱ��ֹͣ
				LCD_Write_Com(0x80+0x40+10);   //���ص���
				LCD_Write_Com(0x0f);   //�����˸
			}
			if(S1num == 2)
			{
				LCD_Write_Com(0x80+0x40+7);   //���ص���
				LCD_Write_Com(0x0f);   //�����˸
			}
			if(S1num == 3)
			{
				LCD_Write_Com(0x80+0x40+4);   //���ص���
				LCD_Write_Com(0x0f);   //�����˸
			}
			if(S1num == 4)
			{
			    S1num = 0;
				LCD_Write_Com(0x0c);   //��겻��˸+����ʾ
				TR0 = 1;
			}				
		}
	 }
	 if(S1num != 0)
	{
		if(S2 == 0)
		{
			delay(5);
			if(S2 == 0)
			{
			    while(!S2);
				if(S1num == 1)	  //��λ��
				{
					miao++;
					if(miao == 60)
					{
						miao = 0;
					}
					write_sfm(10,miao);
					LCD_Write_Com(0x80+0x40+10);
				}
				if(S1num == 2)	   //��λ��
				{
					fen++;
					if(fen == 60)
					{
						fen = 0;
					}
					write_sfm(7,fen);
					LCD_Write_Com(0x80+0x40+7);
				}
				if(S1num == 3)	   //ʮλ��
				{
					shi++;
					if(shi == 24)
					{
						shi = 0;
					}
					write_sfm(4,shi);
					LCD_Write_Com(0x80+0x40+4);
				}	
			}
		}
		if(S3 == 0)
		{
			delay(5);
			if(S3 == 0)
			{
				while(!S3);
				if(S1num == 1)
				{					
					if(miao == 0)
					{
						miao = 60;
						write_sfm(10,miao);
						LCD_Write_Com(0x80+0x40+10);
					}
					miao--;	
					write_sfm(10,miao);
					LCD_Write_Com(0x80+0x40+10);
				}
				if(S1num == 2)
				{
					if(fen == 0)
					{
						fen = 60;
						write_sfm(7,fen);
						LCD_Write_Com(0x80+0x40+7);
					}
					fen--;	
					write_sfm(7,fen);
					LCD_Write_Com(0x80+0x40+7);
				}
				if(S1num == 3)
				{
					if(shi == 0)
					{
						shi = 24;
						write_sfm(4,shi);
						LCD_Write_Com(0x80+0x40+4);
					}
					shi--;	
					write_sfm(4,shi);
					LCD_Write_Com(0x80+0x40+4);
				}
			}
		}
	}
}

void main(void)
{
	LCD_Init();   //��̬��ʼ��
	while(1)
	{
		key();
	}	
}

void timer0()interrupt 1
{
	TH0 = (65536 - 50000) / 256;
    TL0 = (65536 - 50000) % 256;
	count++;
	if(count == 18)
		{
			count = 0;
			miao++;
			if(miao == 60)
			{
				miao = 0;
				fen++;
				if(fen == 60)
				{
					fen = 0;
					shi++;
					if(shi == 24)
					{
						shi = 0;
					}
					write_sfm(4,shi);
				}
				write_sfm(7,fen);
			}
			write_sfm(10,miao);			 
		}
}











/*#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit DU = P2^6;
sbit WE = P2^7;
sbit RS = P1^0;
sbit EN = P2^5;
uchar code table[]="   2017-8-11";
uchar code table1[]="    00:00:00";

void delay(uchar z)
{
	uint x,y;
	for(x=z;x>0;x--)
		for(y=112;y>0;y--);
}

void write_com(uchar com)
{
	RS = 0;
	EN = 0;
	P0 = com;
	delay(5);
	EN = 1;
	delay(5);
	EN = 0;	
}

void write_date(uchar date)
{
	RS = 1;
	EN = 0;
	P0 = date;
	delay(5);
	EN = 1;
	delay(5);
	EN = 0;	
}

void init()
{
	uchar num;
	DU = 1;
	P0 = 0x00;
	DU = 0;
	WE = 1;
	P0 = 0x00;
	WE = 0;

    write_com(0x38);
	write_com(0x0c);
	write_com(0x06);
	write_com(0x01);
	write_com(0x80);

	for(num = 0;num < 12;num++)
	{
		write_date(table[num]);
		delay(5);
	}
	write_com(0x80+0x40);
	for(num = 0;num < 12;num++)
	{
		write_date(table1[num]);
		delay(5);
	}
}

 void LCD_Init(void)		//��̬��ʼ��
 {
    uchar num;
	DU = 1;
	P0 = 0x00;
	DU = 0;
	WE = 1;
	P0 = 0x00;
	WE = 0;

    write_com(0x38);    /*��ʾģʽ����*/
/*    delay(5);
    write_com(0x38);
    delay(5);
    write_com(0x38);
    delay(5);
    write_com(0x38);
    write_com(0x08);    /*��ʾ�ر�*/
/*    write_com(0x01);    /*��ʾ����*/
/*    write_com(0x06);    /*��ʾ����ƶ�����*/
/*    delay(5);
	write_com(0x0c);
    write_com(0x80);    

	for(num = 0;num < 12;num++)
	{
		write_date(table[num]);
		delay(5);
	}
	write_com(0x80+0x40);
	for(num = 0;num < 12;num++)
	{
		write_date(table1[num]);
		delay(5);
	}
 }

void main()
{
//	init();
	LCD_Init();
	while(1);
}
*/

