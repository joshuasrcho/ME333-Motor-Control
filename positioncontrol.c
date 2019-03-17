#include "positioncontrol.h"

static volatile float KP, KI, KD; // global Kp, Ki, and Kd position gains

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
