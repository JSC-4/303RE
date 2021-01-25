/* 
 * This program is a basic UART Transmitter sketch for the STM32F303RE board.
 * The datasheet used for this is the RM0316 and STM32F303xE datasheet.
 * Author: Jerome Samuels-Clarke
 * Website: www.jscblog.com
 * Last Modified 15/11/20
 */
 
#include "stm32f303xe.h"

void USART2_Init(void);
void USART_Write(int ch);

int main ()
{

	USART2_Init();

	while (1)
	{
		USART_Write('H');
		USART_Write('I');
		for (int i=0; i<500000; i++);
	}
}

void USART2_Init(void)
{
	// Enable USART 2
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
		
	// Turn on Port A clock for USART pins PA2 (TX) and PA3 (RX)
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	// Set PA2 (TX) and PA3 (RX) as Alternate Functions
	GPIOA->MODER &= ~(GPIO_MODER_MODER3 | GPIO_MODER_MODER2);
	GPIOA->MODER |= GPIO_MODER_MODER3_1 | GPIO_MODER_MODER2_1;
	
	// Conifgure Alternate Function Register for PA3 and PA2
	GPIOA->AFR[0] |= (0x7 << 12) | (0x7 << 8);	
	
	USART2->BRR = 0x341;			// Set baud rate to 9600 (8Mhz/9600)
	USART2->CR1 |= USART_CR1_TE;	// Enable transmitter
	USART2->CR1 |= USART_CR1_UE;	// Enable USART2
}

void USART_Write(int ch)
{
	while (!(USART2->ISR & USART_ISR_TXE)); // Check to see if transmit data register is empty
	USART2->TDR = (ch & 0xFF); // Load TX data register with ch
}
