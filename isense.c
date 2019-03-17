#include "isense.h"
#include <xc.h>

unsigned int get_adc_count() { // sample & convert the value on AN0 pin

  unsigned int total = 0, average_reading = 0;
  int i;
  for(i=0; i<=4; i++){ // read adc five times
    AD1CON1bits.SAMP = 1; // start sampling
    while (!AD1CON1bits.DONE){ // Do nothing while samping and converting
      ;
    }
    total += ADC1BUF0; // sum the readings
  }
  average_reading = total/5; // take the average of the five readings

  return average_reading;
}

float get_current() {
  unsigned int adc_count;
  float current;
  // linear least square fit shows that when count=0, current=-416.38mA
  // and when count=1023, current=468.04mA.
  // current (mA) = slope * (measured_count - 1023) + 468.04
  adc_count = get_adc_count();
  current = ((468.04 + 416.38)/1023.0)*((float)adc_count - 1023.0) + 468.04;
  return current;
}


void adc_init(){
  AD1PCFGbits.PCFG0 = 0;  // AN0 is an adc pin
  AD1CHSbits.CH0SA = 0;   // connect AN0 pin to MUXA for sampling
  AD1CON1bits.SSRC = 0b111; // Enable auto conversion
  AD1CON3bits.ADCS = 2;  // ADC clock period is Tad = 2*(ADCS+1)*Tpb =
                         //                           2*3*12.5ns = 75ns
  AD1CON1bits.ON = 1;   // turn on

}
