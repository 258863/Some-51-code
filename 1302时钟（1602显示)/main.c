#include<reg52.h> //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit RS = P1^0;   //����˿�
sbit RW = P1^1;
sbit EN = P2^5;
sbit DU = P2^6;
sbit WE = P2^7;
sbit  DS1302_CLK = P2^1;              //ʵʱʱ��ʱ��������
sbit  DS1302_IO  = P2^0;              //ʵʱʱ������������
sbit  DS1302_RST = P2^4;              //ʵʱʱ�Ӹ�λ������
sbit  ACC0 = ACC^0;
sbit  ACC7 = ACC^7;

typedef struct __SYSTEMTIME__
{
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour;
	unsigned char Week;
	unsigned char Day;
	unsigned char Month;
	unsigned char Year;
	unsigned char DateString[9];
	unsigned char TimeString[9];
}SYSTEMTIME;	//�����ʱ������

#define AM(X)	X
#define PM(X)	(X+12)            	  // ת��24Сʱ��
#define DS1302_SECOND	0x80     //д�Ĵ���
#define DS1302_MINUTE	0x82
#define DS1302_HOUR		0x84
#define DS1302_WEEK		0x8A
#define DS1302_DAY		0x86
#define DS1302_MONTH	0x88
#define DS1302_YEAR		0x8C
#define DS1302_RAM(X)	(0xC0+(X)*2)   	//���ڼ��� DS1302_RAM ��ַ�ĺ�

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

void delay(uint z)
{
	uint x,y;
	for(x=z;z>0;z--)
		for(y=112;y>0;y--);
}

void DS1302InputByte(unsigned char d) 	//ʵʱʱ��д��һ�ֽ�(�ڲ�����)
{
    unsigned char i;
    ACC = d;
    for(i=8; i>0; i--)
    {
        DS1302_IO = ACC0;           	//�൱�ڻ���е� RRC
        DS1302_CLK = 1;
        DS1302_CLK = 0;
        ACC = ACC >> 1;
    }
}

unsigned char DS1302OutputByte(void) 	//ʵʱʱ�Ӷ�ȡһ�ֽ�(�ڲ�����)
{
    unsigned char i;
    for(i=8; i>0; i--)
    {
        ACC = ACC >>1;         			//�൱�ڻ���е� RRC
        ACC7 = DS1302_IO;
        DS1302_CLK = 1;
        DS1302_CLK = 0;
    }
    return(ACC);
}

void Write1302(unsigned char ucAddr, unsigned char ucDa)	//ucAddr: DS1302��ַ, ucData: Ҫд������
{
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302InputByte(ucAddr);       	// ��ַ������
    DS1302InputByte(ucDa);       	// д1Byte����
//    DS1302_CLK = 1;
    DS1302_RST = 0;
}

unsigned char Read1302(unsigned char ucAddr)	//��ȡDS1302ĳ��ַ������
{
    unsigned char ucData;
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302InputByte(ucAddr|0x01);        // ��ַ������
    ucData = DS1302OutputByte();         // ��1Byte����
//    DS1302_CLK = 1;
    DS1302_RST = 0;
    return(ucData);
}

void DS1302_SetProtect(bit flag)        //�Ƿ�д����
{
	if(flag)
		Write1302(0x8E,0x80);   //Ӧ����0x80�Ŷ�
	else
		Write1302(0x8E,0x00);
}

void DS1302_SetTime(unsigned char Address, unsigned char Value)        // ����ʱ�亯��
{
	DS1302_SetProtect(0);
	Write1302(Address, ((Value/10)<<4 | (Value%10)));  //����BCD��
}

void DS1302_GetTime(SYSTEMTIME *Time)
{
	unsigned char ReadValue;
	ReadValue = Read1302(DS1302_SECOND);
	Time->Second = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);	//�˽���ת����ʮ����
	ReadValue = Read1302(DS1302_MINUTE);
	Time->Minute = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_HOUR);
	Time->Hour = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_DAY);
	Time->Day = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_WEEK);
	Time->Week = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_MONTH);
	Time->Month = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_YEAR);
	Time->Year = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
}

void DateToStr(SYSTEMTIME *Time)	 //ת��Ϊ�ַ���
{
	Time->DateString[0] = Time->Year/10 ;	 //�������λ��ʮλ
	Time->DateString[1] = Time->Year%10 ;
	Time->DateString[2] = '-';
	Time->DateString[3] = Time->Month/10;
	Time->DateString[4] = Time->Month%10 ;
	Time->DateString[5] = '-';
	Time->DateString[6] = Time->Day/10 ;
	Time->DateString[7] = Time->Day%10 ;
	Time->DateString[8] = '\0';
}

