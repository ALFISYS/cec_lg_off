/*
 * This file is part of cec_ir_off.
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
*/

inline void cec_low(uint16_t delay)
{
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  
  delayMicroseconds(delay);
}

inline void cec_high(uint16_t delay)
{
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  
  delayMicroseconds(delay);
}

void setup() 
{
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  cec_high(0);
}

void cec_send( uint8_t bytes[], uint8_t length ) 
{
  // Start Bit
  cec_low(3700);
  cec_high(4500-3700);

  for (uint8_t i_byte = 0; i_byte < length; i_byte++)
  {
    uint8_t b = bytes[i_byte];
    for (uint8_t i_bit = 0; i_bit < 10; i_bit++)
    {
      uint8_t bit = 1;
      if (i_byte == length - 1 && i_bit == 8) // EOM
      {
        bit = 1;
      }
      else if (i_bit < 8)
      {
        bit = b >> 7 & 0x01;
        b = b << 1;
      }
      else if (i_bit == 9) {
        bit = 0;
      } else 
      {
        bit = 0;  
      }

      if (bit == 1)
      {
        cec_low(600);
        cec_high(2400-600);
      }
      else
      {
        cec_low(1500);
        cec_high(2400-1500);
      }
    }
  }

}



void loop() {
  byte off[] = {0xF0, 0x36};
  
  
  cec_send(off, 2);
  delay(1000);

  //byte png[] = {0xF0, 0x00, 0x00, 0x00, 0x00, 0x00};
  //cec_send(png, 6);
  //delay(3000);
  
  

}
