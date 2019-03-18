#ifndef CURRENTCONTROL_H
#define CURRENTCONTROL_H

#define PLOTPTS 100 // We use 100 current samples to plot ITEST

void icontroller_init();
void set_duty_cycle(char percentage);
void set_current_gains(float kp, float ki);
float get_current_kp();
float get_current_ki();
void set_target_current(float target);
void send_pwm_signal();
void current_controller(float current, int ref);
int get_ref_current_array();
float get_actual_current_array();

#endif
