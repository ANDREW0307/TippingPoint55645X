#include "main.h"

// makes sure back mogo control function is running throughout driver control
extern pros::task_t back_mogo_control_task;

// tells bot to move arms up and down during auton
extern pros::task_t frontUpAutonTask;
extern pros::task_t frontDownAutonTask;
