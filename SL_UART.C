#include <LPC21XX.h>   // Header for LPC2129 register definitions

#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>


// Voltage divider resistor values
float R1 = 30000.0f;
float R2 = 7500.0f;
#define LDR_PIN 1<<19     // GPIO pin for LDR (P0.30)
#define LED_PIN 1<<18
#define relay 1<<21// voltage cut off
// Serial communication baud rate
#define BAUD 9600

// ADC input channel (Assuming ADC0.2 corresponds to pin P0.29)
//#define ADC_CHANNEL 2

// Variables for voltage and ADC reading
float adc_voltage = 0.0f;
float in_voltage = 0.0f;
unsigned int adc_value = 0;

// Initialize UART for serial communication
void UART_Init() {
	
    // Set baud rate (Fdiv = (PCLK / (16 * BaudRate)))
   
	
   PINSEL0|=0X00000005;
	U0LCR=0X83;//DLAB=1 //put 1 in 1 and 2 bit --> selected as 8 bit
	U0DLL=97;
	U0DLM=0;
	U0LCR=0X03;//DLAB=0 //put 0 in 8th bit and put 1 in 1st and 2nd bit
}

// Function to send a character via UART
void UART_WriteChar(char ch) {
    while (!(U0LSR & 0x20));  // Wait for THRE (Transmit Holding Register Empty)
    U0THR = ch;  // Transmit character
}

// Function to send a string via UART
void UART_WriteString(const char *str) {
    while (*str) {
        UART_WriteChar(*str++);
    }
}

// Function to initialize ADC
void ADC_Init() {
    // Enable ADC, select 10-bit resolution (you can change this if needed)
    ADCR = 0x00200604;  // PDN=1, CLKDIV=6, channel=AD0.2 (P0.29)
}


void delay_ms(int msec)
{
	T0PR=15000-1;
	T0TCR=0X01;
	while(T0TC<msec);
	T0TCR=0X03;
	T0TCR=0X00;
}
float ADC_Conversion()
{
	

 int ab;                                /* Variable to store ADC value         */
 delay_ms(1000);
 ADCR  = ADCR|0x01000000;               /* Start conversion                    */
 while((ADDR&0x80000000)!=0x80000000);  /* Wait here till conversion is over   */ 
 ab    = (ADDR&0x0000FFC0);             /* Extracting the result               */
 ab    = (ab>>6);                       /* Shift 6 bits right                  */
 //return ab ; 	/* Return the result 	*/
 //	return ((ab*3.3/1023)*(1000/10));
 return (3.3*ab/1023);


	
}
int main() {
	char buffer[50];
	volatile int i;
	IODIR1|=LED_PIN|relay;
	IOSET1|=LED_PIN;
	IOCLR0|=relay;
	
    UART_Init();

    // Initialize ADC
    ADC_Init();

    // Send startup message to Serial Monitor
    UART_WriteString("DC Voltage Test on LPC2129\r\n");
   
    while (1) {
			
			if((IOPIN1&LDR_PIN)==0)
			{
     IOSET1 = LED_PIN;
			}
     else
		 {
     IOCLR1=LED_PIN;
     }
        // Perform ADC conversion and get the ADC voltage
        adc_voltage = ADC_Conversion();

        // Calculate input voltage using the voltage divider formula
        in_voltage = adc_voltage / (R2 / (R1 + R2));

        // Print the results
        
        snprintf(buffer, sizeof(buffer), "Input Voltage = %.2f V\r\n", in_voltage);
        UART_WriteString(buffer);
       /* voltage cut off logic*/ 
			if(in_voltage>=8.0)
			{
			 IOCLR0=relay;//relay off	
			}else
			{
				IOSET0=relay;//relay on
			}
			
			
	 }
        
      
    }

  

