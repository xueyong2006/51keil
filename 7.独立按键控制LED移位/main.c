#include <REGX52.H>
unsigned char LEDNum;
void Delay(unsigned int Xms)		//@12.000MHz
{
	unsigned char i, j;
	while(Xms--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
	}
	
}

void main()

{
	P2=~0X01;
	while(1)
		{
			if(P3_1==0)
			{
				Delay(20);
				while(P3_1==0);
				Delay(20);
				LEDNum++;
				if(LEDNum>=8)
							LEDNum=0;
				P2=~(0X01<<LEDNum);
			}
			if(P3_0==0)
			{
				Delay(20);
				while(P3_0==0);
				Delay(20);
				if(LEDNum==0)
							LEDNum=7;
				else
					LEDNum--;
				P2=~(0X01<<LEDNum);
			}
		}
			

}
