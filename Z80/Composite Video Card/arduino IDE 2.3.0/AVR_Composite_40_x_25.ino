/*
*          WARNING!!!!  I have developed and tested this software
*          on MODERN monitors..It works as expected on my equipment.. 
*          I make no guarantee and am not responsible for any damage
*          caused by it's use...
*
*           Composite Video on the AVR128DB48
*           Vernon Billingsley  c2024
*
*
*     Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission
*    notice shall be included in all copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*    THE SOFTWARE.
*   
*     9/2/24    Composite working on the AVR128DB48
*     9/14/24   Got the AVR to read from the IDT7130 memory chip.
*               Got the Z80 to wrote to the IDT7130
*               Data sent from the Z80
*     9/19/24   Added the function to reprogram charactors in the charset
*               by writing the number of the charactor you want to reprogram
*               to address 0x3F2, and the new charactor bytes to 0x3F3 to 0x3FB
*     9/20/24   Converted to 40 x 25 display..
*     9/28/24   Increased the char set to 256. This allows 127 programable charactors
*               above the standard set.
*               Added my chibi to the splash screen. 
*
*
*     Settings:
*     Arduino IDE 2.3.0
*     Board:        AVR DB-series(no bootloader)
*     Chip:         AVR128DB48
*     Clock Speed:  32 mHz internal (over clocked)
*     MulitVoltage: disabled
*     Sartup:       8ms          
*
*/


/************************ Includes ********************************/
#include "globals.h"
#include "isr.h"

#define chip_enable_low digitalWriteFast(PIN_PF2, LOW);      //PORTF.OUTCLR = (PIN2_bm);
#define chip_enable_high digitalWriteFast(PIN_PF2, HIGH);    //PORTF.OUTSET = (PIN2_bm);
#define output_enable_low digitalWriteFast(PIN_PF3, LOW);    //PORTF.OUTCLR = (PIN3_bm);
#define output_enable_high digitalWriteFast(PIN_PF3, HIGH);  //PORTF.OUTSET = (PIN3_bm);
#define chip_read digitalWriteFast(PIN_PB4, HIGH);           //PORTB.OUTSET = PIN4_bm;
#define chip_write digitalWriteFast(PIN_PB4, LOW);           //PORTE.OUTCLR = PIN4_bm;

/************************** Variables *****************************/


/**************************  Functions ****************************/
void write_byte(uint8_t data, uint16_t address) {
  /* Set data port to write */
  VPORTC.DIR = 0XFF;
  PORTC.PINCONFIG |= (1 << 0);
  NOP;
  NOP;
  NOP;
  NOP;
  /* Put address on the bus */
  VPORTD.OUT = address & 0xff;
  VPORTE.OUT = (address >> 8) & 0x03;
  NOP;
  NOP;
  NOP;
  NOP;
  PORTC.OUT = data;
  NOP;
  NOP;
  NOP;
  NOP;
  chip_write;
  NOP;
  NOP;
  chip_enable_low;
  NOP;
  NOP;
  NOP;
  NOP;
  NOP;
  NOP;
  NOP;
  NOP;
  NOP;
  NOP;
  NOP;
  NOP;
  chip_enable_high;
  NOP;
  NOP;
  chip_read;
  NOP;
  NOP;

  /* Set data port to read */
  VPORTC.DIR = 0X00;
  PORTC.PINCONFIG &= ~(1 << 0);
}

uint8_t read_byte(uint16_t address) {
  /* Read a byte from the chip */
  /* Put address on the bus */
  VPORTD.OUT = address & 0xff;
  VPORTE.OUT = (address >> 8) & 0x03;
  NOP;
  NOP;
  chip_enable_low;
  NOP;
  NOP;
  output_enable_low;
  NOP;
  NOP;
  NOP;
  NOP;
  uint8_t r_byte = VPORTC_IN;
  NOP;
  NOP;
  NOP;
  NOP;
  chip_enable_high;
  NOP;
  NOP;
  output_enable_high;
  NOP;
  NOP;
  return r_byte;
}

void check_program_byte() {
  /* Check the program byte at 0x3F2 */
  uint8_t p_byte = read_byte(0x3F2);
  /* If the byte is greater than 0, read the next 8 bytes and */
  /* store them in the charset byte register */
  if (p_byte > 0) {
    for (uint8_t x = 0; x < 8; x++) {
      charset[x][p_byte] = read_byte(0x3F3 + x);
    }
    /* Clear the program byte */
    write_byte(0x00, 0x3F2);
  }
}

