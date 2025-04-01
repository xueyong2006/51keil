

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
