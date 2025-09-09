#include <stdint.h>
#include "tm4c123gh6pm.h"

#define STCTRL      (*((volatile uint32_t *)0xE000E010))
#define STRELOAD    (*((volatile uint32_t *)0xE000E014))
#define STCURRENT   (*((volatile uint32_t *)0xE000E018))

#define RED_LED     (1U << 1)   // PF1
#define BLUE_LED    (1U << 2)   // PF2
#define SW1         (1U << 4)   // PF4

#define ENABLE      (1U << 0)
#define CLK_SRC     (1U << 2)
#define COUNTFLAG   (1U << 16)

int main(void) {
    // Enable PORTF clock
    SYSCTL_RCGCGPIO_R |= 0x20;
    while((SYSCTL_PRGPIO_R & 0x20) == 0);

    // Configure PF1 (Red), PF2 (Blue) as output
    GPIO_PORTF_DIR_R |= (RED_LED | BLUE_LED);
    GPIO_PORTF_DEN_R |= (RED_LED | BLUE_LED | SW1);

    // Configure PF4 (SW1) as input with pull-up
    GPIO_PORTF_DIR_R &= ~SW1;
    GPIO_PORTF_PUR_R |= SW1;

    STRELOAD = 0xF42400;
    STCURRENT = 0;
    STCTRL = ENABLE | CLK_SRC;

    while (1) {
        // Check if 2 sec passed → toggle RED LED
        if (STCTRL & COUNTFLAG) {
            GPIO_PORTF_DATA_R ^= RED_LED;
        }

        // Check switch state → control BLUE LED
        if ((GPIO_PORTF_DATA_R & SW1) == 0) {
            GPIO_PORTF_DATA_R |= BLUE_LED;   // ON
        } else {
            GPIO_PORTF_DATA_R &= ~BLUE_LED;  // OFF
        }
    }
}
