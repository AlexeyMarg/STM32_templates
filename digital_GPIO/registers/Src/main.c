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


int main(void)
{

}
