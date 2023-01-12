/*
 * This file is part of cec_lg_off.
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/

/**
 * WARNING!
 * When connecting this board to anything except HDMI, physically disconnect HDMI. You can damage your TV.
 * 
 * PINS Used: 
 * * PB2 (P2 on Digispark USB) - HDMI CEC Line, connected to HDMI Pin 13 (CEC)
 * * PB0 (P0 on Digispark USB) - TX. Connected to RX on PL2303
*/
#include "./config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <inttypes.h>
#include <avr/sleep.h>
#include "./cec_mon.h"
#include "./uart_send.h"





void setup();
void disable_wdt();
volatile void process_cec_message(tinycec_message_t *, uint8_t, uint8_t);


#define SEND_NOTHING   ((uint8_t)0)
#define SEND_POWER_CMD ((uint8_t)1)


uint8_t send_status_flag = 0;

int main() 
{
  setup();


  
  send_status_flag = SEND_NOTHING;
  tinycec_start(process_cec_message);
  sei();
  do 
  {
    if (send_status_flag == SEND_POWER_CMD) 
    {
      PORTB |= (uint8_t)(1 << PB1); // Indicator ON
      cli();
      send_status_flag = SEND_NOTHING;
      send_uart_cmd(PWROFF_UART_CMD);
      sei();
      PORTB &= (uint8_t)~(1 << PB1); // Indicator OFF
    }
    sleep_enable();
    sleep_cpu();
    sleep_disable();
  }
  while(1);
}

uint8_t tmp_buf[256];
volatile void process_cec_message(tinycec_message_t * message, uint8_t message_len, uint8_t message_ack)
{
  //send_ir_cmd(0);
  if (
    message_len > 0 &&
    message->data.opcode == CEC_OPCODE_STANDBY &&
    (
      message->data.destination_addr == CEC_ADDRESS_TV ||
      message->data.destination_addr == CEC_ADDRESS_BROADCAST))
  {
    send_status_flag = SEND_POWER_CMD;
  }
}


void disable_wdt()
{
  wdt_reset();
  MCUSR = (uint8_t)0x00;
  // WDT should be disabled exactly this way.
  WDTCR |= (uint8_t)((1<<WDCE) | (1<<WDE));
  WDTCR = (uint8_t)0x00; // This command must be executed within 4 cycles after previous
}


void setup()
{
  // Disable all interrupts while setup
  cli(); 

  // Disable watchdog.
  disable_wdt(); 

  // Disable power for USI, ADC, Timer1 subsystems.
  PRR = (uint8_t)((1 << PRADC) | (1 << PRUSI) |  (1 << PRTIM1)); 

  // Disable Pull-Up resistor on all pins & set Pin Change interrupts to fire at any pin change
  MCUCR |= (uint8_t)((1 << PUD) | (1 << ISC00)); 

  // Set PB0=TX as output, PB1=LED as output, others (including PB2=CEC) are input.
  DDRB = (uint8_t)((1 << PB0) | (1 << PB1)); 

  // Set PB0 to high
  PORTB |= (uint8_t)(1 << PB0);

  // Enable interrupts for PB2=CEC pin
  PCMSK = (1 << PB2);

  set_sleep_mode(SLEEP_MODE_IDLE);
}
