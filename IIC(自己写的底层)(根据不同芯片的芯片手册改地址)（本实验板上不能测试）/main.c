#include <reg52.h>
#
#define uchar unsigned char
#define uint unsigned int
sbit SDA = P2^0;
sbit SCL = P2^1;
uchar a;

void delay()
{ ; ; }

void delay1(uint z)
{
      uint x,y;
      for(x=z;x>0;x--)
            for(y=112;y>0;y--);
}

void start()      //��ʼ�ź�
{
      SDA = 1;
      delay();
      SCL = 1;
      delay();
      SDA = 0;
      delay();
}

void stop()      //��ֹ�ź�
{
      SDA = 0;
      delay();
      SCL = 1;
      delay();
      SDA = 1;
      delay();
}

void respons()    //Ӧ��
{
      uchar i;
      SCL = 1;
      delay();
      while((SDA == 1)&&(i < 250))   //SDA��Ӧ���һֱ��ѭ��������ʱ��̫��
      {
            i++;
      }
      SCL = 0;
      delay();


}

void init()       //��ʼ��
{
      SDA = 1;
      delay();
      SCL = 1;
      delay();
}

void write_byte(uchar date)
{
      uchar i,temp;
      temp = date;
      for(i=0;i<8;i++)
      {
            temp = temp << 1;
            SCL = 0;
            delay();
            SDA = CY;
            delay();
            SCL = 1;
            delay();
      }
      SCL = 0;
      delay();
      SDA = 1;//�ͷ������źţ�Ϊ�˼���Ӧ���źţ�SDA = 0��
      delay();
}

uchar read_byte()
{
      uchar i,k;
      SCL = 0;
      delay();
      SDA = 1;
      delay();
      for(i=0;i<8;i++)
      {
            SCL = 1;
            delay();
            k = (k<<1)|SDA;
            SCL = 0;
            delay();
      }
      return k;
}

void write_add(uchar address,uchar date)
{
      start();
      write_byte(0xa0);  //��ʼ��ַ
      respons();
      write_byte(address);   //Ҫ�����ݵĵ�ַ
      respons();
      write_byte(date);   //Ҫ��������
      respons();
      stop();
}

uchar read_add(uchar address)
{
      uchar date;
      start();
      write_byte(0xa0);
      respons();
      write_byte(address);   
      respons();
      start();
      write_byte(0xa1);   //������ַ
      respons();
      date = read_byte();
      stop();
      return date;
}

void main()
{
      init();
      write_add(23,0xaa);
      delay1(100);
      P1 = read_add(23);      //���������
      while(1);
}