#ifndef POSITIONCONTROL_H
#define POSITIONCONTROL_H

void set_position_gains(float kp, float ki, float kd);
float get_position_kp();
float get_position_ki();
float get_position_kd();

#endif
