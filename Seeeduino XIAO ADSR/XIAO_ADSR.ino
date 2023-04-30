/*
                        XIAO ADSR
                 Vernon Billingsley c2023

        An ADSR envelope generator using the Seeeduino XIAO..
        Based on m0xpd's arduino version, modified for the
        XIAO and using a timer for better control..

    Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

    The above copyright notice and this permission
    notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.


                        Seeeduino
                     -----------------
         DAC A0   --| D0              |-- 5 Vin
             A1   --| D1              |-- GND
             A2   --| D2              |-- 3v3
             A3   --| D3          D10 |-- A10 MOSI
         SDA A4   --| D4           D9 |-- A9  MISO
         SCL A5   --| D5           D8 |-- A8  SCK
          TX A6   --| D6           D7 |-- A7  RX
                      ----------------

                 Pins:
                 Nano    Function
                 0       DAC Out
                 1       
                 2
                 3
                 4
                 5       Active LED
                 6       Gate, Trigger Input, 10k to GND (interrupt)
                 7       A7   Attack
                 8       A8   Decay
                 9       A9   Sustain Level
                 10      A10  Release

  http://www.lucadavidian.com/2017/08/08/arduino-m0-pro-il-sistema-di-clock/

  https://emalliab.wordpress.com/2021/04/16/comparing-timers-on-samd21-and-samd51-microcontrollers/

  https://wiki.seeedstudio.com/Seeeduino-XIAO/

  and in the datasheet: http://ww1.microchip.com/downloads/en/DeviceDoc/SAM_D21_DA1_Family_DataSheet_DS40001882F.pdf
  Timer example:
  https://gist.github.com/nonsintetic/ad13e70f164801325f5f552f84306d6f

  Based on:
  https://m0xpd.blogspot.com/2017/02/signal-processing-on-arduino.html
  https://github.com/m0xpd/ADSRduino

*/
#include <Arduino.h>

/************************* Defines ********************************/
#define DEBUG 0

#if DEBUG == 1
#define dprint(expression) Serial.print("# "); Serial.print( #expression ); Serial.print( ": " ); Serial.println( expression )
#define dshow(expression) Serial.println( expression )
#else
#define dprint(expression)
#define dshow(expression)
#endif

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

/* Set the sample rate */
#define SAMPLE_RATE 20000.0

#define LED_PIN 5
#define GATE_PIN 6

/************************** Variables *****************************/
float alpha = 0.7; // this is the pole location ('time constant') used for the first-order difference equation
double alpha1 = 0.98; // initial value for attack
double alpha2 = 0.95; // initial value for decay
double alpha3 = 0.97; // initial value for release

float envelope = 0.0;
/* Hold the ADC values */
int CV0 = 0;
int CV1 = 0;
int CV2 = 0;
int CV3 = 0;
/* The target coef of the difference equation */
/* y(k) = (1 - alpha) * x(k) + alpha * y(k-1) */
int drive = 0;
/* We use 16384, 2^14 as max drive and reduce to 10 bit */
/* to gain more samples per mode                        */
int sustain_level = 11500;

volatile boolean next_sample = false;

volatile boolean gate = false;

boolean note_active = false;
/* The ADC to read      */
uint8_t read_adc = 0;
/* Hold the current mode */
/* 0    Attack           */
/* 1    Decay            */
/* 2    Sustain          */
/* 3    Release          */
uint8_t mode = 0;
/* Debounce the gate input pin to reduce false triggers */
volatile uint16_t debounce = 0;

