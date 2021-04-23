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

}
