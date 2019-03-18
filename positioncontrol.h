#ifndef POSITIONCONTROL_H
#define POSITIONCONTROL_H

void pcontroller_init();
void set_target_angle(float angle);
void position_controller(float target, float current);
void set_position_gains(float kp, float ki, float kd);
float get_position_kp();
float get_position_ki();
float get_position_kd();

#endif
