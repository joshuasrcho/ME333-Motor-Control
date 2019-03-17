#ifndef CURRENTCONTROL_H
#define CURRENTCONTROL_H

void icontroller_init();
void set_duty_cycle(char percentage);
void set_current_gains(float kp, float ki);
float get_kp();
float get_ki();
void current_controller(float adc, int ref);

#endif
