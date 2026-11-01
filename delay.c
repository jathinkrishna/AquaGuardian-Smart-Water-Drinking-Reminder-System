//delay.c
void delay_ms(unsigned int ms)
{
	int i,j;
	for(i=0;i<ms;i++)
	{
		for(j=0;j<6000;j++);
	}
}