float cv_coef[1024] = {
0.9498, 0.9499, 0.9500, 0.9501, 0.9502, 0.9503, 0.9504, 0.9505, 0.9506, 0.9507, 0.9508, 0.9509, 0.9510, 0.9511, 0.9512, 0.9513, 0.9514, 0.9515, 0.9516, 0.9517, 0.9518, 0.9519, 0.9520, 0.9520, 0.9521, 0.9522, 0.9523, 0.9524, 0.9525, 0.9526, 0.9527, 0.9528, 0.9529, 0.9530, 0.9531, 0.9532, 0.9533, 0.9534, 0.9535, 0.9536, 0.9537, 0.9538, 0.9539, 0.9540, 0.9540, 0.9541, 0.9542, 0.9543, 0.9544, 0.9545, 0.9546, 0.9547, 0.9548, 0.9549, 0.9550, 0.9551, 0.9552, 0.9553, 0.9554, 0.9555, 0.9555, 0.9556, 0.9557, 0.9558, 
0.9559, 0.9560, 0.9561, 0.9562, 0.9563, 0.9564, 0.9565, 0.9566, 0.9566, 0.9567, 0.9568, 0.9569, 0.9570, 0.9571, 0.9572, 0.9573, 0.9574, 0.9575, 0.9576, 0.9576, 0.9577, 0.9578, 0.9579, 0.9580, 0.9581, 0.9582, 0.9583, 0.9584, 0.9585, 0.9585, 0.9586, 0.9587, 0.9588, 0.9589, 0.9590, 0.9591, 0.9592, 0.9593, 0.9593, 0.9594, 0.9595, 0.9596, 0.9597, 0.9598, 0.9599, 0.9600, 0.9601, 0.9601, 0.9602, 0.9603, 0.9604, 0.9605, 0.9606, 0.9607, 0.9608, 0.9608, 0.9609, 0.9610, 0.9611, 0.9612, 0.9613, 0.9614, 0.9614, 0.9615, 
0.9616, 0.9617, 0.9618, 0.9619, 0.9620, 0.9620, 0.9621, 0.9622, 0.9623, 0.9624, 0.9625, 0.9626, 0.9626, 0.9627, 0.9628, 0.9629, 0.9630, 0.9631, 0.9631, 0.9632, 0.9633, 0.9634, 0.9635, 0.9636, 0.9637, 0.9637, 0.9638, 0.9639, 0.9640, 0.9641, 0.9642, 0.9642, 0.9643, 0.9644, 0.9645, 0.9646, 0.9646, 0.9647, 0.9648, 0.9649, 0.9650, 0.9651, 0.9651, 0.9652, 0.9653, 0.9654, 0.9655, 0.9655, 0.9656, 0.9657, 0.9658, 0.9659, 0.9660, 0.9660, 0.9661, 0.9662, 0.9663, 0.9664, 0.9664, 0.9665, 0.9666, 0.9667, 0.9668, 0.9668, 
0.9669, 0.9670, 0.9671, 0.9672, 0.9672, 0.9673, 0.9674, 0.9675, 0.9675, 0.9676, 0.9677, 0.9678, 0.9679, 0.9679, 0.9680, 0.9681, 0.9682, 0.9683, 0.9683, 0.9684, 0.9685, 0.9686, 0.9686, 0.9687, 0.9688, 0.9689, 0.9690, 0.9690, 0.9691, 0.9692, 0.9693, 0.9693, 0.9694, 0.9695, 0.9696, 0.9696, 0.9697, 0.9698, 0.9699, 0.9699, 0.9700, 0.9701, 0.9702, 0.9703, 0.9703, 0.9704, 0.9705, 0.9706, 0.9706, 0.9707, 0.9708, 0.9709, 0.9709, 0.9710, 0.9711, 0.9711, 0.9712, 0.9713, 0.9714, 0.9714, 0.9715, 0.9716, 0.9717, 0.9717, 
0.9718, 0.9719, 0.9720, 0.9720, 0.9721, 0.9722, 0.9723, 0.9723, 0.9724, 0.9725, 0.9725, 0.9726, 0.9727, 0.9728, 0.9728, 0.9729, 0.9730, 0.9730, 0.9731, 0.9732, 0.9733, 0.9733, 0.9734, 0.9735, 0.9735, 0.9736, 0.9737, 0.9738, 0.9738, 0.9739, 0.9740, 0.9740, 0.9741, 0.9742, 0.9743, 0.9743, 0.9744, 0.9745, 0.9745, 0.9746, 0.9747, 0.9747, 0.9748, 0.9749, 0.9749, 0.9750, 0.9751, 0.9752, 0.9752, 0.9753, 0.9754, 0.9754, 0.9755, 0.9756, 0.9756, 0.9757, 0.9758, 0.9758, 0.9759, 0.9760, 0.9760, 0.9761, 0.9762, 0.9762, 
0.9763, 0.9764, 0.9764, 0.9765, 0.9766, 0.9766, 0.9767, 0.9768, 0.9768, 0.9769, 0.9770, 0.9770, 0.9771, 0.9772, 0.9772, 0.9773, 0.9774, 0.9774, 0.9775, 0.9776, 0.9776, 0.9777, 0.9778, 0.9778, 0.9779, 0.9780, 0.9780, 0.9781, 0.9782, 0.9782, 0.9783, 0.9783, 0.9784, 0.9785, 0.9785, 0.9786, 0.9787, 0.9787, 0.9788, 0.9789, 0.9789, 0.9790, 0.9790, 0.9791, 0.9792, 0.9792, 0.9793, 0.9794, 0.9794, 0.9795, 0.9795, 0.9796, 0.9797, 0.9797, 0.9798, 0.9799, 0.9799, 0.9800, 0.9800, 0.9801, 0.9802, 0.9802, 0.9803, 0.9804, 
0.9804, 0.9805, 0.9805, 0.9806, 0.9807, 0.9807, 0.9808, 0.9808, 0.9809, 0.9810, 0.9810, 0.9811, 0.9811, 0.9812, 0.9813, 0.9813, 0.9814, 0.9814, 0.9815, 0.9816, 0.9816, 0.9817, 0.9817, 0.9818, 0.9818, 0.9819, 0.9820, 0.9820, 0.9821, 0.9821, 0.9822, 0.9823, 0.9823, 0.9824, 0.9824, 0.9825, 0.9825, 0.9826, 0.9827, 0.9827, 0.9828, 0.9828, 0.9829, 0.9829, 0.9830, 0.9831, 0.9831, 0.9832, 0.9832, 0.9833, 0.9833, 0.9834, 0.9835, 0.9835, 0.9836, 0.9836, 0.9837, 0.9837, 0.9838, 0.9838, 0.9839, 0.9840, 0.9840, 0.9841, 
0.9841, 0.9842, 0.9842, 0.9843, 0.9843, 0.9844, 0.9844, 0.9845, 0.9846, 0.9846, 0.9847, 0.9847, 0.9848, 0.9848, 0.9849, 0.9849, 0.9850, 0.9850, 0.9851, 0.9851, 0.9852, 0.9852, 0.9853, 0.9854, 0.9854, 0.9855, 0.9855, 0.9856, 0.9856, 0.9857, 0.9857, 0.9858, 0.9858, 0.9859, 0.9859, 0.9860, 0.9860, 0.9861, 0.9861, 0.9862, 0.9862, 0.9863, 0.9863, 0.9864, 0.9864, 0.9865, 0.9865, 0.9866, 0.9866, 0.9867, 0.9867, 0.9868, 0.9868, 0.9869, 0.9869, 0.9870, 0.9870, 0.9871, 0.9871, 0.9872, 0.9872, 0.9873, 0.9873, 0.9874, 
0.9874, 0.9875, 0.9875, 0.9876, 0.9876, 0.9877, 0.9877, 0.9878, 0.9878, 0.9879, 0.9879, 0.9880, 0.9880, 0.9881, 0.9881, 0.9882, 0.9882, 0.9882, 0.9883, 0.9883, 0.9884, 0.9884, 0.9885, 0.9885, 0.9886, 0.9886, 0.9887, 0.9887, 0.9888, 0.9888, 0.9888, 0.9889, 0.9889, 0.9890, 0.9890, 0.9891, 0.9891, 0.9892, 0.9892, 0.9893, 0.9893, 0.9893, 0.9894, 0.9894, 0.9895, 0.9895, 0.9896, 0.9896, 0.9897, 0.9897, 0.9897, 0.9898, 0.9898, 0.9899, 0.9899, 0.9900, 0.9900, 0.9901, 0.9901, 0.9901, 0.9902, 0.9902, 0.9903, 0.9903, 
0.9904, 0.9904, 0.9904, 0.9905, 0.9905, 0.9906, 0.9906, 0.9906, 0.9907, 0.9907, 0.9908, 0.9908, 0.9909, 0.9909, 0.9909, 0.9910, 0.9910, 0.9911, 0.9911, 0.9911, 0.9912, 0.9912, 0.9913, 0.9913, 0.9913, 0.9914, 0.9914, 0.9915, 0.9915, 0.9915, 0.9916, 0.9916, 0.9917, 0.9917, 0.9917, 0.9918, 0.9918, 0.9919, 0.9919, 0.9919, 0.9920, 0.9920, 0.9921, 0.9921, 0.9921, 0.9922, 0.9922, 0.9922, 0.9923, 0.9923, 0.9924, 0.9924, 0.9924, 0.9925, 0.9925, 0.9925, 0.9926, 0.9926, 0.9927, 0.9927, 0.9927, 0.9928, 0.9928, 0.9928, 
0.9929, 0.9929, 0.9930, 0.9930, 0.9930, 0.9931, 0.9931, 0.9931, 0.9932, 0.9932, 0.9932, 0.9933, 0.9933, 0.9933, 0.9934, 0.9934, 0.9935, 0.9935, 0.9935, 0.9936, 0.9936, 0.9936, 0.9937, 0.9937, 0.9937, 0.9938, 0.9938, 0.9938, 0.9939, 0.9939, 0.9939, 0.9940, 0.9940, 0.9940, 0.9941, 0.9941, 0.9941, 0.9942, 0.9942, 0.9942, 0.9943, 0.9943, 0.9943, 0.9944, 0.9944, 0.9944, 0.9945, 0.9945, 0.9945, 0.9946, 0.9946, 0.9946, 0.9946, 0.9947, 0.9947, 0.9947, 0.9948, 0.9948, 0.9948, 0.9949, 0.9949, 0.9949, 0.9950, 0.9950, 
0.9950, 0.9951, 0.9951, 0.9951, 0.9951, 0.9952, 0.9952, 0.9952, 0.9953, 0.9953, 0.9953, 0.9953, 0.9954, 0.9954, 0.9954, 0.9955, 0.9955, 0.9955, 0.9956, 0.9956, 0.9956, 0.9956, 0.9957, 0.9957, 0.9957, 0.9958, 0.9958, 0.9958, 0.9958, 0.9959, 0.9959, 0.9959, 0.9959, 0.9960, 0.9960, 0.9960, 0.9961, 0.9961, 0.9961, 0.9961, 0.9962, 0.9962, 0.9962, 0.9962, 0.9963, 0.9963, 0.9963, 0.9963, 0.9964, 0.9964, 0.9964, 0.9964, 0.9965, 0.9965, 0.9965, 0.9965, 0.9966, 0.9966, 0.9966, 0.9966, 0.9967, 0.9967, 0.9967, 0.9967, 
0.9968, 0.9968, 0.9968, 0.9968, 0.9969, 0.9969, 0.9969, 0.9969, 0.9970, 0.9970, 0.9970, 0.9970, 0.9971, 0.9971, 0.9971, 0.9971, 0.9971, 0.9972, 0.9972, 0.9972, 0.9972, 0.9973, 0.9973, 0.9973, 0.9973, 0.9974, 0.9974, 0.9974, 0.9974, 0.9974, 0.9975, 0.9975, 0.9975, 0.9975, 0.9975, 0.9976, 0.9976, 0.9976, 0.9976, 0.9976, 0.9977, 0.9977, 0.9977, 0.9977, 0.9978, 0.9978, 0.9978, 0.9978, 0.9978, 0.9978, 0.9979, 0.9979, 0.9979, 0.9979, 0.9979, 0.9980, 0.9980, 0.9980, 0.9980, 0.9980, 0.9981, 0.9981, 0.9981, 0.9981, 
0.9981, 0.9982, 0.9982, 0.9982, 0.9982, 0.9982, 0.9982, 0.9983, 0.9983, 0.9983, 0.9983, 0.9983, 0.9983, 0.9984, 0.9984, 0.9984, 0.9984, 0.9984, 0.9984, 0.9985, 0.9985, 0.9985, 0.9985, 0.9985, 0.9985, 0.9986, 0.9986, 0.9986, 0.9986, 0.9986, 0.9986, 0.9987, 0.9987, 0.9987, 0.9987, 0.9987, 0.9987, 0.9987, 0.9988, 0.9988, 0.9988, 0.9988, 0.9988, 0.9988, 0.9988, 0.9989, 0.9989, 0.9989, 0.9989, 0.9989, 0.9989, 0.9989, 0.9990, 0.9990, 0.9990, 0.9990, 0.9990, 0.9990, 0.9990, 0.9990, 0.9991, 0.9991, 0.9991, 0.9991, 
0.9991, 0.9991, 0.9991, 0.9991, 0.9992, 0.9992, 0.9992, 0.9992, 0.9992, 0.9992, 0.9992, 0.9992, 0.9992, 0.9993, 0.9993, 0.9993, 0.9993, 0.9993, 0.9993, 0.9993, 0.9993, 0.9993, 0.9994, 0.9994, 0.9994, 0.9994, 0.9994, 0.9994, 0.9994, 0.9994, 0.9994, 0.9994, 0.9995, 0.9995, 0.9995, 0.9995, 0.9995, 0.9995, 0.9995, 0.9995, 0.9995, 0.9995, 0.9995, 0.9995, 0.9996, 0.9996, 0.9996, 0.9996, 0.9996, 0.9996, 0.9996, 0.9996, 0.9996, 0.9996, 0.9996, 0.9996, 0.9996, 0.9996, 0.9997, 0.9997, 0.9997, 0.9997, 0.9997, 0.9997, 
0.9997, 0.9997, 0.9997, 0.9997, 0.9997, 0.9997, 0.9997, 0.9997, 0.9997, 0.9997, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9998, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 
};

