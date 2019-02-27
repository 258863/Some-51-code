#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit DS=P2^2;      //����     //define interface
uint temp;             // variable of temperature
uchar flag1;            // sign of the result positive or negative
sbit dula=P2^6;
sbit wela=P2^7;
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};				//��С����
unsigned char code table1[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,
                        0x87,0xff,0xef};		  //��С�����

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

void display(uint temp)			//��ʾ����
{
   uchar A1,A2,A2t,A3,ser;
   ser=temp/10;
   SBUF=ser;
   A1=temp/100;
   A2t=temp%100;
   A2=A2t/10;
   A3=A2t%10;
   dula=0;
   P0=table[A1];		//��ʾ��λ
   dula=1;
   dula=0;

   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(1);

   dula=0;
   P0=table1[A2];		//��ʾʮλ
   dula=1;
   dula=0;

   wela=0;
   P0=0xfd;
   wela=1;
   wela=0;
   delay(1);

   P0=table[A3];		//��ʾ��λ
   dula=1;
   dula=0;

   P0=0xfb;
   wela=1;
   wela=0;
   delay(1);
}


void main()
{
 uchar a;
  do
  {
      tmpchange();
      for(a=100;a>0;a--)
      {
          display(tmp());
      }
  }while(1);
}

