#include<reg52.h>
#define uint unsigned int
sbit LED = P1^4;		  
void delay(uint z)	//����ʱ����ʱ��ʹ�õ��Եķ�����
{
	uint x,y;
	for(x = z;x > 0;x--)
		for(y = 227;y > 0;y--);
}

void main()
{
    while(1)
	{
		LED = 1;
		delay(1);
		LED = 0;
		delay(1);
	}
    	
}