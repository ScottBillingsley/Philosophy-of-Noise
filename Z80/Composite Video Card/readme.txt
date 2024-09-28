Here are the arduino and kicad files for a 40x25 composite video card..The card uses an
AVR128DB48 Curiosity Nano as the video generator and an IDT7130 1k dual port ram running
parallel to one segment of the ROM..This allows for 25 lines of 40 charactors in 2 color.
The charset is 256 charactors. 0 to 31 and 128 to 256 can be reprogramed without loosing 
the full display set. The charset is reprogramed one 8x8 pixel at a time by writing the 
char to reprogram to address 0x3f2 and the byte to reprogram to at address 0x3f3 to 0x3fb.
There is no feedback from the video card to the processor, so a short delay should be called
between the reprograming pixel bytes..
  There are 8 nops at the end of the main program..If the screen has to much jitter, try adding 
or subtracting a nop. I think this has to do with the way the AVR loops the main and the clock speed..
