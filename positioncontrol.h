#ifndef POSITIONCONTROL_H
#define POSITIONCONTROL_H

#define MAX_TRACK_SAMPLE  2000 // 200Hz * 10 seconds = 2000

void pcontroller_init();
void set_target_angle(float angle);
void position_controller(float target, float current);
void set_position_gains(float kp, float ki, float kd);
void set_trajectory(int n, float *tarray);
int get_numsamps();
float get_angle_array(int index);
float get_ref_angle_array(int index);
float get_position_kp();
float get_position_ki();
float get_position_kd();

#endif
