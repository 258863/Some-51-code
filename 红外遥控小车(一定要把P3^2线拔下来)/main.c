#include<reg52.h>   
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit RS = P1^0;   //����˿�
sbit RW = P1^1;
sbit EN = P2^5;
sbit DU = P2^6;
sbit WE = P2^7;
sbit FM = P2^3;
sbit IR=P3^2;  //����ӿڱ�־
sbit IN1=P1^2; // �ߵ�ƽ1 ���ˣ���ת��
sbit IN2=P1^3; // �ߵ�ƽ1 ǰ������ת��
sbit IN3=P1^6; // �ߵ�ƽ1 ǰ������ת��	
sbit IN4=P1^7; // �ߵ�ƽ1 ���ˣ���ת��
sbit EN1=P1^4; // �ߵ�ƽʹ�� 
sbit EN2=P1^5; // �ߵ�ƽʹ�� 
#define DataPort P0 //�������ݶ˿� ����������DataPort ����P0 �滻
/*------------------------------------------------
                ȫ�ֱ�������
------------------------------------------------*/
unsigned char  irtime;//������ȫ�ֱ���

bit irpro_ok,irok;
unsigned char IRcord[4];
unsigned char irdata[33];

/*------------------------------------------------
                  ��������
------------------------------------------------*/

void Ir_work(void);
void Ircordpro(void);
void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data)	;
void run();
void back();
void right();
void left();
void stop();

/*------------------------------------------------
                  ��ʱ��0�жϴ���
------------------------------------------------*/

void tim0_isr (void) interrupt 1 using 1
{
  irtime++;  //���ڼ���2���½���֮���ʱ��
}

/*------------------------------------------------
                  �ⲿ�ж�0�жϴ���
------------------------------------------------*/
void EX0_ISR (void) interrupt 0 //�ⲿ�ж�0������
{
  static unsigned char  i;             //���պ����źŴ���
  static bit startflag;                //�Ƿ�ʼ�����־λ

if(startflag)                         
   {
    if(irtime<63&&irtime>=33)//������ TC9012��ͷ�룬9ms+4.5ms
                        i=0;
    		irdata[i]=irtime;//�洢ÿ����ƽ�ĳ���ʱ�䣬�����Ժ��ж���0����1
    		irtime=0;
    		i++;
   			 if(i==33)
      		{
	  			  irok=1;   //������׼��
				    i=0;
	  			}
          }
  	 else
		{
		irtime=0;
		startflag=1;
		}

}

/*------------------------------------------------
                ��ʱ��0��ʼ��
------------------------------------------------*/
void TIM0init(void)//��ʱ��0��ʼ��
{

  TMOD=0x02;//��ʱ��0������ʽ2��TH0����װֵ��TL0�ǳ�ֵ
  TH0=0x00; //����ֵ
  TL0=0x00; //��ʼ��ֵ
  ET0=1;    //���ж�
  TR0=1;    
}
/*------------------------------------------------
                  �ⲿ�ж�0��ʼ��
------------------------------------------------*/
void EX0init(void)
{
 IT0 = 1;   //ָ���ⲿ�ж�0�½��ش�����INT0 (P3.2)
 EX0 = 1;   //ʹ���ⲿ�ж�
 EA = 1;    //�����ж�
}
/*------------------------------------------------
                  ��ֵ����
------------------------------------------------*/

void Ir_work(void)//�����ֵɢת����
{
       switch(IRcord[2])//�жϵ���������ֵ
	         {
			 case 0x18:run();break;//ǰ
			 case 0x52:back();break;//��
			 case 0x08:left();;break;//��
			 case 0x5a:right();break;//��
			 case 0x1c:stop();break;//ͣ

             default:break;
			 }

		  irpro_ok=0;//������ɱ�־

  }
/*------------------------------------------------
                ������ֵ����
------------------------------------------------*/
void Ircordpro(void)//������ֵ������
{ 
  unsigned char i, j, k;
  unsigned char cord,value;

  k=1;
  for(i=0;i<4;i++)      //����4���ֽ�
     {
      for(j=1;j<=8;j++) //����1���ֽ�8λ
         {
          cord=irdata[k];
          if(cord>7)//����ĳֵΪ1������;����о��Թ�ϵ������ʹ��12M���㣬��ֵ������һ�����
             value|=0x80;
          if(j<8)
		    {
			 value>>=1;
			}
           k++;
         }
     IRcord[i]=value;
     value=0;     
     } 
	 irpro_ok=1;//������ϱ�־λ��1
}

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

 void run(void)			  //ǰ������
{
	EN1=1;
	EN2=1;//���ʹ��
    IN1=0;
	IN2=1; //��������ת
	IN3=1;
	IN4=0;//�ҵ������ת
}

void back(void)			  //���˺���
{
	EN1=1;
	EN2=1;//���ʹ��
    IN1=1;
	IN2=0; //�����ķ�ת
	IN3=0;
	IN4=1;//�ҵ���ķ�ת
}

void right(void)		 //��ת����
{
	EN1=1;
	EN2=1;//���ʹ��
    IN1=0;
	IN2=1; //��������ת
	IN3=0;
	IN4=0;//�ҵ������
}

void left(void)			  //��ת����
{
	EN1=1;
	EN2=1;//���ʹ��
    IN1=0;
	IN2=0; //��������
	IN3=1;
	IN4=0;//�ҵ����ת
}

void stop(void)			  //ֹͣ����
{
	EN1=1;
	EN2=1;//���ʹ��
    IN1=0;
	IN2=0; //��������
	IN3=0;
	IN4=0;//�ҵ����ת
}

/*------------------------------------------------
                    ������
------------------------------------------------*/
void main(void)
{
    EX0init(); //��ʼ���ⲿ�ж�
    TIM0init();//��ʼ����ʱ��
	FM = 1;
 	cmg88();//������ܣ�������
	LCD_Init();   //��̬��ʼ��
	LCD_Clear();//����

    //LCD_Write_String(0,0," value of RC:");
 while(1)//��ѭ��
   {
    if(irok)                        //������պ��˽��к��⴦��
	  {   
	   Ircordpro();
 	   irok=0;
	  }

    if(irpro_ok)                   //�������ú���й��������簴��Ӧ�İ�������ʾ��Ӧ�����ֵ�
	  {
	   Ir_work();
	   FM = 0;
	   DelayMs(100);
	   FM = 1;

  	  }
   }
}
  
  