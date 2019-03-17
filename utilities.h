#ifndef UTILITIES_H
#define UTILITIES_H

enum mode {IDLE,PWM,ITEST,HOLD,TRACK};


int get_mode();
void set_mode(enum mode new_state);

#endif
