#include "main.h"

extern pros::Motor frontLeft;
extern pros::Motor backLeft;

extern pros::Motor frontRight;
extern pros::Motor backRight;

extern pros::Motor conveyor;

extern pros::Motor armLeft;
extern pros::Motor armRight;

extern pros::Motor back_mogo;

extern pros::IMU inertial;

extern pros::ADIEncoder leftEncoder;
extern pros::ADIEncoder rightEncoder;
extern pros::ADIEncoder midEncoder;


extern pros::ADIDigitalOut front_piston;
extern pros::ADIDigitalOut back_piston;

extern pros::ADIButton bumper_front_up;
extern pros::ADIButton bumper_front_down;
extern pros::ADIButton bumper_back_up;
extern pros::ADIButton bumper_back_down;

extern pros::ADIDigitalIn limit_front;
extern pros::ADIDigitalIn limit_back;

extern pros::Controller master;
