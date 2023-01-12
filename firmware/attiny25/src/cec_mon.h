/*
 * This file is part of cec_lg_off.
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/


/**
 * CEC-MON software uses system timers.
 * Some constants depend on F_CPU and should be defined
*/

/**
 * CEC Timings (us):
 * 
 * Start bit starts CEC sequence. 
 * Normally, idle line is HIGH state.
 * Start bit starts as 
 *  * HIGH->LOW transition at Ts0, 
 *  * LOW->HIGH transition at Ts1 (3500-3900 us)
 *  * HIGH-LOW transition at Ts2 (4300-4700 us)
 * 
 * Last fall of bit is start of following bit
 * 
 *      START BIT
 *      |....           .________________.
 *      |   |___________|                |....
 *      | 
 *      |   +-------[---^---]--------[---^---]
 *          0    3500-3700-3900    4300-4500-4700
 *
 * 
 *      DATA BIT
 *      |....           .________________.
 *      |   |___________|                |....
 *      | 
 *      |   +-------[---^---]--------[---^---]
 *      0x0:0       400-600-800     2050-2400-2750
 *      0x1:0      1300-1500-1700   2050-2400-2750
 *
 * According to CEC doc, it is safe to get value at T = 850 us .. 1250 us period
 * 
 * So, we have constants:
 *      CEC_DB_LH_MIN    = 400   us
 *      CEC_DB_LH_D0_MAX = 800   us
 *      CEC_DB_LH_D1_MIN = 1250  us
 *      CEC_DB_LH_MAX    = 1700  us
 *      CEC_DB_HL_MAX    = 2050  us
 *      CEC_SB_LH_MIN    = 3500  us
 *      CEC_SB_LH_MAX    = 3900  us
 *      CEC_SB_HL_MIN    = 4300  us
 *      CEC_SB_HL_MAX    = 4700  us
  * 
 * Those ARE be converted to jiffles (timer ticks) using F_CPU (aka Fclk). There is one main rule:
 * * MAXIMUM (CEC_SB_HL_MAX = 4700) must not exceed 255 jiffles
 * 
*/



#include "./cec_mon_constants.h"
#include <inttypes.h>

#define _CEC_MON_STATE_WAIT_SB         ((uint8_t)0x00)
#define _CEC_MON_STATE_WAIT_DB         ((uint8_t)0x01)
#define _CEC_MON_STATE_WAIT_LAST_ACK   ((uint8_t)0x02)

#define _CEC_MON_MAX_MESSAGE_LEN ((uint8_t)16)
#define _CEC_MON_MAX_PAYLOAD_LEN ((uint8_t)(_CEC_MON_MAX_MESSAGE_LEN - 2))

#define _CEC_MON_LINE_LOW  0
#define _CEC_MON_LINE_HIGH 1

#define _CEC_MON_INRANGE(a, b, c) (((a) <= (b)) && ((b) <= (c)))
#define _CEC_MON_MAX(a, b)        ((a) > (b) ? (a) : (b))
#define _CEC_MON_TIMER_CUR_TICK   (TCNT0)

#define CEC_DB_LH_D0_MAX CEC_DB_LH_MAX
#define CEC_DB_LH_D1_MIN CEC_DB_LH_MIN

#define CEC_OPCODE_STANDBY (0x36)
#define CEC_ADDRESS_TV (0x0)
#define CEC_ADDRESS_BROADCAST (0xF)

typedef struct {
    uint8_t destination_addr : 4; // LOW  nibble of first byte in CEC header
    uint8_t initiator_addr   : 4; // HIGH nibble of first byte in CEC header
    uint8_t opcode;
    uint8_t payload[_CEC_MON_MAX_PAYLOAD_LEN];
} tinycec_message_structured_t;


typedef union {
    tinycec_message_structured_t data;
    uint8_t raw[_CEC_MON_MAX_MESSAGE_LEN];
} tinycec_message_t;


typedef volatile void (*cec_message_complete_function)(tinycec_message_t * message_bytes, uint8_t message_len, uint8_t message_ack);


typedef struct {
    tinycec_message_t     message;
    uint8_t byte_id       : 4; // Bytes are 0..15
    uint8_t bit_id        : 4; // Bits in byte 0..9. [0:7] are data bits, [8] is EOM, [9] is ACK
    uint8_t state         : 2; // 0 = Wait SB (Start Bit), 1 = Wait DB (Data Bit)
    uint8_t message_ack   : 1; // 1 = all bytes had ACK bit set.
    cec_message_complete_function on_message_complete;
} tinycec_line_t;


