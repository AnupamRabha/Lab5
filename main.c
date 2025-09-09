#include <stdint.h>
#include "tm4c123gh6pm.h"

#define STCTRL      (*((volatile uint32_t *)0xE000E010))
#define STRELOAD    (*((volatile uint32_t *)0xE000E014))
#define STCURRENT   (*((volatile uint32_t *)0xE000E018))

#define RED_LED   (1U << 1)
#define ENABLE    (1U << 0)
#define CLK_SRC   (1U << 2)
#define COUNTFLAG (1U << 16)

int main(void) {
    // Enable PORTF clock
    SYSCTL_RCGCGPIO_R |= 0x20;
    GPIO_PORTF_DIR_R |= RED_LED;
    GPIO_PORTF_DEN_R |= RED_LED;


    STRELOAD = 0xF42400;
    STCTRL = CLK_SRC;

    while (1) {
        STCURRENT = 0;       // clear timer
        STCTRL |= ENABLE;


        while ((STCTRL & COUNTFLAG) == 0);

        GPIO_PORTF_DATA_R ^= RED_LED;  // Toggle LED
    }
}
