/*
 *                    Convert .RAW
 *              Vernon Billingsley c2024
 *                    Processing 4
 *
 *        Convert a signed 16 bit .RAW headerless audio file, 
 *        I use Audacity, into either a 10 of 12 bit unsigned
 *        HEX comma-separated values file for use in arduino
 *        programming..
 */
/* Change to convert to either 10 or 12 bit */
int convert = 12;
/* Full path of binary .RAW file to read   */
String file_to_scan = "C:/Users/cash america/Desktop/Output.raw";
/* Create a file reader */
File file_name = new File(file_to_scan);
/* Read in the bytes */
byte b[] = loadBytes(file_name);
/* Create an output writer */
PrintWriter output;
/* Hold the current sample */
int sample = 0;

void setup() {
  size(100, 100);
  /* File name of the output file in the current dir */
  output = createWriter("output_hex.txt");
  
  output.println("// Converted form :");
  output.print("// ");
  output.println(file_to_scan);
  output.println("// read value in arduino using :");
  output.println("// uint16_t x = pgm_read_word_near(&(array_name[index])); ");
  output.println();
  
  int s = b.length / 2;
  output.print("const uint16_t ");
  if(convert == 12){
   output.print("convert_12[");
  }else{
    output.print("convert_10[");
  }
   output.print(s);
   output.print("] PROGMEM = {");
   output.println();
  
  /* Scan the file */
  for ( int i = 0; i < b.length; i = i + 2 ) {

    if (i > 0 && (i % 32) == 0) {
      output.println();
    }
    /* Read the Low and High byte */
    int low_byte = b[i] & 0x00ff;
    int high_byte = b[i + 1]; // & 0x00ff;
    /* Combine the bytes to form a 16 bit signed word */
    int signed_word = (high_byte << 8) + low_byte;
    /* Add to 2^15 to remove the sign */
    int unsigned_word = 32768 + signed_word;
    /* Reduce the bit depth to 12 or 10 bit */
    if (convert == 12 ) {
      sample = unsigned_word >> 4;
    } else {
      sample = unsigned_word >> 6;
    }

    /* Convert to HEX */
    output.print("0x");
    output.print(convert_to_hex((sample >> 12) & 0xf));
    output.print(convert_to_hex((sample >> 8) & 0xf));
    output.print(convert_to_hex((sample >> 4) & 0xf));
    output.print(convert_to_hex(sample & 0xf));
    output.print(", ");

  }

  output.println();
  /* End the array list */
  output.println("};");
  /* Close the file and exit */
  output.flush();
  output.close();
  exit();
}

void draw() {
}

char convert_to_hex(int x) {
  char this_char = ' ';
  switch (x) {
  case 0:
    this_char = '0';
    break;
  case 1:
    this_char = '1';
    break;
  case 2:
    this_char = '2';
    break;
  case 3:
    this_char = '3';
    break;
  case 4:
    this_char = '4';
    break;
  case 5:
    this_char = '5';
    break;
  case 6:
    this_char = '6';
    break;
  case 7:
    this_char = '7';
    break;
  case 8:
    this_char = '8';
    break;
  case 9:
    this_char = '9';
    break;
  case 10:
    this_char = 'A';
    break;
  case 11:
    this_char = 'B';
    break;
  case 12:
    this_char = 'C';
    break;
  case 13:
    this_char = 'D';
    break;
  case 14:
    this_char = 'E';
    break;
  case 15:
    this_char = 'F';
    break;
  }
  return this_char;
}
