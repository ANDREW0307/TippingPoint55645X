#include "main.h"
#include "okapi/api.hpp"


using namespace okapi;







std::shared_ptr<OdomChassisController> chassis =
  ChassisControllerBuilder()
    .withMotors({20,19}, {9,17}) // left motor is 1, right motor is 2 (reversed)
    // green gearset, 4 inch wheel diameter, 11.5 inch wheel track
    .withDimensions({AbstractMotor::gearset::blue, 7.0/3.0}, {{4_in, 14.625_in}, imev5BlueTPR})
		.withMaxVelocity(200)
    // left encoder in ADI ports A & B, right encoder in ADI ports C & D
    .withSensors(ADIEncoder{'A', 'B'}, ADIEncoder{'C', 'D', true}, {ADIEncoder{'G', 'H'}})
    // specify the tracking wheels diameter (2.75 in), track (7 in), and TPR (360)
    .withOdometry({{2.75_in, 14.75_in , 3_in, 2.75_in}, quadEncoderTPR}, StateMode::FRAME_TRANSFORMATION)
    .buildOdometry();


		std::shared_ptr<ChassisController> myChassis =
		  ChassisControllerBuilder()
		    .withMotors({20,19}, {9,17})
		    // Green gearset, 4 in wheel diam, 11.5 in wheel track
		    .withDimensions({AbstractMotor::gearset::blue, 7.0/3.0}, {{4_in, 14.625_in}, imev5BlueTPR})
				.withSensors(ADIEncoder{'A', 'B'}, ADIEncoder{'C', 'D', true}, {ADIEncoder{'G', 'H'}})
		    .build();


 std::shared_ptr<AsyncMotionProfileController> profileController =
   AsyncMotionProfileControllerBuilder()
     .withLimits({
       .5, // Maximum linear velocity of the Chassis in m/s
       1.0, // Maximum linear acceleration of the Chassis in m/s/s
      5.0 // Maximum linear jerk of the Chassis in m/s/s/s
     })
     .withOutput(myChassis)
     .buildMotionProfileController();



		 // make a file for globals later



		 const double liftkP_front = 0.00295;
		 const double liftkI_front = 0.00;
		 const double liftkD_front = 0.00001;

	std::shared_ptr<AsyncPositionController<double, double>> liftController_front =
		   AsyncPosControllerBuilder()
		     .withMotor({15, 14}) // lift motor port 3
		     .withGains({liftkP_front, liftkI_front, liftkD_front})
				 .withGearset({AbstractMotor::gearset::green, 5.0/1.0})
		     .build();


				 const double liftkP_back = 0.00375;
				 const double liftkI_back = 0.00;
				 const double liftkD_back = 0.0000;

				 std::shared_ptr<AsyncPositionController<double, double>> liftController_back =
				   AsyncPosControllerBuilder()
				     .withMotor(-13) // lift motor port 3
				     .withGains({liftkP_back, liftkI_back, liftkD_back})
						 .withGearset({AbstractMotor::gearset::red, 5.0/1.0})
				     .build();




void rampingTwoGoals() {

}




/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */






void opcontrol() {



// 290 POINTS SKILLS ROUTE
//



	pros::Controller master(pros::E_CONTROLLER_MASTER);


		int piston_front_value = 0;
		int piston_back_value = 0;



	front_piston.set_value(false);
	back_piston.set_value(false);

	armLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
	armRight.set_brake_mode(MOTOR_BRAKE_HOLD);

	back_mogo.set_brake_mode(MOTOR_BRAKE_HOLD);


// //FRONT LIFT
// // start from starting lift position
// //top to bottom
// 	liftController_front->waitUntilSettled();
// 	liftController_front->setTarget(-490);
// 	pros::delay(500);
//
// 	// bottom to top
// 	liftController_front->setTarget(0);

// from starting position to scoring position
// liftController_front->setTarget(-195);
// liftController_front->waitUntilSettled();
//



// // BACK LIFT
// // from starting to down position
// liftController_back->setTarget(-445);










	while (true) {


		// drive code
		int power = master.get_analog(ANALOG_LEFT_Y);
		int turn = master.get_analog(ANALOG_RIGHT_X);
		int left = power + turn;
		int right = power - turn;

		frontLeft = left;
		backLeft= left;

		frontRight = right;
		backRight = right;

		// front and back claw variables

		bool isPressedFrontPiston = master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1);
		bool isPressedBackPiston = master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2);


		//front claw piston - top left trigger
		if(isPressedFrontPiston && piston_front_value%2 == 0){
		front_piston.set_value(true);
		piston_front_value++;
		}

		else if(isPressedFrontPiston && piston_front_value%2 == 1){
		front_piston.set_value(false);
		piston_front_value++;
		}


		// //back claw piston - bottom left trigger
		//
		if(isPressedBackPiston && piston_back_value%2 == 0){
		back_piston.set_value(true);
		piston_back_value++;
		}

		else if(isPressedBackPiston && piston_back_value%2 == 1){
		back_piston.set_value(false);
		piston_back_value++;
		}


		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
		 armLeft.move_velocity(200); //rpm
		 armRight.move_velocity(200); //rpm

	 }
	 else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
		 armLeft.move_velocity(-200);
		 armRight.move_velocity(-200);
	 }
	 else {
		 armLeft.move_velocity(0);
		 armRight.move_velocity(0);


	 }



	 if (master.get_digital(DIGITAL_Y)) {
		conveyor.move_velocity(600); //rpm
	}
	else if (master.get_digital(DIGITAL_RIGHT)) {
		conveyor.move_velocity(-600);
	}
	else {
		conveyor.move_velocity(0);
	}




		pros::delay(20);
	}
}
