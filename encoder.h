#ifndef ENCODER_H
#define ENCODER_H
// Encoder library for motor control project
int encoder_counts(void); // send a command to the encoder chip
float encoder_degrees(void);
void encoder_reset(void);
void encoder_init(void); // SPI initialization for reading from the decoder chip

#endif
