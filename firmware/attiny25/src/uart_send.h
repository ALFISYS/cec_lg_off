/*
 * This file is part of cec_lg_off.
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/

#include <avr/io.h>
#include <util/delay.h>

/**
 * LG TV communication over RS232 uses 9600-8-N-1 serial protocol (9600 baud, 8 bits per byte, no parity bits, 1 stop bit)
 * No other signals are used.
 * Power Off command looks like "ka 00 00" and sent as ASCII characters.
 * All commands end up with '\r' character
*/

#define UART_BIT_DELAY_US 1000000/9600

#define _UART_SEND_0() PORTB &= (uint8_t)~(1 << PB0)
#define _UART_SEND_1() PORTB |= (uint8_t)(1 << PB0)


void send_uart_cmd(const uint8_t  * command)
{
    while (command[0] != '\0')
    {
        _UART_SEND_0(); // START BIT
        _delay_us(UART_BIT_DELAY_US);
        for (uint8_t i = 0; i < 8; i++)
        {
            if ((command[0] >> i) & 0x01U)
            {
                _UART_SEND_1(); // DATA BIT
            }
            else
            {
                _UART_SEND_0(); // DATA BIT
            }
            _delay_us(UART_BIT_DELAY_US);
        }
        _UART_SEND_1(); // STOP BIT
        _delay_us(UART_BIT_DELAY_US);

        command++;
    }
}