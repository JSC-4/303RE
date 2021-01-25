/* 
 * This program is a basic UART Transmitter sketch for the STM32F303RE board.
 * The datasheet used for this is the RM0316 and STM32F303xE datasheet.
 * Author: Jerome Samuels-Clarke
 * Website: www.jscblog.com
 * Last Modified 15/11/20
 */
 
#include "stm32f303xe.h"

void USART2_Init(void);
void GPIO_LED_Init(void);
char USART_Read(void);

int main ()
{
	char ch;
	USART2_Init();
	GPIO_LED_Init();
	GPIOA->BSRR = (1 << 5);	
	
	while (1)
	{
		ch = USART_Read();
		if (ch == 'y'){
			GPIOA->BSRR = (1 << 5);
			for (int i=0; i<5000; i++);}
		else
		{
			//GPIOA->ODR ^= (1 << 5);
			GPIOA->BSRR = (1 << 21);
			//for (int i=0; i<5000; i++);
		}
	}
}

void USART2_Init(void)
{
	// Enable USART 2
	RCC->APB1ENR |= (1 << 17);
		
	// Turn on Port A clock for USART pins PA2 (TX) and PA3 (RX)
	RCC->AHBENR |= (1 << 17);
	
	// Set PA2 (TX) and PA3 (RX) as Alternate Functions
	GPIOA->MODER |= (0x2 << 6) | (0x2 << 4);
	
	// Conifgure Alternate Function Register for PA2 and PA3
	GPIOA->AFR[0] |= (0x7 << 12) | (0x7 << 8);	
	
	USART2->BRR = 0xEA6;			// Set baud rate to 9600 (8Mhz/9600)
	USART2->CR1 |= (1 << 2);	// Enable reciever
	USART2->CR1 |= (1 << 0);	// Enable USART2
}

void GPIO_LED_Init(void)
{
	// Turn on clock for GPIO A (LED is connected to PA5)
	//RCC->AHBENR |= (1 << 17);

	// configure PA5 to General Purpose Output Mode 
	GPIOA->MODER |= (~(1 << 11)) | (1 << 10);
	
}
char USART_Read(void)
{
	
	while (!(USART2->ISR & (1 << 5))){} // Check to see if recieve data register is empty
		return USART2->RDR; // Load TX data register with ch
}
