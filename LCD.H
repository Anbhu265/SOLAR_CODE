#include<lpc21xx.h>
//#include"delay.h"
#define LCD_D 0xf<<14
#define RS 1<<12
//#define RW 1<<18
#define E 1<<13


typedef unsigned char U8;
void delay_ms(int);
void lcdcmd(U8);
void lcddata(U8);
void lcdconf(void);
void lcdstring(char*);
void lcdint(int);
void lcdfloat(float);
void LCD_STRING_ROTATE(unsigned char *);
void delay_ms(int msec)
{
	T0PR=15000-1;
	T0TCR=0X01;
	while(T0TC<msec);
	T0TCR=0X03;
	T0TCR=0X00;
}

	 
  void lcdcmd(unsigned char cmd)
{

	
	IOCLR0=LCD_D;
	IOSET0=(cmd&0xf0)<<10;
	IOCLR0=RS;
	//IOCLR0=RW;
	IOSET0=E;
	delay_ms(2);
	IOCLR0=E;
	
	IOCLR0=LCD_D;
	IOSET0=(cmd&0x0f)<<14;
	IOCLR0=RS;
	//IOCLR0=RW;
	IOSET0=E;
	delay_ms(2);
	IOCLR0=E;
}
void LCD_Init(void)
{
	IODIR0|=LCD_D|RS|E;
	lcdcmd(0X01);
	lcdcmd(0X02);
	lcdcmd(0X0C);
	lcdcmd(0X28);
	lcdcmd(0X80);
}
void lcddata(unsigned char D)
{	
	 IOCLR0=LCD_D;
	IOSET0=(D&0xf0)<<10;
	IOSET0=RS;
	//IOCLR0=RW;
	IOSET0=E;
	delay_ms(2);
	IOCLR0=E;
	
	 IOCLR0=LCD_D;
	IOSET0=(D&0x0f)<<14;
	IOSET0=RS;
	//IOCLR0=RW;
	IOSET0=E;
	delay_ms(2);
	IOCLR0=E;
}

void lcdstring( char *s)
{
	/*U8 i;
	for(i=0; S[i]; i++)
	lcddata(S[i]);*/
	while(*s)
	lcddata(*s++);
}

void lcdint(int n)
{
	unsigned char a[5];
	signed char i=0;
	if(n==0)
		lcddata('0');
	else
	{
		if(n<0)
		{
			lcddata('-');
			n=-n;
		}
		while(n>0)
		{
			a[i++]=n%10;
			n=n/10;
		}
		for(--i;i>=0;i--)
			lcddata(a[i]+48);
		}
}
void lcdfloat(float f)
{
	int x=f;
	lcdint(x);
	lcddata('.');
	f=(f-x)*100;
	lcdint(f);
}
/*void lcdhex(U8 n)
{
	#include<stdio.h>
	U8 a[10];
	sprintf(a,"%x",n);
	lcdstring(a);
}*/

void lcdbcd(U8 n)
{
	lcddata((n/16)+48);
	lcddata((n%16)+48);
}
 
void LCD_STRING_ROTATE(unsigned char *p)

{

	int j,i,k=0;

	char temp;

   lcdcmd(0X01);

		for(i=0;p[i]!='\0';i++);

	while(k < i)

		{

			for(j=0;j<16;j++)

			lcddata(p[j]);

			p[i]=' ';

			temp=p[0];

			for(j=0;j<(i+16);j++)

			p[j]=p[j+1];

			p[j-1]=temp;

			//p[j]='\0';

		lcdcmd(0X80);

			k++;

		}
return;
}