/**************************  Functions ****************************/
void scan_adc() {
  switch (read_adc) {
    case 0:
      CV0 = (CV0 + analogRead(7)) / 2;
      alpha1 = cv_coef[CV0];
      break;
    case 1:
      CV1 = (CV1 + analogRead(8)) / 2;
      alpha2 = cv_coef[CV1];
      break;
    case 2:
      CV2 = (CV2 + analogRead(9)) / 2;
      sustain_level = CV2 * 15;
      break;
    case 3:
      CV3 = (CV3 + analogRead(10)) / 2;
      alpha3 = cv_coef[CV3];
      break;
  }

  read_adc ++;
  if (read_adc > 3) {
    /* Reset the count */
    read_adc = 0;
  }
}/* End Scan adc */

/* The timer ISR */
void TC5_Handler (void) {
  /* Convert to 10 bit and send the sample to the DAC */
  analogWrite(A0, ((int)envelope >> 4));
  /* Set the boolean for the next sample */
  next_sample = true;
  /* Debounce the gate pin */
  debounce ++;
  /* Rest the timer */
  TC5->COUNT16.INTFLAG.bit.MC0 = 1;
}

void tcConfigure(int sampleRate)
{
  // select the generic clock generator used as source to the generic clock multiplexer
  GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(GCM_TC4_TC5)) ;
  while (GCLK->STATUS.bit.SYNCBUSY);
  tcReset(); //reset TC5
  // Set Timer counter 5 Mode to 16 bits, it will become a 16bit counter ('mode1' in the datasheet)
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
  // Set TC5 waveform generation mode to 'match frequency'
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
  //set prescaler
  //the clock normally counts at the GCLK_TC frequency, but we can set it to divide that frequency to slow it down
  //you can use different prescaler divisons here like TC_CTRLA_PRESCALER_DIV1 to get a different range
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1 | TC_CTRLA_ENABLE; //it will divide GCLK_TC frequency by 1024
  //set the compare-capture register.
  //The counter will count up to this value (it's a 16bit counter so we use uint16_t)
  //this is how we fine-tune the frequency, make it count to a lower or higher value
  //system clock should be 1MHz (8MHz/8) at Reset by default
  TC5->COUNT16.CC[0].reg = (uint16_t) (SystemCoreClock / sampleRate);
  while (tcIsSyncing());
  // Configure interrupt request
  NVIC_DisableIRQ(TC5_IRQn);
  NVIC_ClearPendingIRQ(TC5_IRQn);
  NVIC_SetPriority(TC5_IRQn, 0);
  NVIC_EnableIRQ(TC5_IRQn);
  // Enable the TC5 interrupt request
  TC5->COUNT16.INTENSET.bit.MC0 = 1;
  while (tcIsSyncing()); //wait until TC5 is done syncing
}
//Function that is used to check if TC5 is done syncing
//returns true when it is done syncing
bool tcIsSyncing()
{
  return TC5->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;
}
//This function enables TC5 and waits for it to be ready
void tcStartCounter()
{
  TC5->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE; //set the CTRLA register
  while (tcIsSyncing()); //wait until snyc'd
}
//Reset TC5
void tcReset()
{
  TC5->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
  while (tcIsSyncing());
  while (TC5->COUNT16.CTRLA.bit.SWRST);
}
//disable TC5
void tcDisable()
{
  TC5->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (tcIsSyncing());
}

