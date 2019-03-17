#include "utilities.h"

enum mode state;

int get_mode(void){
  return (int) state;
}

void set_mode(enum mode new_state){
  state = new_state;
}
