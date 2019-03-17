#include "positioncontrol.h"
#include "NU32.h"

static volatile float KP, KI, KD; // global Kp, Ki, and Kd position gains

void __ISR(_TIMER_3_VECTOR, IPL4SOFT) Timer3ISR(void) {

  IFS0bits.T3IF = 0; // clear interrupt flag
}

void set_position_gains(float kp, float ki, float kd){
  // assign position gains according to user input
  KP = kp;
  KI = ki;
  KD = kd;
}

float get_position_kp(){ // send kp value for menu item 'j'
  return KP;
}

float get_position_ki(){ // send ki value for menu item 'j'
  return KI;
}

float get_position_kd(){ // send kd value for menu item 'j'
  return KD;
}

void pcontroller_init(){

  // Timer3 setup for 200Hz ISR for position control
  T3CONbits.TCKPS = 4; // Prescaler N = 16
  PR3 = 24999; // Period = (PR3+1) * N * 12.5 ns = 5 ms. f = 200Hz
  TMR3 = 0; // initial timer3 count = 0
  T3CONbits.ON = 1; // turn on Timer3
  IPC3bits.T3IP = 4; // interrupt priority is 4
  IPC3bits.T3IS = 0; // interrupt subpriority is 0
  IFS0bits.T3IF = 0; // clear interrupt flag
  IEC0bits.T3IE = 1; // enable interrupt
}
