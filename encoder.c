#include "encoder.h"
#include <xc.h>

static int encoder_command(int read);

static int encoder_command(int read) { // send a command to the encoder chip
                                       // 0 = reset count to 32,768, 1 = return the count
  SPI4BUF = read;                      // send the command
  while (!SPI4STATbits.SPIRBF) { ; }   // wait for the response
  SPI4BUF;                             // garbage was transferred, ignore it
  SPI4BUF = 5;                         // write garbage, but the read will have the data
  while (!SPI4STATbits.SPIRBF) { ; }
  return SPI4BUF;
}

int encoder_counts(void) {
  // read encoder count between 0-65535
  encoder_command(1);
  return encoder_command(1);
}

float encoder_degrees(void){
  float deg = 0;
  encoder_command(1);
  deg = (encoder_command(1)-32768)*(360.0/448.0)*(0.25); // Encoder has 448 lines and has 4x resolution,
                                                        // so 4 count = (360/448) degrees
  return deg;
}

void encoder_reset(void){
  // reset encoder count to 32768
  encoder_command(0);
}

void encoder_init(void) {
  // SPI initialization for reading from the decoder chip
  SPI4CON = 0;              // stop and reset SPI4
  SPI4BUF;                  // read to clear the rx receive buffer
  SPI4BRG = 0x4;            // bit rate to 8 MHz, SPI4BRG = 80000000/(2*desired)-1
  SPI4STATbits.SPIROV = 0;  // clear the overflow
  SPI4CONbits.MSTEN = 1;    // master mode
  SPI4CONbits.MSSEN = 1;    // slave select enable
  SPI4CONbits.MODE16 = 1;   // 16 bit mode
  SPI4CONbits.MODE32 = 0;
  SPI4CONbits.SMP = 1;      // sample at the end of the clock
  SPI4CONbits.ON = 1;       // turn SPI on
}
