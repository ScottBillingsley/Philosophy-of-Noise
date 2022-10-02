                  Decode a rotary encoder and send the data over
                  I2C as a master sender...
                  Sends 0b001 , 1 as Up
                        0b100 , 4 as Down
                        0b010 , 3 as Button pressed
 
           Encoder        
             ---                      
            |   |---- +5               Tiny85               Uno/Nano
          --|   |---- GND              ---U---            
         |  |   |---- CLK ------------| 2   5 |- SDA  ----|A4
          --|   |---- Data -----------| 3   7 |- SCL  ----|A5
            |   |---- Button ---------| 6   1 |-
             ---                 GND -| 4   8 |- +5
                                       -------  
  
                        