tinycec_line_t CEC_LINE;


volatile void tinycec_start(cec_message_complete_function on_message_complete) {
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0 = 0;
    OCR0A = 255;
    OCR0B = 255;
    TIMSK = 0;
    CEC_LINE.on_message_complete = on_message_complete;
    CEC_LINE.state = _CEC_MON_STATE_WAIT_SB;
    GIMSK |= (1 << PCIE);
}


inline void _tinycec_timeout_stop()  
{
    TCCR0B = 0;
}


volatile void _tinycec_timeout_start(uint8_t timeout_ticks)
{
    TCNT0 = 0;
    OCR0B = timeout_ticks;
    TIMSK |= (1 << OCIE0B);
    TCCR0B = T0CS;
}


volatile void _tinycec_line_error()
{
    _tinycec_timeout_stop();
    CEC_LINE.state = _CEC_MON_STATE_WAIT_SB;

}

ISR(TIMER0_COMPB_vect)
{
    cli();
    _tinycec_line_error();
    sei();
}


ISR(PCINT0_vect)
{

    cli();
    
    uint8_t new_line_state = (PINB >> PB2) & 1U;
    uint8_t tick = _CEC_MON_TIMER_CUR_TICK;
    _tinycec_timeout_stop();
    // HIGH->LOW
    if (new_line_state == _CEC_MON_LINE_LOW) 
    { 
        if (CEC_LINE.state == _CEC_MON_STATE_WAIT_SB) 
        {
            _tinycec_timeout_start(CEC_SB_LH_MAX); // Set timeout to 3900 us waiting input signal to rise.
        }
        else
        { 
            // Every other state has same length
            _tinycec_timeout_start(CEC_DB_LH_MAX); // Set timeout to 1700 us waiting input signal to rise.
        }
    } 
    // LOW->HIGH
    else 
    { 
        if (CEC_LINE.state == _CEC_MON_STATE_WAIT_SB) // Second part of Start Bit
        {
            if (_CEC_MON_INRANGE(CEC_SB_LH_MIN, tick, CEC_SB_LH_MAX)) 
            {
                CEC_LINE.message_ack = 1; // initial value. Will be reset to 0 on negative ACK
                CEC_LINE.bit_id = 0;
                CEC_LINE.byte_id = 0;
                CEC_LINE.state = _CEC_MON_STATE_WAIT_DB;
            }
            else // LINE ERROR!
            { 
                _tinycec_line_error();
            }
        } 
        // Second part of Data Bit
        else 
        {
            uint8_t bit = 255;
            if (_CEC_MON_INRANGE(CEC_DB_LH_D0_MIN, tick, CEC_DB_LH_D0_MAX)) 
            {
                bit = 0;
            } 
            else if (_CEC_MON_INRANGE(CEC_DB_LH_D1_MIN, tick, CEC_DB_LH_D1_MAX)) 
            {
                bit = 1;
            } 

            
            if (bit != 255) // Data bit was received successfully
            {
                if (CEC_LINE.bit_id < 8) 
                { // Data bit
                    CEC_LINE.message.raw[CEC_LINE.byte_id] = (CEC_LINE.message.raw[CEC_LINE.byte_id] << 1) | bit;
                } 
                else if (CEC_LINE.bit_id == 8 && bit == 1) // EOM bit, should be followed by ACK.
                { 
                        CEC_LINE.state = _CEC_MON_STATE_WAIT_LAST_ACK;
                } 
                else if (CEC_LINE.bit_id == 9 && bit == 0) // ACK bit
                { 
                    CEC_LINE.message_ack = 0;
                }


                if (CEC_LINE.bit_id == 9) // On 10th bit (ACK)
                {
                    // If it was last ACK bit or message length overflow, we fire a message
                    if (CEC_LINE.state == _CEC_MON_STATE_WAIT_LAST_ACK || CEC_LINE.byte_id == _CEC_MON_MAX_MESSAGE_LEN) 
                    {
                        _tinycec_timeout_stop();
                        // Do Job
                        CEC_LINE.on_message_complete(&CEC_LINE.message, CEC_LINE.byte_id + 1, CEC_LINE.message_ack);
                        // And return to wait next cec command

                        CEC_LINE.state = _CEC_MON_STATE_WAIT_SB;
                    } 

                    CEC_LINE.bit_id = 0;
                    CEC_LINE.byte_id++;
                } 
                else 
                {
                    CEC_LINE.bit_id++;
                }
            }
            else // LINE ERROR
            {
                _tinycec_line_error();
            }
        }
    }
    sei();
}