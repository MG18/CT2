/* ----------------------------------------------------------------------------
 * --  _____       ______  _____                                              -
 * -- |_   _|     |  ____|/ ____|                                             -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems              -
 * --   | | | '_ \|  __|  \___ \   Zurich University of                       -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                           -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland               -
 * ----------------------------------------------------------------------------
 * --
 * -- Project     : CT2 Lab GPIO
 * -- Description : Configure and use GPIO port B as output and 
 * --               GPIO port A is input.
 * --               Lab version without additional hardware 
 * --               except for three wires.
 * --
 * -- $Id: main.c ostt $
 * ------------------------------------------------------------------------- */

#include <stdint.h>
#include <reg_stm32f4xx.h>
#include <string.h>
#include "reg_ctboard.h"

/* user macros */
/* Re-definition guard */
#define MASK_3_BITS  0x0007

static void init_GPIOA(void);
static void init_GPIOB(void);
static void config_check_GPIOA(void);
static void reset_GPIO(void);
static void request_abort_startup(void);


/* ----------------------------------------------------------------------------
 * Main
 * ------------------------------------------------------------------------- */

int main(void)
{
    uint16_t data_gpio_in;              // use to read input values from gpio
		uint16_t data_gpio_out;
    uint8_t data_dip_switch;            // use to read values from dip switches
		uint8_t state_gpio;

    GPIOA_ENABLE();                     // peripheral clk enable
    GPIOB_ENABLE();                     // peripheral clk enable
		
		//Reset the GPIOA and GPIOB to it's base value
		reset_GPIO();
		request_abort_startup();
		
		//Init functions
		init_GPIOA();
		config_check_GPIOA();
		init_GPIOB();

    while (1) {
        /* implement tasks 6.1 to 6.2 below */
        /// STUDENTS: To be programmed
		data_dip_switch = CT_DIPSW->BYTE.S15_8;
    /* 6.1: read PA2..PA0 */
    data_gpio_in = (uint16_t)(GPIOA->IDR & MASK_3_BITS);

    /* show on LED18..LED16 -> liegt im Byte LED23_16, Bits 2..0 */
    CT_LED->BYTE.LED23_16 = (uint8_t)((CT_LED->BYTE.LED23_16 & (uint8_t)~MASK_3_BITS) |
                                      (uint8_t)(data_gpio_in & MASK_3_BITS));
			
			
		/* write to PB2..PB0 via ODR (keep other PB bits unchanged) */
		
		GPIOB->ODR = (GPIOB->ODR & ~(uint32_t)MASK_3_BITS) | (uint32_t)(data_dip_switch & MASK_3_BITS);

		/* read back from ODR and show on LED2..LED0 (bits 2..0 of LED7_0) */
		data_gpio_out = (uint8_t)(GPIOB->ODR & MASK_3_BITS);

		CT_LED->BYTE.LED7_0 =
				(uint8_t)((CT_LED->BYTE.LED7_0 & (uint8_t)~MASK_3_BITS) | (data_gpio_out & MASK_3_BITS));


        /// END: To be programmed
    }
}

/* configure GPIOA pins
* clear register bits: GPIOA->xxxx &= ~(clear_mask << bit_pos);
* set register bits:   GPIOA->xxxx |=  (set_value << bit_pos);
* On GPIOA only pins 11 down to 0 are available to the user. 
* Pins 15 down to 12 are used for system functions of the discovery board, 
* e.g. connection of the debugger.
* These pins must not be reconfigured. 
* OTHERWISE THE DEBUGGER CANNOT BE USED ANY MORE!!!
*/
static void init_GPIOA(void)
{
		/* 6.1 define inputs */
    /// STUDENTS: To be programmed

    /* MODER: PA0..PA2 = input (00)*/
    GPIOA->MODER &= ~(0x03u << (0u * 2u));   // PA0
    GPIOA->MODER &= ~(0x03u << (1u * 2u));   // PA1
    GPIOA->MODER &= ~(0x03u << (2u * 2u));   // PA2

    /* PUPDR:
       PA0 = pull-down (10)
       PA1 = pull-up   (01)
       PA2 = no pull   (00)
    */
    GPIOA->PUPDR &= ~(0x03u << (0u * 2u));   // clear PA0
    GPIOA->PUPDR |=  (0x02u << (0u * 2u));   // set   PA0 pull-down

    GPIOA->PUPDR &= ~(0x03u << (1u * 2u));   // clear PA1
    GPIOA->PUPDR |=  (0x01u << (1u * 2u));   // set   PA1 pull-up

    GPIOA->PUPDR &= ~(0x03u << (2u * 2u));   // clear PA2
    /* set = 0x00 -> nichts zu setzen */

    /// END: To be programmed
		
}

