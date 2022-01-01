#include "main.h"

pros::Motor frontLeft(20, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor backLeft(19, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor frontRight(9, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor backRight(17, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor conveyor(11, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor armLeft(15, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor armRight(14, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor back_mogo(13, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_COUNTS);

pros::IMU inertial(12);

pros::ADIEncoder leftEncoder('A','B');
pros::ADIEncoder rightEncoder('C','D', true);
pros::ADIEncoder midEncoder('G', 'H');


pros::ADIDigitalOut front_piston('E');
pros::ADIDigitalOut back_piston('F');
