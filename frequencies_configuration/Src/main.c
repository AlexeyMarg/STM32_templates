#include <stdint.h>
// library for registers definition
#include "stm32f446xx.h"

// frequency of external oscillator
#define Clock_Input 8000000
// frequency of microcontroller ticks
#define HCLK 100000000
// number of HCLK ticks for 1 tick of system timer
#define SysTicksClk 5000
#define Systicks_Prescaler HCLK/SysTicksClk
#define AHB_Prescaler 1
#define PCLK1 1000000 // APB2 peripheral frequency
#define PCLK2 1000000 // APB1 timer frequency
#define PLLM 4 // 2-63 integers
#define PLLN 100 // 50-432 integers
#define PLLP 0b00 // 00 - 2, 01 - 3, 10 - 6, 11 - 8


int main(void)
{
	uint32_t i=0;
	//FLASH_ACR_LATENCY_3WS
	FLASH->ACR |= (0x11<<2);
	//external oscillator on
	RCC->CR = RCC_CR_HSEON;
	// check for external oscillator ready bit
	while (((RCC->CR & RCC_CR_HSERDY) == 0) && (i<10000))
		{
			i++;
		}
	// AHB prescaller 1
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	//APB1, 2 prescaller equal to 1
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

	//Congfigure PLL (external source, prescallers PLLM, PLLP, multiplier PLLN)
	//RCC_PLLCFGR_PLLSRC_HSE - choise of external oscillator
	// RCC_PLLCFGR_PLLM - M prescaller, RCC_PLLCFGR_PLLN - N-prescaller, PLLCFGR_PLLP - P multiplier
	// clear above bits
	RCC->PLLCFGR &=~((1<<RCC_PLLCFGR_PLLSRC_Pos)|(0b11<<RCC_PLLCFGR_PLLP_Pos)|(0b111111111<<RCC_PLLCFGR_PLLP_Pos)|(0b11<<RCC_PLLCFGR_PLLN_Pos)||(0b111111<<RCC_PLLCFGR_PLLM_Pos));
	// write values to PLL
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE | (PLLM<<RCC_PLLCFGR_PLLM_Pos) | (PLLN<<RCC_PLLCFGR_PLLN_Pos) | (PLLP<<RCC_PLLCFGR_PLLP_Pos);

	// PLL start
	RCC->CR |= RCC_CR_PLLON;
	i=0;
	while (((RCC->CR & RCC_CR_PLLRDY) != 1) && (i<10000))
		{
			i++;
		}
	i=0;

	// Set Main clock switch to PLL source
	// SW - system clock switch: 00 - HSI, 01 - HSE, 10 - PLL_P, 11 - PLLR;
	// SWS - status of chosen mode
	RCC->CFGR |= 0b10<<RCC_CFGR_SW_Pos;

	// On GPIOC on bus AHB1
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	// Output mode for GPIOC8
	GPIOC->MODER |= 1 << 8*2;
	// Set 1 on GPIOC8
	GPIOC->ODR |=(1<<8);
	while (1);
}
