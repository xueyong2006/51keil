#include <REGX52.H>
#include "Buzzer.h"
#include "Delay.h"
#include "Nixie.h"
#include "Timer0.h"
#include "MatrixKey.h"
#include "INTRINS.H"
#include "XPT2046.h"


unsigned char mode;
unsigned char count=98;
unsigned char Number=0;
unsigned char LedLight[]={0x7F,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0xFE,0xFF};
unsigned char KeyNumber;
unsigned char light=0;
unsigned char ADValue;

sbit Buzzer=P2^5;

void test()
{
	if(KeyNumber==3)
	{
		mode=2;
	}
	if(KeyNumber==1)
	{
		mode=3;
	}
	switch(mode)
	{
		case 0:
			Nixie(1,2);
			Nixie(2,0);
			Nixie(3,2);
			Nixie(4,1);
			Nixie(5,1);
			Nixie(6,2);
			Nixie(7,0);
			Nixie(8,4);
			break;
		case 1:
			
			P2=LedLight[Number];
			Nixie(7,count/10);
			Nixie(8,count%10);
			Nixie(1,10);
			Nixie(2,5);
			Nixie(4,ADValue/100);
			Nixie(5,(ADValue%100)/10);
			Nixie(6,ADValue%10);
			break;
		case 2:
			
			P2=LedLight[Number];
			Nixie(7,count/10);
			Nixie(8,count%10);
			Nixie(1,10);
			Nixie(2,5);
			Nixie(4,ADValue/100);
			Nixie(5,(ADValue%100)/10);
			Nixie(6,ADValue%10);
			break;
		case 3:
			P2_0=light;
			P2_1=light;
			P2_2=light;
			P2_3=light;
			P2_4=light;
			P2_5=light;
			P2_6=light;
			P2_7=light;
			
			Nixie(1,11);
			Nixie(2,0);
			break;
		

	}

}
void main()
{
	Timer0Init();
	while(1)
	{
		KeyNumber=MatrixKey();
		test();
	}
}

void Timer0_Routine(void) interrupt 1
{
		
		static unsigned int T0Count1,T0Count2;
		TL0 = 0x66;		
		TH0 = 0xFC;
		T0Count1++;
		T0Count2++;
		
		if(T0Count2 >= 100) 
		{ 
        T0Count2 = 0;
        ADValue = XPT2046_ReadAD(XPT2046_XP);
		
		}
		switch(mode)
		{
			case 0:
				if(T0Count1>=4000)
				{
						T0Count1=0;
						mode=1;
				}
				break;
			case 1:
				if(T0Count1>=500+ADValue)
				{
						T0Count1=0;
						Number++;
						if(Number>8)
						{
							Number=0;
						}
						
						if(Number==7)
						{
							count++;
							if(count>99)
							{
								count=0;
								
							}
						}
						
						mode=1;
						
				}
				break;
			case 2:
			if(T0Count1>=500+ADValue)
			{
					T0Count1=0;
					Number++;
					if(Number>8)
					{
						Number=0;
					}
					
					if(Number==7)
					{
						count++;
						if(count>99)
						{
							count=0;
							
						}
						Buzzer_Time(100);
					}
					
					
		}
		break;
		case 3:
			if(T0Count1>=1000)
			{
					T0Count1=0;
					light=!light;

			}
			break;
	}
		
}