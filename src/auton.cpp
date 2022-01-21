#include "main.h"

using namespace okapi;


#include "main.h"



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
      .withSensors(ADIEncoder{'A', 'B', false}, ADIEncoder{{6, 'A','B'}}, {ADIEncoder{'G', 'H', false}})
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
         .5, // Maximum linear velocity of the Chassis in m/s
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

           int voltageLeft, voltageRight = 0;

           frontRight = voltageRight;
           backRight = voltageRight;

           frontLeft = voltageLeft;
           backLeft = voltageLeft;

         }



         float vel_cap_turn;

         void simple_turn_right(int degrees_target, int tolerance, float kP_turn) {


           float degrees_current = 0;


             int left_sign;
             int right_sign;
             float voltage_left;
             float voltage_right;
             int acc = 5;

           while (degrees_current < degrees_target - tolerance) {
             if (inertial.get_heading() > 350) {
             }
             std::cout << "sensor value: " << inertial.get_heading() << std::endl;


             degrees_current = inertial.get_heading();
             float error_heading = degrees_target - degrees_current; // error - target - current

           std::cout << "error value: " << error_heading << std::endl;

             int left_sign = (error_heading < 0) - (error_heading > 0);
             int right_sign = (error_heading > 0)  - (error_heading < 0);


             vel_cap_turn = vel_cap_turn + acc;  //slew rate
             if(vel_cap_turn > 95){
               vel_cap_turn = 95; //velCap cannot exceed 120
             }

             if(fabs(voltage_left) > vel_cap_turn || fabs(voltage_right) > vel_cap_turn){ //limit the voltage
               voltage_left = vel_cap_turn;
               voltage_right = vel_cap_turn;
             }

             voltage_right = error_heading * left_sign * kP_turn;
             voltage_left = error_heading * right_sign * kP_turn;


             std::cout << voltage_left << std::endl;

             frontRight.move(voltage_right); //set the motors to the intended speed
             backRight.move(voltage_right);

             frontLeft.move(voltage_left);
             backLeft.move(voltage_left);


             pros::delay(20);
           }

           reset_drive();
         }




         void simple_turn_left(int degrees_target, int tolerance, float kP_turn) {


           float degrees_current = 0;


             int left_sign;
             int right_sign;
             float voltage_left;
             float voltage_right;
             int acc = 5;


               while (  inertial.get_heading() > 360 - degrees_target + tolerance) {

                 std::cout << "sensor value: " << inertial.get_heading() << std::endl;


                 degrees_current = inertial.get_heading();
                 float error_heading = degrees_target - fabs(degrees_current - 360); // error - target - current

               std::cout << "error value: " << error_heading << std::endl;

                 int left_sign = (error_heading > 0) - (error_heading < 0);
                 int right_sign = (error_heading < 0)  - (error_heading > 0);


                 vel_cap_turn = vel_cap_turn + acc;  //slew rate
                 if(vel_cap_turn > 95){
                   vel_cap_turn = 95; //velCap cannot exceed 120
                 }

                 if(fabs(voltage_left) > vel_cap_turn || fabs(voltage_right) > vel_cap_turn){ //limit the voltage
                   voltage_left = vel_cap_turn;
                   voltage_right = vel_cap_turn;
                 }

                 voltage_right = error_heading * left_sign * kP_turn;
                 voltage_left = error_heading * right_sign * kP_turn;


                 std::cout << voltage_left << std::endl;

                 frontRight.move(voltage_right); //set the motors to the intended speed
                 backRight.move(voltage_right);

                 frontLeft.move(voltage_left);
                 backLeft.move(voltage_left);


                 pros::delay(20);
               }
               reset_drive();
             }


bool frontGoUp = false;
bool frontGoDown = false;


void front_mogo_up(void *param) {
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

void frontGrabbing(void *param) {
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


void back_mogo_up(void *param) {
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

void backGrabbing(void *param) {
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



void left_side() {


  front_piston.set_value(false);
  back_piston.set_value(false);


  pros::task_t frontUpAutonTask = pros::c::task_create(front_mogo_up, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  TASK_STACK_DEPTH_DEFAULT, "My Task");

  pros::task_t backUpAutonTask = pros::c::task_create(back_mogo_up, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  TASK_STACK_DEPTH_DEFAULT, "My Task");

  pros::task_t frontGrabbingTask = pros::c::task_create(frontGrabbing, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  TASK_STACK_DEPTH_DEFAULT, "My Task");

  pros::task_t backGrabbingTask = pros::c::task_create(backGrabbing, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  TASK_STACK_DEPTH_DEFAULT, "My Task");


  pros::Task auton_task1 (frontUpAutonTask);
  pros::Task auton_task4 (backUpAutonTask);
  pros::Task auton_task2 (frontGrabbingTask);
  pros::Task auton_task3 (backGrabbingTask);
  //
  //


  inertial.reset();

  resetAll();

  frontGoDown = true;
  chassis->driveToPoint({0_in,44_in});
  chassis->waitUntilSettled();

  chassis->driveToPoint({0_in,35_in}, true);
  chassis->waitUntilSettled();

  holdOnFront = false;

  chassis->driveToPoint({0_in,15_in}, true);
  chassis->waitUntilSettled();

  QAngle inertialPosition = inertial.get_heading() * degree;
  chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});

  chassis->turnToPoint({-29_in, 50_in});
  chassis->waitUntilSettled();
  holdOnFront = true;
  chassis->driveToPoint({-29_in, 50_in});
  chassis->waitUntilSettled();

  frontGoUp = true;
  backGoDown = true;

  inertialPosition = inertial.get_heading() * degree;
  chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});

  chassis->driveToPoint({0_in, 20_in}, true);
  chassis->waitUntilSettled();


  inertialPosition = inertial.get_heading() * degree;
  chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});

  chassis->turnToAngle(270_deg);

  resetAll();
  chassis->driveToPoint({0_in, -12_in}, true);
  chassis->waitUntilSettled();
  backGoDown = false;
  backGoUp = true;

  conveyor.move_velocity(600);
  frontGoDown = true;

}

