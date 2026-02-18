/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zurich University of             -
 * --  _| |_| | | | |____ ____) |  Applied Sciences                 -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Description:  Implementation of module statistics
 * --               Collect and provide statistical data on throws
 * --               of the dice
 * --
 * -- $Id: statistics.c 2977 2016-02-15 16:05:50Z ruan $
 * --------------------------------------------------------------- */

/* user includes */
#include "statistics.h"
#include "dice.h"

/* variables visible within the whole module*/
// index 0:         total number of throws
// index 1 to 6:    number of throws for each digit
static uint8_t nr_of_throws[NR_OF_DICE_VALUES + 1] = {0};

/* function definitions */

/// STUDENTS: To be programmed
void statistics_add_throw(uint8_t throw_value)
{
    /* only count valid dice values */
    if ((throw_value >= 1u) && (throw_value <= NR_OF_DICE_VALUES)) {
        nr_of_throws[0]++;               /* total */
        nr_of_throws[throw_value]++;     /* per value */
    }
}

uint8_t statistics_read(uint8_t dice_number)
{
    if (dice_number <= NR_OF_DICE_VALUES) {
        return nr_of_throws[dice_number];
    }

    return ERROR_VALUE;
}




/// END: To be programmed