void startup() {
  /* Clear chip on startup to remove garbage and insure */
  /* 0x3F2, charset reprogram bit, is clear.             */
  for (uint16_t x = 0; x < 1024; x++) {
    write_byte(0x00, x);
  }
  /* Put the splash screen into the video ram          */
  uint16_t memory_address = 0;
  for (uint8_t y = 0; y < 25; y++) {
    for (uint8_t x = 0; x < 40; x++) {
      write_byte(splash[y][x], memory_address);
      memory_address++;
    }
  }
  /* Load Chibi pixels */
  memory_address = 614;
  uint8_t char_address = 128;
  for (uint8_t x = 0; x < 4; x++) {
    for (uint16_t y = 0; y < 4; y++) {
      write_byte(char_address + y, memory_address + y);
    }
    char_address += 4;
    memory_address += 40;
  }
}

/******************************************************************/
/***************************  MAIN ********************************/
/******************************************************************/
int main() {
  /* Set the Internal HF Osc to RUN, at 32 mHz  */
  _PROTECTED_WRITE(CLKCTRL_OSCHFCTRLA, 0XAC);

  /************************* Setup Pins ***************************/
  /* Port B pin 0 to 5 as OUTPUT, pin 0 for H_Sync*/
  PORTB.DIRSET = 0x1F;
  /* Invert the pin */
  PORTB.PIN0CTRL |= (1 << 7);

  /* Set PORTA pin 7 as OUTPUT  for pixel clock        */
  /* Port A is used for pixel clock so all unused pins */
  /* are not available.                                */
  VPORTA.DIR |= (1 << 7);
  /* Port F pin 2 as OUTPUT for /output enable */
  PORTF.DIRSET = (PIN2_bm);
  /* Set HiGH to start */
  PORTF.OUTSET = (PIN2_bm);

  /* Port B pin 4 as OUTPUT for read/write */
  /* Set HIGH to read to start */
  digitalWriteFast(PIN_PB4, HIGH);

  /* Port F pin 3 as OUTPUT for /chip enable */
  VPORTF.DIR = 0x0F;  //|= (1 << 2);
  /* Set HIGH to start */
  VPORTF.OUT |= (1 << 3);

  /* Port C as INPUT for read data */
  VPORTC.DIR = 0X00;
  PORTC.PINCONFIG &= ~(1 << 0);

  /* Port D as OUTPUT for lower 8 bits of address */
  VPORTD.DIR = 0XFF;
  /* Port E as OUTPUT for upper 2 bits of adress, bits 0 & 1 */
  VPORTE.DIR = 0X0F;

  /* Setup TC0 to give a 6 uS pulse at 31.5 kHz */
  /* with an interrupt on compare               */
  PORTMUX.TCAROUTEA = (PORTMUX.TCAROUTEA & ~(PORTMUX_TCA0_gm)) | PORTMUX_TCA0_PORTB_gc;
  takeOverTCA0();                                                                 // this replaces disabling and resettng the timer, required previously.
  TCA0.SINGLE.CTRLB = (TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_WGMODE_SINGLESLOPE_gc);  // Single slope PWM mode, PWM on WO0
  TCA0.SINGLE.PER = 255;                                                          /* At 28 mHz = aprox 31.5 kHz */
  TCA0.SINGLE.CMP0 = 20;
  TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm; /* Enable interupt on CMP0 */
  /* Eable the timer with div 8 prescaler */
  TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm | TCA_SPLIT_CLKSEL_DIV8_gc;
  sei(); /* Enable global interrupts */

  /* Display the splash screen */
  startup();

  /******************************************************************/
  /**************************** WHILE *******************************/
  /******************************************************************/
  while (1) {
    static uint8_t x = 0;
    static uint8_t y = 0;
    static uint16_t memory_address = 0;

    /* If in the blanking period, house keeping */
    if (blanking == true) {

      /* Read a byte from the memory chip */
      uint8_t m_byte = read_byte(memory_address);
      /* Store the charsset of the input byte in the v_ram */
      /* Each screen row equals 8 display lines            */
      v_ram[y][x] = charset[0][m_byte];
      v_ram[y + 1][x] = charset[1][m_byte];
      v_ram[y + 2][x] = charset[2][m_byte];
      v_ram[y + 3][x] = charset[3][m_byte];
      v_ram[y + 4][x] = charset[4][m_byte];
      v_ram[y + 5][x] = charset[5][m_byte];
      v_ram[y + 6][x] = charset[6][m_byte];
      v_ram[y + 7][x] = charset[7][m_byte];

      /* Increment the memory addresss */
      memory_address++;
      /* Incremement the x, column     */
      x++;
      if (x == 40) { /* New line */
        x = 0;       /* Restart the line */
        y += 8;      /* 8 line per pixel */
        /* Check for screen bottom */
        if (y >= 200) {
          /* Restart the screen */
          y = 0;
          memory_address = 0;
        }
      }

      /* Check the program byte at 0x300  */
      check_program_byte();

    } else { /* end blanking */
      /* Reduce screen flutter */
      NOP;
      NOP;
      NOP;
      NOP;
      NOP;
      NOP;
      NOP;
      NOP;
    }


  } /*************************** End While Loop *****************************/
} /****************************End Main Loop ******************************/