void right_side() {

}



void testingAuton() {

  pros::task_t frontUpAutonTask = pros::c::task_create(front_mogo_up, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  TASK_STACK_DEPTH_DEFAULT, "My Task");

  pros::task_t backUpAutonTask = pros::c::task_create(back_mogo_up, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  TASK_STACK_DEPTH_DEFAULT, "My Task");

  pros::task_t frontGrabbingTask = pros::c::task_create(frontGrabbing, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  TASK_STACK_DEPTH_DEFAULT, "My Task");

  pros::task_t backGrabbingTask = pros::c::task_create(backGrabbing, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  TASK_STACK_DEPTH_DEFAULT, "My Task");


  pros::Task auton_task1 (frontUpAutonTask);
  pros::Task auton_task4 (backUpAutonTask);
  pros::Task auton_task2 (frontGrabbingTask);
  pros::Task auton_task3 (backGrabbingTask);



  resetAll();






    resetAll();
    chassis->setState({0_ft,0_ft, 180_deg});

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



  inertialPosition = inertial.get_heading() * degree;
  chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});
  chassis->turnToAngle(0_deg);
  chassis->waitUntilSettled();
  inertialPosition = inertial.get_heading() * degree;
  chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});
  chassis->turnToAngle(0_deg);
  chassis->waitUntilSettled();


  profileControllerFast->setTarget("F", false, false);
  profileControllerFast->waitUntilSettled();

  frontGoUp = false;
  frontGoDown = false;

  armLeft.move_relative(-750, 200);
  armRight.move_relative(-750, 200);

  profileControllerSlow->generatePath({{0_ft, 0_ft, 0_deg}, {1.75_ft, 0_ft, 0_deg}}, "G");
  profileControllerSlow->setTarget("G", false, false);
  profileControllerSlow->waitUntilSettled();

  holdOnFront = false;
  pros::delay(500);


  frontGoUp = true;

  profileControllerSlow->setTarget("G", true, false);
  profileControllerSlow->waitUntilSettled();

  chassis->driveToPoint({0_ft, 0_ft});
  chassis->waitUntilSettled();
  inertialPosition = inertial.get_heading() * degree;
  chassis->setState({(chassis->getState().x), (chassis->getState().y), inertialPosition});
  chassis->turnToAngle(0_deg);




}


























void skills_260() {
  // front_piston.set_value(false);
  // back_piston.set_value(false);
  //
  //
  // pros::task_t frontUpAutonTask = pros::c::task_create(front_mogo_up, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  // TASK_STACK_DEPTH_DEFAULT, "My Task");
  //
  // pros::task_t backUpAutonTask = pros::c::task_create(back_mogo_up, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  // TASK_STACK_DEPTH_DEFAULT, "My Task");
  //
  // pros::task_t frontGrabbingTask = pros::c::task_create(frontGrabbing, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  // TASK_STACK_DEPTH_DEFAULT, "My Task");
  //
  // pros::task_t backGrabbingTask = pros::c::task_create(backGrabbing, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  // TASK_STACK_DEPTH_DEFAULT, "My Task");
  //
  //
  // pros::Task auton_task1 (frontUpAutonTask);
  // pros::Task auton_task4 (backUpAutonTask);
  // pros::Task auton_task2 (frontGrabbingTask);
  // pros::Task auton_task3 (backGrabbingTask);
  //
  //
  //
  // chassis->setState({0_ft,0_ft, 180_deg});
  // backGoDown = true;
  // chassis->driveToPoint({1.75_ft, 0_ft}, true);
  // chassis->waitUntilSettled();
  // backGoDown = false;
  // backGoUp = true;
  //
  //
  // profileControllerSlow->generatePath({{0_ft, 0_ft, 180_deg}, {1_ft, 1_ft, 90_deg}}, "B");
  // profileControllerSlow->setTarget("B", false, true);
  // profileControllerSlow->waitUntilSettled();
  //
  // frontGoDown = true;
  //
  //
  //
  //   chassis->turnToPoint({3_ft, 1.5_ft});
  //   chassis->waitUntilSettled();
  //
  //
  // chassis->setState({0_ft,0_ft,0_deg});
  // chassis->driveToPoint({3_ft,1.5_ft});
  // chassis->waitUntilSettled();
  //
  // frontGoDown = false;
  // frontGoUp = true;
  //
  //
  // armLeft.move_relative(-950, 200);
  // armRight.move_relative(-950, 200);
  //
  // pros::delay(500);
  //
  // holdOnFront = false;


  // chassis->turnToAngle(270_deg);
  // chassis->waitUntilSettled();





  // pros::lcd::set_text(6, chassis->getState().str());





}


void skills_290() {

}
