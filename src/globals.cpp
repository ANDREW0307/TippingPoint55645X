#include "main.h"

pros::Motor frontLeft(20, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor backLeft(19, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor frontRight(9, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor backRight(17, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor conveyor(12, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor armLeft(15, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor armRight(5, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor back_mogo(4, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_COUNTS);

pros::IMU inertial(11);

pros::ADIEncoder leftEncoder('A','B', false);
pros::ADIEncoder rightEncoder({2, 'A','B'}, true);
pros::ADIEncoder midEncoder('G', 'H', true);


pros::ADIDigitalOut front_piston('F');
pros::ADIDigitalOut back_piston('E');

pros::ADIButton bumper_front_up('A');
pros::ADIButton bumper_front_down('B');
pros::ADIButton bumper_back_up('H');
pros::ADIButton bumper_back_down('C');

pros::ADIDigitalIn limit_front( 'G');

pros::ADIAnalogIn lineTracker_back('D');


pros::Controller master(pros::E_CONTROLLER_MASTER);
