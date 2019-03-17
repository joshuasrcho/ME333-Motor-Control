#include "currentcontrol.h"
#include "utilities.h"
#include "isense.h"
#include "NU32.h"

static volatile int duty_cycle;
static volatile float KP, KI;
static volatile int ref_current_array[PLOTPTS];
static volatile float actual_current_array[PLOTPTS];
static volatile float Eint = 0; // Integral term of PI controller

void __ISR(_TIMER_4_VECTOR, IPL5SOFT) Timer4ISR(void) {
  // 5kHz ISR
  switch(get_mode()){

    case 0: // IDLE mode
    {
      OC1RS = 0; // H-bridge in brake mode
      break;
    }

    case 1: // PWM module
    {
      if (duty_cycle < 0){ // if duty cycle is negative
        OC1RS = duty_cycle * -1; // make sure OC1RS is positive
        LATBbits.LATB1 = 0;  // go reverse direction
      }
      else{
        OC1RS = duty_cycle;
        LATBbits.LATB1 = 1;
      }
      break;
    }

    case 2: // ITEST mode
    {
      static int plotind = 0; // variable that counts from 0 to 99
      static int ref_current = 200; // initial reference current of 200 mA
      static float actual_current;

      if ((plotind % 25) == 0){
        // change refrence current sign every 25 counts
        ref_current = -1*ref_current;
      }

      actual_current = get_current(); // obtain actual current

      // put reference current and actual current into PI controller
      current_controller(actual_current, ref_current);
      OC1RS = duty_cycle;

      // store reference and actual current into array so we can plot them later
      ref_current_array[plotind] = ref_current;
      actual_current_array[plotind] = actual_current;

      if (plotind == PLOTPTS){
        // go back to IDLE mode if plotind reaches 99
        set_mode(IDLE);
        break;
      }
      plotind ++;
    }
  }
  IFS0bits.T4IF = 0; // clear interrupt flag
}

void set_duty_cycle(char percentage){ // set PWM duty cycle to the value -100 to 100 specified by the user
                                      // through the client menu item "f"
  if (percentage > 100){ // saturate values outside the range [-100, 100]
    percentage = 100;
  }
  else if (percentage < -100){
    percentage = -100;
  }
  duty_cycle = percentage/100.0 * PR2; // convert duty cycle percentage to OC1RS value
}

void set_current_gains(float kp, float ki){ // set kp and ki based on user input from menu item 'g'
  KP = kp;
  KI = ki;
  Eint = 0;
}

float get_kp(){ // send kp value for menu item 'h'
  return KP;
}

float get_ki(){ // send kp value for menu item 'h'
  return KI;
}

int get_ref_current_array(int index){ // send reference current array for plotting
  return ref_current_array[index];
}

float get_actual_current_array(int index){ // send actual current array for plotting
  return actual_current_array[index];
}

void current_controller(float current, int ref){
  float e = 0;
  float u = 0;
  e = (float)ref-current; // find error between reference and ADC reading
  Eint = Eint + e; // Integral term
  u = KP*e + KI*Eint; // PI controller equation

  set_duty_cycle((int) u); // convert control effort unew to value between 0 and PR3
}


void icontroller_init(){

  // Timer setup for 20kHz PWM signal
  PR2 = 3999; // Period = (PR2+1) * N * 12.5 ns = 50 us. f = 20kHz
  TMR2 = 0; // inital timer2 count = 0
  OC1CONbits.OCM = 0b110; // PWM mode without fault pin
  OC1R = 1000; // 25% initial duty cycle
  T2CONbits.ON = 1; // turn on timer2
  OC1CONbits.ON = 1; // turn on OC1

  // digital output setup for motor direction
  TRISBbits.TRISB1 = 0; // RB1 is output

  // Timer setup for 5kHz ISR for current control
  T4CONbits.TCKPS = 2; // Prescaler N = 4
  PR4 = 3999; // Period = (PR4+1) * N * 12.5 ns = 0.2 ms. f = 5kHz
  TMR4 = 0; // initial timer4 count = 0
  T4CONbits.ON = 1; // turn on Timer4************************
  IPC4bits.T4IP = 5; // interrupt priority is 5
  IPC4bits.T4IS = 0; // interrupt subpriority is 0
  IFS0bits.T4IF = 0; // clear interrupt flag
  IEC0bits.T4IE = 1; // enable interrupt
}
