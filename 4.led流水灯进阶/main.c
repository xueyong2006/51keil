#include <REGX52.H>

void Delay1ms(unsigned int Xms)		//@11.0592MHz
{
	unsigned char i, j;
	while(Xms)
	{
	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
		Xms--;
	}
	
	
}

void main()
{
	while(1)
	{
		P2=0xFE;
		Delay1ms(50);
		P2=0xFD;
		Delay1ms(1000);
		P2=0xFB;
		Delay1ms(500);
		P2=0xF7;
		Delay1ms(5000);
		P2=0xEF;
		Delay1ms(100);
		P2=0xDF;
		Delay1ms(1000  );
		P2=0xBF;
		Delay1ms(5000);
		P2=0x7F;
		Delay1ms(500);
	}
		






}