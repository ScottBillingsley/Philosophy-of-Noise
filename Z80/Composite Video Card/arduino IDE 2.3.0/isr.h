#pragma once

#include "Arduino.h"

/* The pixel clock ISR 
*  PORT A is the output port with bit 7 as 
*  output pin.  
*/
ISR(TCA0_CMP0_vect) {
  /* Hold the current line count */
  static uint16_t line_count;
  /* Keep up with v_ram display line */
  static uint8_t v_ram_line;
  /* Pointer to temp vram, Z register requires a pointer */
  volatile uint8_t* ptr;
  /* Set if in the display time */
  static bool pixel_display = false;

  /* Display the pixels */
  if (pixel_display == true) {

    /* Pointer to the tmp vram buffer */
    ptr = &(v_ram[v_ram_line][0]);

    /* Back porch and screen centering */
    for (uint8_t s = 0; s < 70; s++) {
      NOP;
    }

    /* Pixel Clock */
    asm volatile(
      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */

      "ld  r16, z+    \n\t" /* 2 clks. load byte into r16 */
      "out  %0, r16   \n\t" /* 1 clk   output first pixel */
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t" /* 1 clk shift left 1 */
      "out  %0, r16   \n\t" /* 1 clk  output second pixel */
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t"
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */
      "nop            \n\t" /* 1 clk */
      "rol  r16       \n\t"
      "out  %0, r16   \n\t"
      "nop            \n\t" /* 1 clk */      

      /* Back porch   */
      "nop    \n\t"
      "nop    \n\t"
      "cbr r17, 0xFF        \n\t" /*  1   Clear bit zero */
      "out  %0, r17         \n\t" /*  1   0 bit */
      "nop    \n\t"
      "nop    \n\t"

      ::"i"(_SFR_IO_ADDR(VPORTA_OUT)),
      "z"(ptr)
      : "r16", "r17"); /* End pixel clock */

    /* Increment the line count for the next v_ram line */
    v_ram_line ++;
    NOP;

  } /* End pixel display */

  /* Reset the interrupt flag */
  TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm;

  /* Increment the line count */
  line_count++;

  /* Skip the first 2 rows for screen sync */
  if (line_count == 20) {
    pixel_display = true;
  }

  /* Display 25 lines to the screen */
  if (line_count == 220) {
    pixel_display = false;
  }

  /* Start the blanking period to read the video ram */
  if (line_count == 222) {
    /* Blanking period starts */
    blanking = true;
  }

  /* V_Sync Start */
  if (line_count == 256) {
    blanking = false;
    /* Invert the pin */
    PORTB.PIN0CTRL &= 0x7f;
  }

  /* Reset counts and end V_Sync */
  if (line_count == 262) {
    /* Reset the line count */
    line_count = 0;
    /* Reset the v_ram line count */
    v_ram_line = 0;

    /* V_Sync End     */
    /* Invert the pin */
    PORTB.PIN0CTRL |= (1 << 7);
  }

} /* End ISR */