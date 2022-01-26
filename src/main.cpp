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
void competition_initialize() {

		inertial.reset();

		while(inertial.is_calibrating()) {
			pros::lcd::set_text(1, "inertial is calibrating");
			pros::delay(300);
		}



		pros::lcd::set_text(1, "inertial is done calibrating");



}

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

 pros::Task auton_task1 (front_mogo_up, NULL, TASK_PRIORITY_DEFAULT,
 TASK_STACK_DEPTH_DEFAULT, "My Task");

  pros::Task auton_task2 (frontGrabbing, NULL, TASK_PRIORITY_DEFAULT,
  TASK_STACK_DEPTH_DEFAULT, "My Task");

  pros::Task auton_task3 (backGrabbing, NULL, TASK_PRIORITY_DEFAULT,
  TASK_STACK_DEPTH_DEFAULT, "My Task");

  pros::Task auton_task4 (back_mogo_up, NULL, TASK_PRIORITY_DEFAULT,
  TASK_STACK_DEPTH_DEFAULT, "My Task");




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

	auton_task1.suspend();
	auton_task2.suspend();
		auton_task3.suspend();
			auton_task4.suspend();



	armLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
	armRight.set_brake_mode(MOTOR_BRAKE_HOLD);
	back_mogo.set_brake_mode(MOTOR_BRAKE_HOLD);
	conveyor.set_brake_mode(MOTOR_BRAKE_COAST);




	pros::lcd::set_text(1, "Driving");


		// frontGoUp = false;
		// frontGoDown = false;


		int piston_front_value = 0;
		int piston_back_value = 0;



		bool isUpBackMogo = true;


	front_piston.set_value(false);
	back_piston.set_value(false);





	pros::task_t back_mogo_control_task = pros::c::task_create(backMogoControl, (void*)"PROS", TASK_PRIORITY_DEFAULT,
	TASK_STACK_DEPTH_DEFAULT, "My Task");
	pros::Task my_cpp_task (back_mogo_control_task);

	pros::task_t conveyor_control_task = pros::c::task_create(conveyorControl, (void*)"PROS", TASK_PRIORITY_DEFAULT,
	TASK_STACK_DEPTH_DEFAULT, "My Task");
	pros::Task conveyortaskInit (conveyor_control_task);


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


		if (master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
			frontLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
			frontRight.set_brake_mode(MOTOR_BRAKE_HOLD);
			backLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
			backRight.set_brake_mode(MOTOR_BRAKE_HOLD);

		}

		bool isPressedFrontPiston = master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1);







		back_mogo.move_velocity(back_mogo_velocity);

		conveyor.move_velocity(conveyorVelocity);



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

		if (back_mogo.get_voltage() < -1000) {
			back_piston.set_value(true);
		}





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


































  std::shared_ptr<OdomChassisController> chassis =
    ChassisControllerBuilder()
      .withMotors({20,19}, {9,17})
      // .withGains(
      //   {0.0017, 0, 0.0000025}, // distance controller gains
      //   {0.00, 0, 0.00}, // turn controller gains
      //   {0.0000, 0, 0.000}  // angle controller gains (helps drive straight)
      // )
      .withDimensions({AbstractMotor::gearset::green}, {{4_in, 14.625_in}, imev5GreenTPR})
  		.withMaxVelocity(155)
      .withSensors(ADIEncoder{'A', 'B', false}, ADIEncoder{{6, 'A','B'}, true}, {ADIEncoder{'G', 'H', false}})
      .withOdometry({{2.75_in, 8.75_in , 1.75_in, 2.75_in}, quadEncoderTPR}, StateMode::CARTESIAN)
      .buildOdometry();


	std::shared_ptr<ChassisController> myChassis =
		 ChassisControllerBuilder()
		  .withMotors({20,19}, {9,17})
	    .withDimensions({AbstractMotor::gearset::green}, {{4_in, 14.625_in}, imev5GreenTPR})
      .build();

   std::shared_ptr<AsyncMotionProfileController> profileControllerFast =
     AsyncMotionProfileControllerBuilder()
       .withLimits({
         .85, // Maximum linear velocity of the Chassis in m/s
         2.0, // Maximum linear acceleration of the Chassis in m/s/s
        5.0 // Maximum linear jerk of the Chassis in m/s/s/s
       })
       .withOutput(myChassis)
       .buildMotionProfileController();

		std::shared_ptr<AsyncMotionProfileController> profileControllerSlow =
			 AsyncMotionProfileControllerBuilder()
				 .withLimits({
					 .17, // Maximum linear velocity of the Chassis in m/s
					 0.8, // Maximum linear acceleration of the Chassis in m/s/s
					2.5 // Maximum linear jerk of the Chassis in m/s/s/s
				 })
				 .withOutput(myChassis)
				 .buildMotionProfileController();


         int velCap;
         void drivePIDFn(int targetLeft, int targetRight, int tolerance, int straight_tolerance, float kp){
           leftEncoder.reset();
           rightEncoder.reset();
           int errorLeft = targetLeft;
           int errorRight = targetRight;
           float kpTurn = 0.1;
           int acc = 5;
           int voltageLeft = 0;
           int voltageRight = 0;
           int signLeft;
           int signRight;

           while(abs(errorLeft) > abs(tolerance) || abs(errorRight) > abs(tolerance)){
             errorLeft = targetLeft - leftEncoder.get_value(); //error is target minus actual value
             errorRight = targetRight - rightEncoder.get_value();

             signLeft = (errorLeft > 0) - (errorLeft < 0); // + or -
             signRight = (errorRight > 0) - (errorRight < 0);

             pros::lcd::print(4, "%d", leftEncoder.get_value());
             pros::lcd::print(5, "%d", rightEncoder.get_value());



             if(signLeft == signRight){
               voltageLeft = errorLeft * kp; //intended voltage is error times constant
               voltageRight = errorRight * kp;
             }
             else {
               voltageLeft = errorLeft * kpTurn; //same logic with different turn value
               voltageRight = errorRight * kpTurn;
             }

             velCap = velCap + acc;  //slew rate
             if(velCap > 95){
               velCap = 95; //velCap cannot exceed 120
             }
             if(abs(voltageLeft) > velCap){ //limit the voltage
               voltageLeft = velCap * signLeft;
             }
             if(abs(voltageRight) > velCap){ //ditto
               voltageRight = velCap * signRight;
             }
             //set the motors to the intended speed
             frontRight = voltageRight;
             backRight = voltageRight;

             frontLeft = voltageLeft;
             backLeft = voltageLeft;

             pros::delay(20);
           }




         }


         void reset_drive() {



           //wwrognwornowngoweg

           frontRight = 0;
           backRight = 0;

           frontLeft = 0;
           backLeft = 0;

         }





             bool frontGoUp = false;
             bool frontGoDown = false;


             void front_mogo_up(void *ign) {
               while (true) {
                 while (frontGoUp) {
                   if (!bumper_front_up.get_value()) {
                     armLeft.move_velocity(200);
                     armRight.move_velocity(200);
                   }
                   if (bumper_front_up.get_value() && armLeft.get_voltage() > 1200) {
                     armLeft.move_velocity(0);
                     armRight.move_velocity(0);
                   }
                   pros::delay(80);
                 }
                 while (frontGoDown) {
                   if (!bumper_front_down.get_value()) {
                     armLeft.move_velocity(-200);
                     armRight.move_velocity(-200);
                   }
                   if (bumper_front_down.get_value() && armLeft.get_voltage() < -1200) {
                     armLeft.move_velocity(0);
                     armRight.move_velocity(0);
                   }
                   pros::delay(80);
                 }


                 pros::delay(40);
               }

             }


             bool holdOnFront = true;

             void frontGrabbing(void *ign) {
               while (true) {
                 if (limit_front.get_value() && holdOnFront) {
                   front_piston.set_value(true);
                 } else {
                   front_piston.set_value(false);
                 }
                 pros::delay(100);
               }
             }




             bool backGoUp = false;
             bool backGoDown = false;


             void back_mogo_up(void *ign) {
               while (true) {
                 while (backGoUp) {
                   if (!bumper_back_up.get_value()) {
                     back_mogo.move_velocity(-200);
                   }
                    if (bumper_back_up.get_value() && back_mogo.get_voltage() < -1200) {
                     back_mogo.move_velocity(0);

                   }
                   pros::delay(80);
                 }
                 while (backGoDown) {
                   if (!bumper_back_down.get_value()) {
                     back_mogo.move_velocity(200);

                   }
                    if (bumper_back_down.get_value() && back_mogo.get_voltage() > 1200) {
                     back_mogo.move_velocity(0);

                   }
                   pros::delay(80);
                 }


                 pros::delay(40);
               }

             }




             bool holdOnBack = true;

             void backGrabbing(void *ign) {
               while (true) {
                 if (lineTracker_back.get_value() < 2850 && holdOnBack) {
                   back_piston.set_value(true);
                 } else {
                   back_piston.set_value(false);
                 }
                 pros::lcd::set_text(3, std::to_string(lineTracker_back.get_value()));
                 pros::delay(100);
               }
             }




