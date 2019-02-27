//J8���Ⱥ��¶ȴ�����


#include<reg51.h>
#include<intrins.h>

#define uchar unsigned char
#define uint unsigned int 
#define Write_PCF8591 0x90
#define Read_PCF8591 0x91
#define Control_Byte 0x80	  //0xf0Ϊʵ����ϵ�L3��

sbit SCL=P2^1;
sbit SDA=P2^0;
sbit RST=P2^4;   //ʱ�� ���Ϻ���Թص�DS1302оƬ���
sbit DU=P2^6;
sbit WE=P2^7;

unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

uchar num,temp;

void delayms(uint z)
{
    uint x,y;
    for(x=0;x<z;x++)
        for(y=0;y<112;y++);
}

void delay()
{
    _nop_();_nop_();
    _nop_();_nop_();
    _nop_();_nop_();
    _nop_();_nop_();
}
/***************************************IIC***************************************/
void init_IIC()
{
    SDA=1;
    delay();
    SCL=1;
    delay();
}

void start()
{
    SDA=1;
    delay();
    SCL=1;
    delay();
    SDA=0;
    delay();
}

void stop()
{
    SDA=0;
    delay();
    SCL=1;
    delay();
    SDA=1;
    delay();
}

void ack()   //Ӧ��
{
    uchar i=0;
    SCL=1;
    delay();
    while((SDA==1)&&(i<250)) i++;
    SCL=0;
    delay();
}

void noack()  //��Ӧ��
{
    SDA=1;
    delay();
    SCL=1;
    delay();
    SCL=0;
    delay();
}

void Write_Byte(uchar dat)
{
    uchar i;
    for(i=0;i<8;i++)
    {
        SCL=0;
        delay();
        if(dat&0x80)
        {
            SDA=1;
        }
        else
        {
            SDA=0;
        }
        dat=dat<<1;
        SCL=1;
        delay();
    }
    SCL=0;
    delay();
    SDA=1;
    delay();    
}

uchar Read_Byte()
{
    uchar i,dat;
    SCL=0;
    delay();
    SDA=1;
    delay();
    for(i=0;i<8;i++)
    {
        SCL=1;
        delay();
        dat=dat<<1;
        if(SDA)
        {
            dat++;
        }
        SCL=0;
        delay();
    }
    return dat;
}
/******************************************************************IIC********************************/
/*****************************************************************AD/DA*******************************/
void DAC_PCF8591(uchar add,uchar dat)//д��DA��ģת��ֵ
{
    start();
    Write_Byte(add);		//д��оƬ��ַ
    ack();
    Write_Byte(Control_Byte);   //д�����λ
    ack();
    Write_Byte(dat);   //д����
    ack();
    stop();
}

uchar ADC_Read(uchar add0,uchar add1,uchar chl)		//chl=0�����ظ�һ��ֹͣ��chl=1����ѭ�� ��0�ͺ�
{		//��ȡADģ��ת����ֵ
    uchar dat;
    start();
    Write_Byte(add0);    //оƬ��ַ
    ack();
    Write_Byte(Control_Byte|chl);	//ѡ���ͨ��
    ack();
    start();
    Write_Byte(add1);	   //�����ַ
    ack();
    dat=Read_Byte();	 //������
    noack();
    stop();
    return dat;
}
/****************************************************AD/DA*************************************/
void display (uchar num)
{	
        uchar bai,shi,ge;
		bai = num / 100;
		shi = num / 10 %10;
		ge = num % 10;

		DU=1;
		P0=table[ge];
		DU=0;
		WE=1;
		P0=0xfb;
		WE=0;
		delayms(5);

		DU=1;
		P0=table[shi];
		DU=0;
		WE=1;
		P0=0xfd;
		WE=0;
        delayms(5);

		DU=1;
		P0=table[bai];
		DU=0;
		WE=1;
		P0=0xfe;
		WE=0;
		delayms(5);
}

void init_time0()
{
	    TMOD |= 0x01;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��		     
 		TH0=0x00;		  //���¸�ֵ 1/68s
 		TL0=0x00;
 		EA=1;            //���жϴ�
 		ET0=1;           //��ʱ���жϴ�
 		TR0=1;           //��ʱ�����ش�	
}

void main()
{
	init_IIC();
	init_time0();
	RST = 0;
    DU = 0;
	WE = 0;
	while(1)
	{
		 display(num);
	}	
}

void time0()interrupt 1
{
	TH0 = 0;
	TL0 = 0;
	//DAC_PCF8591(Write_PCF8591,0);
	num = ADC_Read(Write_PCF8591,Read_PCF8591,0);

}