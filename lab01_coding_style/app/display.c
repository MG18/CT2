/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module lcd
 * --               Performs all the interactions with the lcd
 * --
 * -- $Id: lcd.c 5144 2020-09-01 06:17:21Z ruan $
 * ------------------------------------------------------------------
 */

/* standard includes */
#include <stdio.h>
#include <reg_ctboard.h>
#include <hal_ct_seg7.h>

/* user includes */
#include "display.h"

/* macros */
#define LCD_ADDR_LINE1      0u
#define LCD_ADDR_LINE2      20u

#define NR_OF_CHAR_PER_LINE 20u

#define LCD_CLEAR           "                    "

/* function declarations */
/// STUDENTS: To be programmed


static void lcd_write_at_position(uint8_t position, const char text[]);


/// END: To be programmed

/* function definitions */

/*
 * see header file
 */
void display_write_throw(uint8_t dice_number)
{
   hal_ct_seg7_bin_write(dice_number); 
}

/// STUDENTS: To be programmed
static void lcd_write_at_position(uint8_t position, const char text[])
{
    uint8_t i = 0u;
		// set background green
		CT_LCD->BG.GREEN = (uint16_t) 0xFFF;
    /* LCD memory: 2 lines * 20 chars = 40 chars */
    while ((text[i] != '\0') && ((position + i) < (2u * NR_OF_CHAR_PER_LINE))) {
        CT_LCD->ASCII[position + i] = (uint8_t)text[i];
        i++;
    }
}

void display_write_value(uint8_t slot_nr, uint8_t value)
{
    /* slots 1..6 on line 1, 3 chars per slot -> positions 0,3,6,9,12,15 */
    if ((slot_nr < 1u) || (slot_nr > 6u)) {
        return;
    }

    char buf[4]; /* "255" + '\0' */
    (void)snprintf(buf, sizeof(buf), "%3u", (unsigned)value);

    lcd_write_at_position((uint8_t)((slot_nr - 1u) * 3u), buf);
}

void display_write_total(uint8_t total_value)
{
    char buf[NR_OF_CHAR_PER_LINE + 1u];

    (void)snprintf(buf, sizeof(buf), "Total: %3u", (unsigned)total_value);

    buf[NR_OF_CHAR_PER_LINE] = '\0';

    lcd_write_at_position(LCD_ADDR_LINE2, buf);
}


	
	



void display_clear(void)
{
    lcd_write_at_position(LCD_ADDR_LINE1, LCD_CLEAR);
    lcd_write_at_position(LCD_ADDR_LINE2, LCD_CLEAR);

}




/// END: To be programmed
