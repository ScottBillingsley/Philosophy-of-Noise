![AVRduino Eddie Model](https://github.com/user-attachments/assets/47a200f1-de4c-495e-a86f-016f0d26c948)

  The AVRduino EDDIE is a business size microcontroller based around the
  AVR128DB48 chip. Features inckude Serial port with selectable jumper on
  serial0. Jumper for selecting MVIO power, 3.3 or 5 volts. Jumper to power from 
  thẹ USB port. Footprint for a half size external clock for speed upto 40 mHz.
  Footprint for a 24c256 or 24c512 EEPROM on WIRE0. Onboard LED on PA1. And 
  power plugs for GND, 3.3 and 5 volt..

  UPDI
  
  ![USB to TTL](https://github.com/user-attachments/assets/3871edfd-d2cb-4eae-8ad6-9837105febc7)

  The UPDI header is wired for a USB to TTL breakout board..Olny GND, TX and RX pins are used..I
  have found the black boards by Hiletgo work very well but you will need a USB A Male to USB A 
  female cable..

  DXCore

  The board uses Spence Konde's DxCore 1.5.10 and a clean install of arduino IDE version 1.8.19.
  The only trouble I have had is not setting the clock speed on upload. 
  In the templet file you will find the portected writes for setting 
  32 mHz internal or 40 mHz external clock based on the build option at upload.
  Other clock frequencies can be calculated by using the data sheet section 12.4.
  DxCore can be installed in the arduino IDE using the boards manager.
  Lots of good infomation on the AVR chip and the features of the core can 
  be found on the github repo..
  [DxCore](https://github.com/SpenceKonde/DxCore)
  
  
