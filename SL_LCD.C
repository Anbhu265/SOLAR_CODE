#include <LPC21XX.h>   // Header for LPC2129 register definitions

#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include "LCD.H" 

// Voltage divider resistor values
float R1 = 30000.0f;
float R2 = 7500.0f;
#define LDR_PIN 1<<19     // GPIO pin for LDR (P0.30)
#define LED_PIN 0XFF//street light
#define relay 1<<21// voltage cut off
// Serial communication baud rate
//#define BAUD 9600

// ADC input channel (Assuming ADC0.2 corresponds to pin P0.29)
//#define ADC_CHANNEL 2

// Variables for voltage and ADC reading
float adc_voltage = 0.0f;
float in_voltage = 0.0f;
unsigned int adc_value = 0;


// Function to initialize ADC
void ADC_Init() {
    // Enable ADC, select 10-bit resolution (you can change this if needed)
    ADCR = 0x00200604;  // PDN=1, CLKDIV=6, channel=AD0.2 (P0.29)
}



float ADC_Conversion()
{
	

 int ab;                                /* Variable to store ADC value         */
 delay_ms(1000);
 ADCR  = ADCR|0x01000000;               /* Start conversion                    */
 while((ADDR&0x80000000)!=0x80000000);  /* Wait here till conversion is over   */ 
 ab    = (ADDR&0x0000FFC0);             /* Extracting the result               */
 ab    = (ab>>6);                       /* Shift 6 bits right                  */
 
 return (3.3*ab/1023);


	
}
int main() {
	char buffer[50];
	volatile int i;
	IODIR0|=LED_PIN|relay;
	IOSET0|=LED_PIN;
	IOCLR0|=relay;
	
   ADC_Init();
   LCD_Init();
	 lcdcmd(0X80);
	lcdstring("   WELCOME TO ");
	 delay_ms(1000);
	 lcdcmd(0X01);
  lcdcmd(0X80);	
	lcdstring("SMART SOLAR STREET ");
	 delay_ms(1000);
	lcdcmd(0XC0);	
	lcdstring("LIGHT CONTROL SYSTEM");
	 delay_ms(1000);
	 lcdcmd(0X01);
	
   
   
    while (1) {
			
			if((IOPIN1&LDR_PIN)==0)
			{
     IOSET0 = LED_PIN;
			lcdcmd(0X01);
		  lcdcmd(0X80);
		 lcdstring("Turn OFF LIGHT ");
			}
     else
		 {
			 lcdcmd(0X01);
			lcdcmd(0X80);
		 lcdstring("Turn ON LIGHT ");
     IOCLR0=LED_PIN;
     }
        // Perform ADC conversion and get the ADC voltage
        adc_voltage = ADC_Conversion();

        // Calculate input voltage using the voltage divider formula
        in_voltage = adc_voltage / (R2 / (R1 + R2));

        // Print the results
        
        snprintf(buffer, sizeof(buffer), " %.2f V", in_voltage);
       // UART_WriteString(buffer);
		 lcdcmd(0Xc0);
		 lcdstring("Volt : ");
		 lcdcmd(0Xc7);
		 lcdstring(buffer);
		 delay_ms(5000);
     /* voltage cut off logic*/ 
			if(in_voltage>=8.0)
			{
			 IOCLR0=relay;	//relay off 
			}else
			{
				IOSET0=relay;//relay on
			}
			
	 }
        
      
    }

  

