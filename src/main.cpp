#include "main.h"
#include "okapi/api.hpp"


using namespace okapi;



/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
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

	pros::lcd::register_btn1_cb(on_center_button);
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
pros::ADIEncoder rightEncoder({6, 'A','B'}, true);
pros::ADIEncoder midEncoder('G', 'H');


pros::ADIDigitalOut front_piston({{6, 'G'}});
pros::ADIDigitalOut back_piston('F');

pros::ADIButton bumper_front_up({6,'C'});
// pros::ADIButton bumper_front_down({6,'C'});
pros::ADIButton bumper_back_up({6,'D'});
pros::ADIButton bumper_back_down({6,'H'});





std::shared_ptr<OdomChassisController> chassis =
  ChassisControllerBuilder()
    .withMotors({20,19}, {9,17}) // left motor is 1, right motor is 2 (reversed)
    // green gearset, 4 inch wheel diameter, 11.5 inch wheel track
    .withDimensions({AbstractMotor::gearset::blue, 7.0/3.0}, {{4_in, 14.625_in}, imev5BlueTPR})
		.withMaxVelocity(200)
    // left encoder in ADI ports A & B, right encoder in ADI ports C & D
    .withSensors(ADIEncoder{'A', 'B'}, ADIEncoder{{6, 'A','B'}, true}, {ADIEncoder{'G', 'H'}})
    // specify the tracking wheels diameter (2.75 in), track (7 in), and TPR (360)
    .withOdometry({{2.75_in, 14.75_in , 3_in, 2.75_in}, quadEncoderTPR}, StateMode::FRAME_TRANSFORMATION)
    .buildOdometry();


		std::shared_ptr<ChassisController> myChassis =
		  ChassisControllerBuilder()
		    .withMotors({20,19}, {9,17})
		    // Green gearset, 4 in wheel diam, 11.5 in wheel track
		    .withDimensions({AbstractMotor::gearset::blue, 7.0/3.0}, {{4_in, 14.625_in}, imev5BlueTPR})
				.withSensors(ADIEncoder{'A', 'B'}, ADIEncoder{{6, 'A','B'}, true}, {ADIEncoder{'G', 'H'}})
		    .build();


 std::shared_ptr<AsyncMotionProfileController> profileControllerFast =
   AsyncMotionProfileControllerBuilder()
     .withLimits({
       .5, // Maximum linear velocity of the Chassis in m/s
       2.0, // Maximum linear acceleration of the Chassis in m/s/s
      5.0 // Maximum linear jerk of the Chassis in m/s/s/s
     })
     .withOutput(myChassis)
     .buildMotionProfileController();

		 std::shared_ptr<AsyncMotionProfileController> profileControllerSlow =
			 AsyncMotionProfileControllerBuilder()
				 .withLimits({
					 .25, // Maximum linear velocity of the Chassis in m/s
					 1.0, // Maximum linear acceleration of the Chassis in m/s/s
					2.5 // Maximum linear jerk of the Chassis in m/s/s/s
				 })
				 .withOutput(myChassis)
				 .buildMotionProfileController();



		 // make a file for globals later



		 const double liftkP_front = 0.00295;
		 const double liftkI_front = 0.00;
		 const double liftkD_front = 0.00001;

		 // std::shared_ptr<AsyncPositionController<double, double>> liftController_front =
		 //   AsyncPosControllerBuilder()
		 //     .withMotor({15, 14}) // lift motor port 3
		 //     .withGains({liftkP_front, liftkI_front, liftkD_front})
			// 	 .withGearset({AbstractMotor::gearset::green, 5.0/1.0})
		 //     .build();
		 //

				 const double liftkP_back = 0.00375;
				 const double liftkI_back = 0.00;
				 const double liftkD_back = 0.0000;

				 // std::shared_ptr<AsyncPositionController<double, double>> liftController_back =
				 //   AsyncPosControllerBuilder()
				 //     .withMotor(-13) // lift motor port 3
				 //     .withGains({liftkP_back, liftkI_back, liftkD_back})
					// 	 .withGearset({AbstractMotor::gearset::red, 5.0/1.0})
				 //     .build();




void backMogoUp() {

	while (!bumper_back_up.get_value()) {

		back_mogo.move_velocity(-100);
		pros::delay(2);
	}
	back_mogo.tare_position();
	back_mogo.move_velocity(0);
}


