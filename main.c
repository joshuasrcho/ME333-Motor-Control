#include "NU32.h"          // config bits, constants, funcs for startup and UART
#include "encoder.h"       // allows  you to use encoder module
#include "utilities.h"     // allows you to get and set PIC32 mode
#include "isense.h"        // allows you to use current sensor module
#include "currentcontrol.h" // allows you to use current controller module
#include "positioncontrol.h" // allows you to use position controller module
#include <stdio.h>

// include other header files here

#define BUF_SIZE 200

int main()
{
  char buffer[BUF_SIZE];
  NU32_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
  NU32_LED1 = 1;  // turn off the LEDs
  NU32_LED2 = 1;
  __builtin_disable_interrupts();
  set_mode(IDLE); // default mode is IDLE
  encoder_init(); // initialize encoder
  adc_init(); // initialize adc peripheral for current sensor
  icontroller_init(); // initialize current controller module
  __builtin_enable_interrupts();

  while(1)
  {
    NU32_ReadUART3(buffer,BUF_SIZE); // we expect the next character to be a menu command
    NU32_LED2 = 1;                   // clear the error LED
    switch (buffer[0]) {
      case 'a': // read adc count (0-1023) from the current sensor
      {
        sprintf(buffer,"%d\r\n",get_adc_count());
        NU32_WriteUART3(buffer);
        break;
      }

      case 'b': // read calibrated and measured current from the current sensor in mA
      {
        sprintf(buffer,"%.2f\r\n",get_current());
        NU32_WriteUART3(buffer);
        break;
      }

      case 'c': // send encoder count between 0 and 65535
      {
        sprintf(buffer,"%d\r\n",encoder_counts());
        NU32_WriteUART3(buffer);
        break;
      }

      case 'd': // send encoder reading in degrees
      {
        sprintf(buffer,"%f\r\n",encoder_degrees());
        NU32_WriteUART3(buffer);
        break;
      }

      case 'e': // resets encoder count/angle
      {
        encoder_reset();
      }

      case 'f': // set PWM (-100 t0 100)
      {
        int duty_cycle = 0;

        NU32_ReadUART3(buffer,BUF_SIZE); // get user input
        sscanf(buffer,"%d",&duty_cycle);
        set_duty_cycle(duty_cycle); // set PWM duty cycle according to user input
        set_mode(PWM); // switch to PWM mode
        break;
      }

      case 'g': // set current gains
      {
        float kp,ki;
        NU32_ReadUART3(buffer,BUF_SIZE);
        sscanf(buffer,"%f",&kp);
        NU32_ReadUART3(buffer,BUF_SIZE);
        sscanf(buffer,"%f",&ki);
        set_current_gains(kp,ki);
        break;
      }

      case 'h': // get current gains
      {
        sprintf(buffer,"%f %f\r\n",get_current_kp(),get_current_ki());
        NU32_WriteUART3(buffer);
        break;
      }

      case 'i': // set poisition gains
      {
        float kp,ki,kd;
        NU32_ReadUART3(buffer,BUF_SIZE);
        sscanf(buffer,"%f",&kp);
        NU32_ReadUART3(buffer,BUF_SIZE);
        sscanf(buffer,"%f",&ki);
        NU32_ReadUART3(buffer,BUF_SIZE);
        sscanf(buffer,"%f",&kd);
        set_position_gains(kp,ki,kd);
        break;
      }

      case 'j': // get position gains
      {
        sprintf(buffer,"%f %f %f\r\n",get_position_kp(),get_position_ki(),get_position_kd());
        NU32_WriteUART3(buffer);
        break;
      }

      case 'k': // Test current gains. Set mode to ITEST
      {
        set_mode(ITEST);
        while (get_mode() == ITEST){ // wait while testing...
          ;
        }
        sprintf(buffer,"%d\r\n",PLOTPTS);
        NU32_WriteUART3(buffer); // send the number of samples PIC will be sending (100)
        int i;
        for (i=0;i<PLOTPTS;i++){ // for each loop PIC sends out a pair of samples

          sprintf(buffer,"%d %f\r\n",get_ref_current_array(i),get_actual_current_array(i));
          NU32_WriteUART3(buffer);
        }
        break;
      }

      case 'p': // unpower the motor
      {
        set_mode(IDLE); // switch to IDLE mode
        break;
      }

      case 'q':
      {
        // handle q for quit. Later you may want to return to IDLE mode here.
        set_mode(IDLE);
        break;
      }
      case 'r': // Get mode
      {
        sprintf(buffer,"%d\r\n",get_mode());
        NU32_WriteUART3(buffer);
        break;
      }
      default:
      {
        NU32_LED2 = 0;  // turn on LED2 to indicate an error
        break;
      }
    }
  }
  return 0;
}
