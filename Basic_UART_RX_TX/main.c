/*
 * This program is a basic echo server using USART2 for the STM32F303RE board.
 * The datasheet used for this is the RM0316 and STM32F303RE datasheet
 * Author: Jerome Samuels-Clarke
 * Website: www.jscblog.com
 * Last Modified 11/06/20
 */
 
#include "stm32f3xx.h"                  // Device headers

#define		LED			5
#define		BUTTON	13

void IO_Init(void);
void UART_Init(void);
void UART_Write(int ch);
char UART_Read(void);

int main()
{
	IO_Init();
	UART_Init();
	
	while(1)
	{
		UART_Write(UART_Read());
		for(int i = 0;i<500000; i++);
	}
	
}

void IO_Init(void)
{
	RCC->AHBENR |= (1 << 17) | (1 << 19); //Set clock for GPIOA and GPIOC
	
	GPIOA->MODER &= ~(0x3 << LED*2);	//Clear GPIOA mode register for PA5
	GPIOA->MODER |= (0x1 << LED*2);		//LED set as input
	
	GPIOC->MODER &= ~(0x3 << BUTTON*2);		//Button set as input
	GPIOC->PUPDR |= (0x1 << BUTTON*2);		//Button set as pull up
}

void UART_Init(void)
{
	RCC->APB1ENR |= (1 << 17);	//Set clock for USART2
	RCC->AHBENR |= (1 << 17); 	//Turn GPIOA clock on
	GPIOA->MODER &= ~(0x3 << 4) | (0x3 << 6); //Clear PA2 and PA3
	GPIOA->MODER |= (0x2 << 4) | (0x2 << 6); 	//Set PA2 (TX) and PA3 (RX) as AF
	GPIOA->AFR[0] |= (0x7 << 8) | (0x7 << 12);		//Configure AF for PA2 and PA3
	
	USART2->BRR = 0x45; //115200 baud rate
	USART2->CR1 |= (1 << 3) | (1 << 2) | (1 << 0); //Enable TX, RX and UE
}

void UART_Write(int ch)
{
	while(!(USART2->ISR & (1 << 7))){} //wait till TX data register is empty (P.946)
	
		USART2->TDR = ch & 0xFF; //send character to TX data register (P.949), 0xFF garuntees 8 bits will be sent
		
}

char UART_Read(void)
{
	while(!(USART2->ISR & (1 << 5))){} //Wait until "Read data register not empty" bit is set
	
	return USART2->RDR; //Return read data
	
}