#include<reg52.h>
sbit WE=P2^7;
sbit DU=P2^6;
sbit D1=P1^0;
unsigned  char  num;
unsigned char code yyy[] ={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,
0x7d,0x07,0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71
};	
void delay(unsigned int xms);
void main()
{
    EA=1;	 //�����ж�
	EX0=1;			   //���ⲿ�ж�0
    IT0=0;   //0Ϊ�͵�ƽ���� ��1λ�����ش���


	WE=1;  //��λѡ������
	P0=0x00; //λѡ�͵�ƽ��Ч
    WE=0;		//�ر�

	while(1)
	{
		for(num=0;num<16;num++)
		{
		      D1=1;

			  DU=1;
			  P0=yyy[num];
	     	  DU=0;
			  delay(1000);
		}
	}
}

void delay(unsigned int xms)
{
	unsigned int i,j;
	for(i=xms;i>0;i--)
		for(j=112;j>0;j--);
}

void in()interrupt 0
{	
    D1=0;
}