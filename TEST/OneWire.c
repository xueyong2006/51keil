#include<REGX52.H>

sbit OneWire_DQ=P3^7;

unsigned char OneWire_Init()
{
	unsigned char i;
	unsigned char AckBit;
	EA=0;
	OneWire_DQ=1;
	OneWire_DQ=0;
	i = 247;while (--i);      //500us
	OneWire_DQ=1;
	i = 32;while (--i);      //70us 
	AckBit=OneWire_DQ;
	i = 247;while (--i);     //500us
	EA=1;
	return AckBit;
}

void OneWire_SendBit(unsigned char Bit)
{
	unsigned char i;
	EA=0;
	OneWire_DQ=0;
	i = 4;while (--i);      //10us
	OneWire_DQ=Bit;
	i = 24;while (--i);     //50us
	OneWire_DQ=1;
	EA=1;
}

unsigned char OneWire_ReceiveBit()
{
	unsigned char Bit,i;
	EA=0;
	OneWire_DQ=0;
	i = 2;while (--i);     //5us
	OneWire_DQ=1;
	i = 2;while (--i);     //5us
	Bit=OneWire_DQ;
	i = 24;while (--i);    //50us
	EA=1;
	return Bit;
}
void OneWire_SendByte(unsigned char Byte)
{
	unsigned char i;
	
	for(i=0;i<8;i++)
	{
		OneWire_SendBit(Byte&(0x01<<i));
		
	}
}

unsigned char OneWire_ReceiveByte()
{
	unsigned char i;
	unsigned char Byte=0X00;
	for(i=0;i<8;i++)
	{
		if(OneWire_ReceiveBit())
		{
			Byte|=(0x01<<i);
		}
	}
	return Byte;
}