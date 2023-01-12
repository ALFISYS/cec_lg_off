/*
 * This file is part of cec_lg_off.
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/

/**
 * Power Off type setting.
 * 
 * Currently, cec_lg_off can turn off LG TVs:
 *     * Sending command via UART 
 */

// Enable UART command to be sent when CEC STANDBY is detected
//      Comment line to disable UART command
#define ENABLE_PWROFF_UART
#define PWROFF_UART_CMD ((uint8_t *)"ka 00 00\r")
