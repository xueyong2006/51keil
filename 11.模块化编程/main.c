#include <REGX52.H>
#include"Delay.h"
#include"NIXIE.h"
void main()
{
	while(1)
	{
		Nixie(3,1);
		Nixie(4,8);
		Nixie(5,8);
		Nixie(6,4);
	}
}
