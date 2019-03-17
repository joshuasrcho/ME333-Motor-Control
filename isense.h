#ifndef ISENSE_H
#define ISENSE_H
// This module owns the ADC peripheral, used to sense the motor current.

void adc_init();
float get_current();
unsigned int get_adc_count();

#endif