void opcontrol() {





	pros::Controller master(pros::E_CONTROLLER_MASTER);


		int piston_front_value = 0;
		int piston_back_value = 0;

		int conveyor_value = 0;

		bool isUpBackMogo = true;


	front_piston.set_value(false);
	back_piston.set_value(false);

	armLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
	armRight.set_brake_mode(MOTOR_BRAKE_HOLD);
	back_mogo.set_brake_mode(MOTOR_BRAKE_HOLD);



// 260 skills
//
// chassis->setState({0_ft, 0_ft, 180_deg});
//
// //first 20 seconds
// liftController_back->setTarget(-440);
// chassis->driveToPoint({25_in, 0_in}, true);
// chassis->waitUntilSettled();
// pros::delay(500);
// back_piston.set_value(true);
// liftController_back->setTarget(0);
// liftController_back->waitUntilSettled();
// pros::delay(500);
//
// profileControllerSlow->generatePath({{0_ft, 0_ft, 0_deg},{1_ft,1_ft,0_deg}}, "A");
// profileControllerSlow->setTarget("A", false, true);
// profileControllerSlow->waitUntilSettled();
// profileControllerSlow->removePath("A");
//
//
// liftController_front->setTarget(-480);
// liftController_front->waitUntilSettled();
//
// chassis->driveToPoint({2_ft, -4_ft});
// chassis->waitUntilSettled();
//
//
// front_piston.set_value(true);
// pros::delay(1000);
//
// liftController_front->setTarget(0);
//
// profileControllerFast->generatePath({{0_ft, 0_ft, 10_deg},{3.25_ft,3.25_ft,0_deg}}, "B");
// profileControllerFast->setTarget("B", false, true);
// profileControllerFast->waitUntilSettled();
// profileControllerFast->removePath("B");



bool isPressedBackMogo = false;
int backMogoVelocity = 0;

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



		bool isPressedFrontPiston = master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1);
		bool isPressedBackPiston = master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2);



		bool isPressedConveyor = master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT);




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





		//ARM MOVEMENT

		if (master.get_digital(DIGITAL_R1) && !bumper_front_up.get_value()) {
			armLeft.move_velocity(200);
		 armRight.move_velocity(200);
	 }
	 else if (master.get_digital(DIGITAL_R2)) {
		 armLeft.move_velocity(-200);
		 armRight.move_velocity(-200);
	 }
	 else {
		 armLeft.move_velocity(0);
		 armRight.move_velocity(0);
	 }



	//
	//  if (isPressedConveyor && conveyor_value % 2 == 0) {
	// 	conveyor = 127; //rpm
	// 	conveyor_value++;
	// }
	// else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)  && conveyor_value % 2 == 1) {
	// 	conveyor = 0;
	// 	conveyor_value++;
	// }
	// else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP)) {
	// 	conveyor = -127;
	// }
	//
	//
	//




	//WAS PRESSED BACK MOGO
	if (isPressedBackMogo) {
		while (!bumper_back_down.get_value() && isUpBackMogo) {
			backMogoVelocity = 200;
			isUpBackMogo = false;
			break;
		}
		while (!bumper_back_up.get_value() && !isUpBackMogo) {
			backMogoVelocity = -200;
			isUpBackMogo = true;
			break;
		}
		backMogoVelocity = 0;
		isPressedBackMogo = false;
	}

	back_mogo.move_velocity(backMogoVelocity);



















	// int backMogoVelocity;
	//
	// if (bumper_back_up.get_value()) {
	// 	isUpBackMogo = true;
	// } else if (bumper_back_down.get_value()){
	// 	isUpBackMogo = false;
	// }
	//
	// if(isPressedBackMogo && !bumper_back_down.get_value() && isUpBackMogo ) {
	// 	backMogoVelocity = 200;
	// } else if(isPressedBackMogo && !bumper_back_up.get_value() && !isUpBackMogo) {
	// 	backMogoVelocity = -200;
	// } else {
	// 	if (bumper_back_up.get_value() || bumper_back_down.get_value()) {
	// 		backMogoVelocity = 0;
	// 	}
	// }
	//
	// // else {
	// // 	backMogoVelocity = 0;
	// // }
	//
	// back_mogo.move_velocity(backMogoVelocity);
	//






		pros::delay(20);
	}
}