void TimeToStr(SYSTEMTIME *Time)
{
	Time->TimeString[0] = Time->Hour/10 ;
	Time->TimeString[1] = Time->Hour%10 ;
	Time->TimeString[2] = ':';
	Time->TimeString[3] = Time->Minute/10 ;
	Time->TimeString[4] = Time->Minute%10 ;
	Time->TimeString[5] = ':';
	Time->TimeString[6] = Time->Second/10;
	Time->TimeString[7] = Time->Second%10 ;
	Time->TimeString[8] = '\0';
}

void Initial_DS1302(void)
{
	unsigned char Second=Read1302(DS1302_SECOND);
	if(Second&0x80)
		DS1302_SetTime(DS1302_SECOND,0);
}

void Week(SYSTEMTIME *Time)	
{
	switch(Time->Week)
	{								
		case 1:LCD_Write_Char(12,0,'M');//delay(5);
		       delay(5);
			   LCD_Write_Char(13,0,'0');//delay(5);
			   delay(5);
			   LCD_Write_Char(14,0,'N');
			   delay(5);
			   break;

		case 2:LCD_Write_Char(12,0,'T');//delay(5);
		       delay(5);
			   LCD_Write_Char(13,0,'U');//delay(5);
			   delay(5);
			   LCD_Write_Char(14,0,'E');
			   delay(5);
			   break;

		case 3:LCD_Write_Char(12,0,'W');//delay(5);
		       delay(5);
			   LCD_Write_Char(13,0,'E');//delay(5);
			   delay(5);
			   LCD_Write_Char(14,0,'D');
			   delay(5);
			   break;

		case 4:LCD_Write_Char(12,0,'T');//delay(5);
		       delay(5);
			   LCD_Write_Char(13,0,'H');//delay(5);
			   delay(5);
			   LCD_Write_Char(14,0,'U');
			   delay(5);
			   break;

		case 5:LCD_Write_Char(12,0,'F');//delay(5);
		       delay(5);
			   LCD_Write_Char(13,0,'R');//delay(5);
			   delay(5);
			   LCD_Write_Char(14,0,'I');
			   delay(5);
			   break;

		case 6:LCD_Write_Char(12,0,'S');//delay(5);
		       delay(5);
			   LCD_Write_Char(13,0,'T');//delay(5);
			   delay(5);
			   LCD_Write_Char(14,0,'A');
			   delay(5);
			   break;

		case 7:LCD_Write_Char(12,0,'S');//delay(5);
		       delay(5);
			   LCD_Write_Char(13,0,'U');//delay(5);
			   delay(5);
			   LCD_Write_Char(14,0,'N');
			   delay(5);
			   break;
	}
}

void display(SYSTEMTIME *Time)
{
   LCD_Write_Char(1,0,'2');
   LCD_Write_Char(2,0,'0');
   LCD_Write_Char(3,0,(Time->DateString[0])+'0');   
   LCD_Write_Char(4,0,(Time->DateString[1])+'0');
   LCD_Write_Char(5,0,'-');
   LCD_Write_Char(6,0,(Time->DateString[3])+'0');
   LCD_Write_Char(7,0,(Time->DateString[4])+'0');   
   LCD_Write_Char(8,0,'-');
   LCD_Write_Char(9,0,(Time->DateString[6])+'0');
   LCD_Write_Char(10,0,(Time->DateString[7])+'0'); 
   Week(Time);
   LCD_Write_Char(4,1,(Time->TimeString[0])+'0');   
   LCD_Write_Char(5,1,(Time->TimeString[1])+'0');
   LCD_Write_Char(6,1,':');
   LCD_Write_Char(7,1,(Time->TimeString[3])+'0');
   LCD_Write_Char(8,1,(Time->TimeString[4])+'0');   
   LCD_Write_Char(9,1,':');
   LCD_Write_Char(10,1,(Time->TimeString[6])+'0');
   LCD_Write_Char(11,1,(Time->TimeString[7])+'0');
}

void main()
{
    SYSTEMTIME CurrentTime;
	Initial_DS1302();
	cmg88();//������ܣ�������
	LCD_Init();   //��̬��ʼ��
	LCD_Clear();//����
	DS1302_SetTime(DS1302_HOUR,23);	//���õ�ʱ����10��54��30
	DS1302_SetTime(DS1302_MINUTE,59);
	DS1302_SetTime(DS1302_SECOND,56);
	DS1302_SetTime(DS1302_WEEK,7);
	DS1302_SetTime(DS1302_DAY,13);
	DS1302_SetTime(DS1302_MONTH,8);
	DS1302_SetTime(DS1302_YEAR,17); 
	while(1)
	{
	    DS1302_GetTime(&CurrentTime);
		DateToStr(&CurrentTime);
		TimeToStr(&CurrentTime);
		display(&CurrentTime);
	}
}

