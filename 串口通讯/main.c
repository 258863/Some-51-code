#include <reg52.h>

void main()
{
	TMOD = 0x20;	 //ȷ��T1�Ĺ�����ʽ����ʽ2	��8λ������ʽ��һ������ʮ�����ơ�����LED��ʾ

	TH1 = 0xfd;	//����PPT�Ϲ�ʽ�����Ϊ������Ϊ9600ʱ��TH1ֵ
	TL1 = 0xfd;

	TR1 = 1;	   //����T1

	REN = 1;	  //��Ϊ1Ϊ�����������

	SM0 = 0;	 //SCON���пڿ��Ʒ�ʽ1
	SM1 = 1;

	EA = 1;
	ES = 1;

	while(1)
	{
	/*	if(RI == 1)
		{
			RI = 0;
			P1 = SBUF;
		}  */
	}
}

void ser()interrupt 4		   //�ж���ʽ
{
	RI = 0;
	P1 = SBUF;
}
