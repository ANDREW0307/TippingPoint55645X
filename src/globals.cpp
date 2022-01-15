#include "main.h"

pros::Motor frontLeft(20, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor backLeft(19, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor frontRight(9, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor backRight(17, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor conveyor(12, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor armLeft(15, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_COUNTS);
pros::Motor armRight(14, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_COUNTS);

pros::Motor back_mogo(13, pros::E_MOTOR_GEARSET_36, false, pros::E_MOTOR_ENCODER_COUNTS);

pros::IMU inertial(11);

pros::ADIEncoder leftEncoder('A','B');
pros::ADIEncoder rightEncoder({6, 'A','B'}, true);
pros::ADIEncoder midEncoder('G', 'H');


pros::ADIDigitalOut front_piston({{6, 'G'}});
pros::ADIDigitalOut back_piston('F');

pros::ADIButton bumper_front_up({6,'C'});
pros::ADIButton bumper_front_down({6,'E'});
pros::ADIButton bumper_back_up({6,'D'});
pros::ADIButton bumper_back_down('C');

pros::ADIDigitalIn limit_front({6, 'H'});

pros::ADIAnalogIn lineTracker_back('D');


pros::Controller master(pros::E_CONTROLLER_MASTER);
