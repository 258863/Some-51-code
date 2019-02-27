#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit RS = P1^0;   //����˿�
sbit RW = P1^1;
sbit EN = P2^5;
sbit DU = P2^6;
sbit WE = P2^7;
sbit RX = P2^0 ;//���ն�
sbit TX = P2^1 ;//���ƶ�
sbit FM = P2^3;
sbit key = P3^7;
sbit IN1=P1^2; // �ߵ�ƽ1 ���ˣ���ת��
sbit IN2=P1^3; // �ߵ�ƽ1 ǰ������ת��
sbit IN3=P1^6; // �ߵ�ƽ1 ǰ������ת��	
sbit IN4=P1^7; // �ߵ�ƽ1 ���ˣ���ת��
sbit EN1=P1^4; // �ߵ�ƽʹ�� 
sbit EN2=P1^5; // �ߵ�ƽʹ�� 

uint x,y;
uchar a;
uchar num,num_m;
void delay(uint z);
void DelayUs2x(unsigned char t);
void DelayMs(unsigned char t);
bit LCD_Check_Busy(void);
void LCD_Write_Com(unsigned char com) ;
void LCD_Write_Data(unsigned char Data);
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s);
void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data);
void LCD_Clear(void);
void distance_show();
void gogogo();
uchar shu;
uint time;
unsigned long dis;
unsigned long distance[4];
unsigned char code ASCII[10] = {0,1,2,3,4
,5,6,7,8,9};
/************************************������ģ��*****************************/
void start_ultrasonic()	 //����������
{
	TX = 1;
	delay(1);	 //���β��֮���ʱ����
	TX = 0;
}

void  count_high()
{
	TR0 = 1;
	while(RX);	 //����
	TR0 = 0;
	shu = 0;
	distance_show();
	gogogo();	
}

void time0()interrupt 1
{
	shu++;	
}

void distance_show()
{
	time = TH0 * 256 + TL0;
	TH0 = 0;
	TL0 = 0;

	dis = (time * 1.7)/10 + 10;
	if(dis > 2800||dis < 25)
	{
		LCD_Write_String(0,1,"out of range!   ");
	}
	else
	{
		distance[0]=dis%10;
	    distance[1]=dis/10%10;
	    distance[2]=dis/100%10;
	    distance[3]=dis/1000;

	    LCD_Write_String(0,1,"Distance:");
	    LCD_Write_Char(9, 1, ASCII[distance[3]]+'0');
	    LCD_Write_Char(10, 1, ASCII[distance[2]]+'0');
	    LCD_Write_Char(11, 1, ASCII[distance[1]]+'0');
        LCD_Write_Char(12, 1, '.');
	    LCD_Write_Char(13, 1, ASCII[distance[0]]+'0');
		LCD_Write_Char(14, 1, 'C');
		LCD_Write_Char(15, 1, 'M');
	}
	
}




/************************************������ģ��*****************************/

/**********************************1602Һ��******************************/
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

void delay(uint z)
{
	uint x,y;
	for(x=z;z>0;z--)
		for(y=112;y>0;y--);
}

void _1602_init()
{
	cmg88();//������ܣ�������
	LCD_Init();   //��̬��ʼ��
	LCD_Clear();//����	
}

/************************1602Һ��************************************/

/***************************������***********************************/
void run(void)			  
{
	EN1=1;
	EN2=1;
    IN1=0;
	IN2=1; //��������ת
	IN3=1;
	IN4=0;//�ҵ������ת
}

void back(void)			
{
	EN1=1;
	EN2=1;
    IN1=1;
	IN2=0; //�����ķ�ת
	IN3=0;
	IN4=1;//�ҵ���ķ�ת
}

void right(void)		 
{
	EN1=1;
	EN2=1;
    IN1=0;
	IN2=1; //��������ת
	IN3=0;
	IN4=0;//�ҵ������
}

void left(void)		
{
	EN1=1;
	EN2=1;
    IN1=0;
	IN2=0; //��������
	IN3=1;
	IN4=0;//�ҵ����ת
}

void stop(void)			 
{
	EN1=1;
	EN2=1;
    IN1=0;
	IN2=0; //��������
	IN3=0;
	IN4=0;//�ҵ����ת
}

void spin_right(void)			  //���Ҵ�ת����
{
	EN1=1;
	EN2=1;//���ʹ��
    IN1=0;
	IN2=1; //������ת
	IN3=0;
	IN4=1;//�ҵ����ת
}

void gogogo()
{
     if(dis <= 240)  
     {  
          a++;
          while(a > 3)
          {
              //a=0;
              FM=0;
              back(); 
              delay(230);

                right();
                delay(100);        
                //stop();
                delay(100);     
                start_ultrasonic();                 
                while(RX==0);
                TR0 = 1;             
                while(RX);      
                TR0 = 0;            
                distance_show();

                if(dis>340)  
                {
                    a=0;
                    right();
                    delay(90);  
                    stop();               
                    delay(200);        
                    FM=1;
                    run();
                }             
           } 
     }
     else
     {
          a=0;
          run();       //���ϰ��� ֱ��
     }
} 

void keyscan()
{
  A:	if(key == 0)
		{
			delay(10);
			if(key == 0)
			{
				FM = 0;
				while(!key);
				FM = 1;
			}
			else
			{
				goto A;
			}
		 }
		 else
		 {
		 	goto A;
		 }
}

/**************************������*************************************/
void main(void)
{
    uint i;
	_1602_init();

	TMOD=0x01;
    EA=1;		  
    TH0=0;
    TL0=0;
    ET0=1; 
	LCD_Write_String(2,0,"come on baby");         
	keyscan();
	
 	while(1)
	  {	   
		RX=1;
		start_ultrasonic();		 //����ģ�� 
        for(i = 951;i > 0;i--)
	    {
	       if(RX==1)
		   {
               count_high();		 //�������ߵ�ƽ�����ȼ��㺯��
		   }
    	 }	
	  }	
}