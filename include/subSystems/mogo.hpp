#include "main.h"

// extern bool frontGoUp;
// extern bool frontGoDown;

// velocity used in driver control along with the control function / task
extern int back_mogo_velocity;

// for driver control - if L2 is pressed and mogo is up, mogo goes down and piston opens.
// if l2 is pressed and mogo is down, mogo goes up and piston closes
extern void backMogoControl(void *param);


// for auton
void front_mogo_up(void *param);
void front_mogo_down(void *param);