void triggered() {
  if (debounce >= 100) {
    gate = digitalRead(GATE_PIN);
    debounce = 0;
  }
}

/******************************************************************/
/*************************** Setup ********************************/
/******************************************************************/
void setup() {
  if (DEBUG) {
    Serial.begin(115200);
  }

  /* Configure the timer */
  tcConfigure(SAMPLE_RATE);
  /* Start the timer */
  tcStartCounter();
  /* Set the analog input and output resolution */
  analogWriteResolution(10);
  analogReadResolution(10);

  /************************* Setup Pins ***************************/
  pinMode(LED_PIN, OUTPUT);
  pinMode(GATE_PIN, INPUT);
  /* Set the ISR for the gate pin */
  attachInterrupt(digitalPinToInterrupt(GATE_PIN), triggered, CHANGE);

}/**************************  End Setup **************************/



/******************************************************************/
/**************************** Loop ********************************/
/******************************************************************/
void loop() {

  /* If gate is triggered and no note is active, */
  /* Start a new note */
  if (gate == true && note_active == false) {
    /* Set the note active */
    note_active = true;
    /* Set the mode to attack */
    mode = 0;
    /* Set the alpha to the attack alpha */
    alpha = alpha1;
    /* Set the drive to full level */
    drive = 16384;
    /* Turn on the active LED */
    digitalWrite(LED_PIN, HIGH);
  }

  if (next_sample == true && note_active == true) {
    switch (mode) {
      case 0:   /* Attack */
        if (envelope > 16300) { /* End of Attack */
          /* Set the mode to the decay mode */
          mode = 1;
          /* Set the alpha to the decay alpha */
          alpha = alpha2;
          /* Set the drive a little lower than the sustain level */
          drive = sustain_level - 10;
        }
        break;
      case 1:   /* Decay */
        if (envelope <= sustain_level) { /* End decay */
          /* Set the mode to sustain mode */
          mode = 2;
        }
        break;
      case 2:   /* Sustain */
        /* If the gate pin has gone low, release the note */
        if (gate == false) {
          /* Set the mode to the release mode */
          mode = 3;
          /* Set the alpha to the release alpha */
          alpha = alpha3;
          /* Set the drive a liitle low than zero */
          drive = - 10;
        }
        break;
      case 3:   /* Release */
        if (envelope <= 0) {
          /* Clear the note active */
          note_active = false;
          /* Turn off the active LED */
          digitalWrite(LED_PIN, LOW);
        }
        /* Retrigger */
        /* If the gate pin goes HIGH while in the release   */
        /* mode, restart the note from the current envelope */
        if (gate == true) {
          note_active = false;
        }
        break;
    }/* End switch */

    envelope = ((1.0 - alpha) * drive + alpha * envelope);

    /* Clear the boolean */
    next_sample = false;
  }

  if (note_active == false) {
    /* Scan the adc */
    scan_adc();
  }


}/*************************** End Loop *****************************/
