#include "main.h"


// drive motors
extern pros::Motor frontLeft;
extern pros::Motor backLeft;
extern pros::Motor frontRight;
extern pros::Motor backRight;

// sucks in rings
extern pros::Motor conveyor;

// 2 motors on the front arm
extern pros::Motor armLeft;
extern pros::Motor armRight;

// 1 motor for the back arm
extern pros::Motor back_mogo;

// inertial sensor
extern pros::IMU inertial;

// encoders for accurate autonomous drive control
extern pros::ADIEncoder leftEncoder;
extern pros::ADIEncoder rightEncoder;
extern pros::ADIEncoder midEncoder;

// front and back pistons
extern pros::ADIDigitalOut front_piston;
extern pros::ADIDigitalOut back_piston;

// bumpers for the front and back arms - up means the arm is the highest it can be, down means lowest
extern pros::ADIButton bumper_front_up;
extern pros::ADIButton bumper_front_down;
extern pros::ADIButton bumper_back_up;
extern pros::ADIButton bumper_back_down;

// limit switches in the arms that detect mogos
extern pros::ADIDigitalIn limit_front;

extern pros::ADIAnalogIn lineTracker_back;

// vex v5 controller
extern pros::Controller master;
