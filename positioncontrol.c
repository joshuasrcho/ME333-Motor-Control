#include "positioncontrol.h"
#include "utilities.h"
#include "encoder.h"
#include "NU32.h"

static volatile float KP, KI, KD; // global Kp, Ki, and Kd position gains
static volatile float TARGET_ANGLE;
static volatile float Eint = 0;
static volatile float Eprev = 0;

void __ISR(_TIMER_3_VECTOR, IPL4SOFT) Timer3ISR(void) {

  LATEbits.LATE0 = !(LATEbits.LATE0); // toggle RE0 digital output every time ISR is called

  switch (get_mode()) {
    case 3: // check if in HOLD mode
    {
      static float current_angle;
      current_angle = encoder_degrees(); // measure current angle
      position_controller(TARGET_ANGLE, current_angle); // put target angle and current angle into PID controller
      break;
    }
    default:
    {
      break;
    }
  }

  IFS0bits.T3IF = 0; // clear interrupt flag
}

void set_position_gains(float kp, float ki, float kd){
  // assign position gains according to user input
  KP = kp;
  KI = ki;
  KD = kd;
  Eint = 0;
  Eprev = 0;
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

void set_target_angle(float angle){
  TARGET_ANGLE = angle; // ANGLE is in degrees
}

void position_controller(float target, float current){
  float e = 0;
  float edot = 0;
  float u = 0;
  e = target - current; // find error between target angle and encoder reading
  edot = e - Eprev; // Derivative term
  Eint = Eint + e; // Integral term
  u = KP*e + KI*Eint + KD*edot; // PI controller equation
  set_target_current(u);
  Eprev = e; // current error is now prev error for next iteration
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

  // digital output pin setup to verify 200Hz ISR frequency
  TRISEbits.TRISE0 = 0; // RE0 is a digital output pin
  LATEbits.LATE0 = 0; // RE0 is initially low
}