/* configure GPIOB pins
* clear register bits: GPIOB->xxxx &= ~(clear_mask << bit_pos);
* set register bits:   GPIOB->xxxx |=  (set_value << bit_pos);
* On GPIOB only pins 11 down to 0 are available to the user. 
* Pins 15 down to 12 are used for system functions of the discovery board, 
* e.g. connection of the debugger.
* These pins must not be reconfigured. 
* OTHERWISE THE DEBUGGER CANNOT BE USED ANY MORE!!!
*/
static void init_GPIOB(void)
{
		/* 6.2 define outputs */
    /// STUDENTS: To be programmed

    GPIOB->MODER &= ~(0x03u << (0u * 2u));
    GPIOB->MODER |=  (0x01u << (0u * 2u));

    GPIOB->MODER &= ~(0x03u << (1u * 2u));
    GPIOB->MODER |=  (0x01u << (1u * 2u));

    GPIOB->MODER &= ~(0x03u << (2u * 2u));
    GPIOB->MODER |=  (0x01u << (2u * 2u));

    /* OTYPER: PB0 push-pull (0), PB1/2 open-drain (1)
       (OTYPER has 1 bit per pin!) */
    GPIOB->OTYPER &= ~(0x01u << 0u);        // PB0 = 0
    GPIOB->OTYPER |=  (0x01u << 1u);        // PB1 = 1
    GPIOB->OTYPER |=  (0x01u << 2u);        // PB2 = 1

    /* PUPDR: PB0 none (00), PB1 none (00), PB2 pull-up (01) */
    GPIOB->PUPDR &= ~(0x03u << (0u * 2u));  // PB0
    GPIOB->PUPDR &= ~(0x03u << (1u * 2u));  // PB1
    GPIOB->PUPDR &= ~(0x03u << (2u * 2u));  // PB2
    GPIOB->PUPDR |=  (0x01u << (2u * 2u));  // PB2 pull-up

    /* OSPEEDR: PB0 low (00), PB1 medium (01), PB2 high (10) */
    GPIOB->OSPEEDR &= ~(0x03u << (0u * 2u));           // PB0 low -> 00

    GPIOB->OSPEEDR &= ~(0x03u << (1u * 2u));
    GPIOB->OSPEEDR |=  (0x01u << (1u * 2u));           // PB1 medium -> 01

    GPIOB->OSPEEDR &= ~(0x03u << (2u * 2u));
    GPIOB->OSPEEDR |=  (0x02u << (2u * 2u));           // PB2 high -> 10


    /// END: To be programmed

}

/* checking config of GPIOA
* If the GPIOA is configured as output instead of input,
* the programm will show an error message on the lcd and goes
* in an endless loop to prevent shortcircuits.
*/
static void config_check_GPIOA(void)
{

	if(GPIOA->MODER != 0xa8000000)
	{
    CT_LCD->BG.RED = 0xffff;
    CT_LCD->BG.GREEN = 0x0000;
    CT_LCD->BG.BLUE = 0x0000;
		
		strcpy((void*)CT_LCD->ASCII, "Config error!       Reconfig needed!");
		
		while(1);
    
	}
}

/* reset GPIOA & GPIOB
*/
static void reset_GPIO(void)
{
    /* Reset GPIOA specific values */
    GPIOA->MODER = 0xa8000000;           // mode register
    GPIOA->OSPEEDR = 0x00000000;         // output speed register
    GPIOA->PUPDR = 0x64000000;           // pull resistor register
    GPIOA->OTYPER = 0x00000000;          // type register (pp / f. gate)
    GPIOA->AFRL = 0x00000000;
    GPIOA->AFRH = 0x00000000;
    GPIOA->ODR = 0x00000000;             // output data register
    
    /* Reset GPIOB specific values */
    GPIOB->MODER = 0x00000280;           // mode register
    GPIOB->OSPEEDR = 0x000000c0;         // output speed register
    GPIOB->PUPDR = 0x00000100;           // pull resistor register
    GPIOB->OTYPER = 0x00000000;          // type register (pp / f. gate)
    GPIOB->AFRL = 0x00000000;
    GPIOB->AFRH = 0x00000000;
    GPIOB->ODR = 0x00000000;             // output data register	
}


/* reset of debug pins
* After a reset it checks for a short while if T0-T3 is pressed.
* If one is pressed it goes into a endless loop so it doesn't load
* a wrong config and the MCU stays programmable. 
*/
static void request_abort_startup(void) 
{
  strcpy((void*)CT_LCD->ASCII, "Stop: any Tx button");
  for (size_t i = 0; i < 1000000; ++i) {
    if (CT_BUTTON & 0xF) {
      while(1) {
        strcpy((void*)CT_LCD->ASCII, "Stopped -> reload  ");
      }
    }
  }
  strcpy((void*)CT_LCD->ASCII, "                   ");
}