void resetAll() {
  inertial.set_heading(0);
  chassis->setState({0_in, 0_in, 0_deg});
  leftEncoder.reset();
  rightEncoder.reset();
  midEncoder.reset();
}


void resetEncoders() {
  leftEncoder.reset();
  rightEncoder.reset();
  midEncoder.reset();
}













void testingAuton() {

		// while(true) {
		// 	std::string s = std::to_string(rightEncoder.get_value());
		// 	std::string t = std::to_string(leftEncoder.get_value());
		//
		// 	pros::lcd::set_text(1, s + "right");
		// 	pros::lcd::set_text(2, t + "left");
		//
		// 	pros::delay(500);
		// }


			resetAll();
			resetAll();



    chassis->setState({0_ft,0_ft, 180_deg});

		backGoUp = false;
    backGoDown = true;
    drivePIDFn(-650, -650, 25, 30, .3);

    reset_drive();
    backGoDown = false;
    backGoUp = true;

    profileControllerSlow->generatePath({{0_ft, 0_ft, 180_deg}, {0.5_ft, 1_ft, 90_deg}}, "B");
    profileControllerSlow->setTarget("B", false, true);
    profileControllerSlow->waitUntilSettled();




  QAngle inertialPosition = inertial.get_heading() * degree;
  chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});


  frontGoDown = true;
  chassis->turnToAngle(112_deg);
  chassis->waitUntilSettled();

  inertialPosition = inertial.get_heading() * degree;
  chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});

  chassis->turnToAngle(112_deg);
  chassis->waitUntilSettled();

  inertialPosition = inertial.get_heading() * degree;
  chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});

  chassis->turnToAngle(112_deg);
  chassis->waitUntilSettled();




  drivePIDFn(1850, 1850, 50, 30, .3);
  reset_drive();

  frontGoDown = false;
  frontGoUp = true;

  pros::delay(1000);

  inertialPosition = inertial.get_heading() * degree;
  chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});

  chassis->turnToAngle(90_deg);
  chassis->waitUntilSettled();


  inertialPosition = inertial.get_heading() * degree;
  chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});

  chassis->turnToAngle(90_deg);
  chassis->waitUntilSettled();


  resetAll();










  frontLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
  frontRight.set_brake_mode(MOTOR_BRAKE_HOLD);
  backLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
  backRight.set_brake_mode(MOTOR_BRAKE_HOLD);


  profileControllerFast->generatePath({{0_ft, 0_ft, 0_deg}, {2.5_ft, 3.05_ft, 0_deg}}, "C");
  profileControllerFast->setTarget("C", false, true);
  profileControllerFast->waitUntilSettled();









  resetAll();


  profileControllerSlow->generatePath({{0_ft, 0_ft, 0_deg}, {3.5_ft, 0_ft, 0_deg}}, "Z");
  profileControllerSlow->setTarget("Z", true, false);
  profileControllerSlow->waitUntilSettled();


  profileControllerSlow->generatePath({{0_ft, 0_ft, 0_deg}, {2_ft, 0_ft, 0_deg}}, "Y");
  profileControllerSlow->setTarget("Y", false, false);
  profileControllerSlow->waitUntilSettled();


    backGoUp = false;
    backGoDown = true;

    holdOnBack = false;



  frontGoUp = false;

    armLeft.move_relative(-630, 100);
    armRight.move_relative(-630, 100);

  profileControllerFast->generatePath({{0_ft, 0_ft, 0_deg}, {2.65_ft, 0_ft, 0_deg}}, "D");
  profileControllerFast->setTarget("D", false, false);
  profileControllerFast->waitUntilSettled();

  armLeft.move_relative(-660, 50);
  armRight.move_relative(-660, 50);

  pros::delay(500);

  holdOnFront = false;
  holdOnBack = true;

  backGoDown = false;
  backGoUp = true;



  frontGoUp = true;

  frontLeft.set_brake_mode(MOTOR_BRAKE_COAST);
  frontRight.set_brake_mode(MOTOR_BRAKE_COAST);
  backLeft.set_brake_mode(MOTOR_BRAKE_COAST);
  backRight.set_brake_mode(MOTOR_BRAKE_COAST);


  profileControllerFast->generatePath({{0_ft, 0_ft, 0_deg}, {1_ft, 0_ft, 0_deg}}, "E");
  profileControllerFast->setTarget("E", true, false);
  profileControllerFast->waitUntilSettled();

  // inertial.set_heading(0);

  inertialPosition = inertial.get_heading() * degree;
  chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});

  chassis->turnToAngle(180_deg);
  chassis->waitUntilSettled();

  frontGoUp = false;
  frontGoDown = true;

  holdOnFront = true;

  inertialPosition = inertial.get_heading() * degree;
  chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});

  chassis->turnToAngle(180_deg);
  chassis->waitUntilSettled();



  profileControllerFast->generatePath({{0_ft, 0_ft, 0_deg}, {2.5_ft, 0_ft, 0_deg}}, "F");
  profileControllerFast->setTarget("F", false, false);
  profileControllerFast->waitUntilSettled();

  frontGoDown = false;
  frontGoUp = true;

  //
  //
  // inertialPosition = inertial.get_heading() * degree;
  // chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});
  // chassis->turnToAngle(0_deg);
  // chassis->waitUntilSettled();
  // inertialPosition = inertial.get_heading() * degree;
  // chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});
  // chassis->turnToAngle(0_deg);
  // chassis->waitUntilSettled();
  //
  //
  // profileControllerFast->setTarget("F", false, false);
  // profileControllerFast->waitUntilSettled();
  //
  // frontGoUp = false;
  // frontGoDown = false;
  //
  // armLeft.move_relative(-750, 200);
  // armRight.move_relative(-750, 200);
  //
  // profileControllerSlow->generatePath({{0_ft, 0_ft, 0_deg}, {1.75_ft, 0_ft, 0_deg}}, "G");
  // profileControllerSlow->setTarget("G", false, false);
  // profileControllerSlow->waitUntilSettled();
  //
  // holdOnFront = false;
  // pros::delay(500);
  //
  //
  // frontGoUp = true;
  //
  // profileControllerSlow->setTarget("G", true, false);
  // profileControllerSlow->waitUntilSettled();
  //
  // chassis->driveToPoint({0_ft, 0_ft});
  // chassis->waitUntilSettled();
  //
  // drivePIDFn(-500, 500, 55, 100, 3.96);
  // reset_drive();
















	//
  // chassis->turnToAngle(270_deg);
  // chassis->waitUntilSettled();
	//
  // QAngle inertialPosition = inertial.get_heading() * degree;
  // chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});
	//
  // chassis->turnToAngle(270_deg);
  // chassis->waitUntilSettled();
	//
	//
  // profileControllerFast->generatePath({{0_ft, 0_ft, 0_deg}, {4_ft, 0_ft, 0_deg}}, "U");
  // profileControllerFast->setTarget("U", true, false);
  // profileControllerFast->waitUntilSettled();
	//
	//
	// frontGoUp = false;
	// frontGoDown = true;
	//
	// resetAll();
	//
	// inertialPosition = inertial.get_heading() * degree;
  // chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});
	//
	// chassis->turnToAngle(50_deg);
	// chassis->waitUntilSettled();
	//
	//
	//
	//   profileControllerFast->generatePath({{0_ft, 0_ft, 0_deg}, {2.25_ft, 0_ft, 0_deg}}, "T");
	//   profileControllerFast->setTarget("T", false, false);
	//   profileControllerFast->waitUntilSettled();
	//
	// 	pros::delay(500);
	//
	//
	// 	profileControllerFast->setTarget("T", true, false);
	// 	profileControllerFast->waitUntilSettled();
	//
	//
	// 			frontGoDown = false;
	// 			frontGoUp = true;
	//
	//
	// 			inertialPosition = inertial.get_heading() * degree;
	// 			chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});
	//
	// 			chassis->turnToAngle(0_deg);
	// 			chassis->waitUntilSettled();
	//
	//
	// 			profileControllerFast->setTarget("U", false, false);
	// 			profileControllerFast->waitUntilSettled();
	//
	// 			inertialPosition = inertial.get_heading() * degree;
	// 			chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});
	//
	// 			chassis->turnToAngle(180_deg);
	// 			chassis->waitUntilSettled();
	//
	//
	// 							inertialPosition = inertial.get_heading() * degree;
	// 							chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});
	//
	// 			chassis->turnToAngle(180_deg);
	// 			chassis->waitUntilSettled();
	//
	// 			backGoUp = false;
	// 			backGoDown = true;
	//
	// 			profileControllerFast->setTarget("U", true, false);
	// 			profileControllerFast->waitUntilSettled();
	//
	// 			profileControllerFast->setTarget("T", true, false);
	// 			profileControllerFast->waitUntilSettled();
	//
	// 			backGoDown = false;
	// 			backGoUp = true;

			// resetAll();
		 //
			// profileControllerFast->generatePath({{0_ft, 0_ft, 0_deg}, {0.5_ft, 0_ft, 0_deg}}, "E");
			// profileControllerFast->setTarget("E", false, false);
			// profileControllerFast->waitUntilSettled();
		 //
			// QAngle inertialPosition = inertial.get_heading() * degree;
			// chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});
		 //
			// chassis->turnToAngle(90_deg);
			// chassis->waitUntilSettled();
		 //
			// inertialPosition = inertial.get_heading() * degree;
		 // chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});
		 //
		 // chassis->turnToAngle(90_deg);
		 // chassis->waitUntilSettled();
		 //
			// frontGoUp = false;
			// frontGoDown = false;
		 //
			// armLeft.move_relative(-350, 200);
			// armRight.move_relative(-350, 200);
		 //
			// conveyor.move_velocity(600);
		 //
		 //
			// profileControllerFast->generatePath({{0_ft, 0_ft, 0_deg}, {4_ft, 0_ft, 0_deg}}, "Q");
			// profileControllerFast->setTarget("Q", false, false);
			// profileControllerFast->waitUntilSettled();
		 //
			// profileControllerFast->setTarget("Q", false, false);
			// profileControllerFast->waitUntilSettled();
		 //
		 //
		 //
			// conveyor.move_velocity(0);
		 //
			//  inertialPosition = inertial.get_heading() * degree;
			// chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});
			// chassis->turnToAngle(0_deg);
			// chassis->waitUntilSettled();
		 //
		 //
			// frontGoUp = false;
			// frontGoDown = true;
			//
			// frontLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
			// frontRight.set_brake_mode(MOTOR_BRAKE_HOLD);
			// backLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
			// backRight.set_brake_mode(MOTOR_BRAKE_HOLD);
			//
			// pros::delay(2000);
			//
			// frontGoUp = false;
			// frontGoDown = false;
			//
			// armLeft.move_relative(-350, 200);
			// armRight.move_relative(-350, 200);
			//
			// while(inertial.get_yaw() > -5) {
			// 	std::string s = std::to_string(inertial.get_yaw());
			// 	pros::lcd::set_text(1, s);
			// 	pros::delay(300);
			// 	frontLeft.move_velocity(100);
			// 	frontRight.move_velocity(100);
			// 	backLeft.move_velocity(100);
			// 	backRight.move_velocity(100);
			//
			// }

}










