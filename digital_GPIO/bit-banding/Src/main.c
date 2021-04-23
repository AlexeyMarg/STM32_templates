#include <stdint.h>

// Periphery base address
#define PERIPH_BASE 0x40000000
// AHB! periphery bus address
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x20000)
// GPIOC address
#define GPIOC_BASE (AHB1PERIPH_BASE + 0x800)


// GPIO registers struct
typedef struct
{
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFRH;
	volatile uint32_t AFRL;
} GPIO_Type_Def;

#define GPIOC ((GPIO_Type_Def *) GPIOC_BASE)

//bit banding region addresses
#define PERIPH_BB_BASE	0x42000000
// definition of buttons and
#define led_red *((uint32_t *)(PERIPH_BB_BASE + ((uint32_t)&(GPIOC->ODR) - PERIPH_BASE)*32 + 9*4)) // bit 9
#define led_blue *((uint32_t *)(PERIPH_BB_BASE + ((uint32_t)&(GPIOC->ODR) - PERIPH_BASE)*32 + 8*4)) // bit 8
#define button_red *((uint32_t *)(PERIPH_BB_BASE + ((uint32_t)&(GPIOC->IDR) - PERIPH_BASE)*32 + 5*4)) // bit 5
#define button_blue *((uint32_t *)(PERIPH_BB_BASE + ((uint32_t)&(GPIOC->IDR) - PERIPH_BASE)*32 + 6*4)) // bit 6

int main(void)
{
	*((uint32_t *) 0x40023830) |= 1 | 4;
	uint32_t r_led, b_led, i, counter=0;

	GPIOC->MODER |= 1 << 8*2 | 1 << 9*2;

	while (1)
	{
		if (button_red==0)
			{
				counter--;
				for(i=0; i<1000000; i++);
			}

		if (button_blue==0)
			{
				counter++;
				for(i=0; i<1000000; i++);
			}

		if (counter>3) counter=3;
		else if (counter<0) counter=0;

		switch (counter)
			{
				case 0: led_blue=0; led_red=0; break;
				case 1: led_blue=1; led_red=0; break;
				case 2: led_blue=0; led_red=1; break;
				case 3: led_blue=1; led_red=1;

			}
	}
}
