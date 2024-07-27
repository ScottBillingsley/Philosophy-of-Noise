Files for my RC2014 Z80 Sound Card...

*                           Cicada Sound Card
*                         Vernon Billingsley c2024
*                 An arduino based sound card for use on the Z80 
*                 single board computer..
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
*
*
*                             Arduino Nano
*                           -----------------
*              TXD PD1   --| D1              |-- Vin
*              RXD PD0   --| D0              |-- GND
*              Rest      --|                 |-- PC6 Reset
*              GND       --|                 |-- 5 volt
*              INT0 PD2  --| D2              |-- ADC7
*         OC2B INT1 PD3  --| D3              |-- ADC6
*                   PD4  --| D4         ADC5 |-- PC5 ADC5 SCL
*         OC0B      PD5  --| D5         ADC4 |-- PC4 ADC4 SDA
*         OC0A AIN0 PD6  --| D6         ADC3 |-- PC3 ADC3
*              AIN1 PD7  --| D7         ADC2 |-- PC2 ADC2
*              CLK0 PB0  --| D8         ADC1 |-- PC1 ADC1
*              OC1A PB1  --| D9         ADC0 |-- PC0 ADC0
*         SS   OC1B PB2  --| D10        AREF |--
*         MOSI OC2A PB3  --| D11             |-- 3.3 volt
*         MISO      PB4  --| D12         D13 |-- PB5 SCK LED
*                            ----------------
*
*
*                 Pins:
*                 Nano    Function
*                 0
*                 1
*                 2       Interrupt from address select   
*                 3       
*                 4       D0 Input    
*                 5       D1 Input
*                 6       D2 Input
*                 7       D3 Input
*                 8       
*                 9
*                 10
*                 11      PCM Wave Out
*                 12
*                 13
*                 A0      D4 Input     
*                 A1      D5 Input
*                 A2      D6 Input
*                 A3      D7 Input
*                 A4      
*                 A5      
*                 A6
*                 A7
*                 AREF 
*
*     Frequency Byte(s):
*               Tone Mode
*     Byte 1                
*     Bit 0   |               
*     Bit 1   |               
*     Bit 2   |--Low Nibble   0 to 31
*     Bit 3   |               
*     Bit 4   |             
*     Bit 5   0 = Add, 1 = Subtract               
*     Bit 6   1               
*     Bit 7   1               
*
*                 MIDI Mode (default)
*     10xxxxxx  = 0 to 52 decimal, 0 to 34 Hex
*               = MIDI start note A1, 55 Hz, is 0   0x80
*                           Middle C                0x9b
*               = MIDI note C6, 1046, Hz is 52      0xb3
*               = formula is 33 + n, where n is the MIDI note number minus 33
*
*     Command Byte:
*     00000000  = Note Off                    0x00
*     00000001  = Note On                     0x01
*     00000010  = Triangle Wave               0x02
*     00000011  = Saw Wave                    0x03
*     00000100  = Square Wave                 0x04
*     00000101  = AR Envelope Mode (default)  0x05
*     00000110  = Gain Envelope Mode          0x06        
*
*     00001111  = MIDI note mode(default)     0x0f
*     00010000  = Tone mode                   0x10
*     00100000  = Noise mode                  0x20
*                 
*
*     Envelope Byte:
*     01xxxxxx    0 to 63, 0 to 0x3F  Input 0x40 to 0x7f 
*                 31, 0x5f (default)
*
*     A1    0x80    A2    0x8C    A3    0x98    A4    0xA4    A5    0xB0
*     A#1   0x81    A#2   0x8D    A#3   0x99    A#4   0xA5    A#5   0xB1
*     B1    0x82    B2    0x8E    B3    0x9A    B4    0xA6    B5    0xB2
*     C2    0x83    C3    0x8F    C4    0x9B    C5    0xA7    C6    0xB3
*     C#2   0x84    C#3   0x90    C#4   0x9C    C#5   0xA8
*     D2    0x85    D3    0x91    D4    0x9D    D5    0xA9
*     D#2   0x86    D#3   0x92    D#4   0x9E    D#5   0xAA
*     E2    0x87    E3    0x93    E4    0x9F    E5    0xAB
*     F2    0x88    F3    0x94    F4    0xA0    F5    0xAC
*     F#2   0x89    F#3   0x95    F#4   0xA1    F#5   0xAD
*     G2    0x8A    G3    0x96    G4    0xA2    G5    0xAE
*     G#2   0x8B    G#3   0x97    G#4   0xA3    G#5   0xAF
*
