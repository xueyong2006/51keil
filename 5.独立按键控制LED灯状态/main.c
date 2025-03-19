#include <REGX52.H>
void Delay(unsigned int Xms)		//@12.000MHz
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
		if(P3_1==0)
		{
			Delay(20);
			while(P3_1==0);
			Delay(20);
			P2_0=~P2_0;
		}
		
	}
}