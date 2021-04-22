#include <stdint.h>

// Periphery base address
#define PERIPH_BASE 0x40000000
// AHB! periphery bus address
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x20000)
// GPIOA address
#define GPIOA_BASE (AHB1PERIPH_BASE+0x0)
// GPIOC address
#define GPIOC_BASE (AHB1PERIPH_BASE + 0x800)

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

#define GPIOA ((GPIO_Type_Def *) GPIOA_BASE)
#define GPIOC ((GPIO_Type_Def *) GPIOC_BASE)

int main(void)
{
	// Enable bus for periphery
	*((uint32_t *) 0x40023830) |= 1 | 4;
	// variables for red and blue button state
	uint32_t r_button, b_button, i, counter=0;

	// Configure GPIOC8, 9 to output (LEDs)
	GPIOC->MODER |= 1 << 8*2 | 1 << 9*2;

	while (1)
	{
		// read red button state
		r_button = GPIOC->IDR & (0x1 << 5);
		// if red button pressed
		if (r_button==0)
			{
				counter--;
				for(i=0; i<1000000; i++);
			}

		// read blue button state
		b_button = GPIOC->IDR & (0x1<<6);
		// if blue button pressed
		if (b_button==0)
			{
				counter++;
				for(i=0; i<1000000; i++);
			}

		// 2 LEDs indicate counter value as 2-bit indicator
		switch (counter)
			{
				case 0: GPIOC->ODR = 0x0; break;
				case 1: GPIOC->ODR = (1<<8); break;
				case 2: GPIOC->ODR = (1<<9); break;
				case 3: GPIOC->ODR = (1<<8)|(1<<9);

			}
	}
}
