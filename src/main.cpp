#include "main.h"

using namespace okapi;

#include "main.h"



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
		pros::lcd::initialize();
		pros::lcd::set_text(1, "Initializing");




		front_piston.set_value(true);
		back_piston.set_value(true);

		pros::lcd::set_text(1, "done");







		//GENERATE AUTON PATHFINDER COMMANDS HERE


}



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
void autonomous() {

	armLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
	armRight.set_brake_mode(MOTOR_BRAKE_HOLD);
	back_mogo.set_brake_mode(MOTOR_BRAKE_HOLD);


	left_side();

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



}

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

	armLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
	armRight.set_brake_mode(MOTOR_BRAKE_HOLD);
	back_mogo.set_brake_mode(MOTOR_BRAKE_HOLD);



	pros::lcd::set_text(1, "Driving");


		// frontGoUp = false;
		// frontGoDown = false;


		int piston_front_value = 0;
		int piston_back_value = 0;

		bool conveyorON = false;

		bool isUpBackMogo = true;


	front_piston.set_value(false);
	back_piston.set_value(false);





	pros::task_t back_mogo_control_task = pros::c::task_create(backMogoControl, (void*)"PROS", TASK_PRIORITY_DEFAULT,
	TASK_STACK_DEPTH_DEFAULT, "My Task");
	pros::Task my_cpp_task (back_mogo_control_task);



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


		back_mogo.move_velocity(back_mogo_velocity);


		//front claw piston - top left trigger
		if(isPressedFrontPiston && piston_front_value%2 == 0){
		front_piston.set_value(true);
		piston_front_value++;
		}

		else if(isPressedFrontPiston && piston_front_value%2 == 1){
		front_piston.set_value(false);
		piston_front_value++;
		}



		if (back_mogo.get_voltage() > 1000) {
			back_piston.set_value(false);
		}

		if (back_mogo.get_voltage() < -1000 && back_mogo.get_voltage() ) {
			back_piston.set_value(true);
		}


		std::string s = std::to_string(42);
		pros::lcd::set_text(5, std::to_string(piston_back_value));





		//ARM MOVEMENT

		if (master.get_digital(DIGITAL_R1) && !bumper_front_up.get_value()) {
			armLeft.move_velocity(200);
		 armRight.move_velocity(200);
	 }
	 else if (master.get_digital(DIGITAL_R2) && !bumper_front_down.get_value()) {
		 armLeft.move_velocity(-200);
		 armRight.move_velocity(-200);
	 }
	 else {
		 armLeft.move_velocity(0);
		 armRight.move_velocity(0);
	 }






		pros::delay(20);
	}
}
