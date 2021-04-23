#include <stdint.h>
#include "stm32f446xx.h"

#define HCLK 76800000
#define APB1_FREQ  38400000 //PCLK1 for USART
#define InputFrequency 8000000
#define BAUDRATE 19200 // USART baudrate
// Prescallers definition
#define APB1_Prescaller 2
#define AHB_Prescaller 1
#define PLLM 5
#define PLLN 96
#define PLLP 0b00  //2

void InitFreq(void)
{
	uint32_t i;
	//FLASH_ACR_LATENCY_2WS
	FLASH->ACR |= FLASH_ACR_LATENCY_2WS;
	//external oscillator on
	RCC->CR = RCC_CR_HSEON;
	// check for external oscillator ready bit
	while (((RCC->CR & RCC_CR_HSERDY) == 0) && (i<10000))
		{
			i++;
		}

	// AHB prescaler 1
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	//APB1 prescaller equal to 2, APB2 prescaller equal to 1
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

	//Congfigure PLL (external source, two prescallers and multiplier)
	//RCC_PLLCFGR_PLLSRC_HSE - choise of external oscillator
	// RCC_PLLCFGR_PLLM - M prescaller, RCC_PLLCFGR_PLLN - N-prescaller, PLLCFGR_PLLP - P multiplier
	// clear above bits
	RCC->PLLCFGR &=~(RCC_PLLCFGR_PLLSRC_Msk | RCC_PLLCFGR_PLLP_Msk | RCC_PLLCFGR_PLLN_Msk | RCC_PLLCFGR_PLLM_Msk );
	// Set of oscillator and prescallers
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE | (PLLM<<RCC_PLLCFGR_PLLM_Pos) | (PLLN<<RCC_PLLCFGR_PLLN_Pos) | (PLLP<<RCC_PLLCFGR_PLLP_Pos);
	// PLL start
	RCC->CR |= RCC_CR_PLLON;
	i=0;
	while (((RCC->CR & RCC_CR_PLLRDY) != 1) && (i<10000))
		{
			i++;
		}

	// Set Main clock switch to PLL source
	// SW - system clock switch: 00 - HSI, 01 - HSE, 10 - PLL_P, 11 - PLLR;
	// SWS - status of chosen mode
	RCC->CFGR |= 0b10<<RCC_CFGR_SW_Pos;
}

// Turn on LED to indicate start
void CheckLed(void)
{
	// Enable GPIOC on bus AHB1
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	// Output mode for GPIOC8
	GPIOC->MODER |= 1 << 8*2;
	// Set 1 on GPIOC8
	GPIOC->ODR |=(1<<8);
}

// Interrupt handler
void USART2_IRQHandler(void)
{
	uint8_t data;
	// if data received
	if (USART2->SR & (USART_SR_RXNE))
		// read data
		data = USART2->DR;
	if (data < 255)
	{
		data+=1;
		// send modified data
		USART2->DR = data;
	}
	else USART2->DR = data;
}

void InitUsart(void)
{
	// Enable USART2 on bus APB1
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	// Enable pins of USART2 on GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// Choose pins alternate function mode
	GPIOA->MODER|= 0b10<<2*2 | 0b10<<3*2;
	// Choose USART2 alternate function
	GPIOA->AFR[0] |= 0b0111<<2*4 | 0b0111<<3*4;
	// Choose USART2 baudrate
	USART2->BRR = APB1_FREQ/BAUDRATE;
	// Usart, transmitter, receiver and interrupts for them enable
	USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;
}

int main(void)
{
	InitFreq();
	CheckLed();
	InitUsart();

	// enable interrupt handler for USART2
	NVIC_EnableIRQ(USART2_IRQn);
	while (1);
}