void left_side_panic() {
	// resetAll();
	// frontGoUp = false;
	// frontGoDown = false;
	//
	// armLeft.move_relative(-630, 100);
	// armRight.move_relative(-630, 100);
	//
	// backGoUp = false;
	// backGoDown = true;
	// drivePIDFn(-800, -800, 100, 100, 1.5);
	// reset_drive();
	//
	// backGoUp = true;
	// backGoDown = false;
	//
	// pros::delay(2000);
	//
	// conveyor.move_velocity(600);
	//
	// drivePIDFn(-100, -100, 100, 100, 1.5);
	// resetAll();



	resetAll();

	armLeft.move_velocity(-200);
	armRight.move_velocity(-200);

	profileControllerFast->generatePath({{0_ft, 0_ft, 0_deg}, {4_ft, 0_ft, 0_deg}}, "F");
	profileControllerFast->setTarget("F", false, false);
	profileControllerFast->waitUntilSettled();

	armLeft.move_velocity(0);
	armLeft.move_velocity(0);

	pros::delay(3000);
	// // frontGoUp = false;
	// // frontGoDown = true;
	// //
	// // holdOnFront = true;
	//
	//
	// 	armLeft.move_velocity(0);
	// 	armLeft.move_velocity(0);
	//
	// 	holdOnFront = false;
	//
	// profileControllerFast->generatePath({{0_ft, 0_ft, 0_deg}, {4_ft, 0_ft, 0_deg}}, "F");
	// profileControllerFast->setTarget("F", false, false);
	// profileControllerFast->waitUntilSettled();
	//
	// holdOnFront = true;
	// front_piston.set_value(true);
	//
	// pros::delay(2000);
	//
	//
	// frontGoDown = false;
	// frontGoUp = true;
	//
	// pros::delay(1000);
	//
	// backGoUp = false;
	// backGoDown = true;
	//
	// profileControllerSlow->generatePath({{0_ft, 0_ft, 0_deg}, {2_ft, 2.25_ft, 45_deg}}, "Q");
	// profileControllerSlow->setTarget("Q", true, true);
	// profileControllerSlow->waitUntilSettled();
	//
	// backGoDown = false;
	// backGoUp = true;
	//
	// frontGoDown = false;
	// frontGoUp = false;
	//
	//
	//
	// armLeft.move_relative(-630, 100);
	// armRight.move_relative(-630, 100);
	//
	// pros::delay(2000);
	//
	//
	// conveyor.move_velocity(600);

	profileControllerSlow->generatePath({{0_ft, 0_ft, 0_deg}, {1.25_ft, 0_ft, 0_deg}}, "J");
	profileControllerSlow->setTarget("J", false, false);
	profileControllerSlow->waitUntilSettled();

}













void autonomous() {



	armLeft.set_brake_mode(MOTOR_BRAKE_HOLD);
	armRight.set_brake_mode(MOTOR_BRAKE_HOLD);
	back_mogo.set_brake_mode(MOTOR_BRAKE_HOLD);



	left_side_panic();
	// testingAuton();


}
