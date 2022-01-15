#include "main.h"

using namespace okapi;


#include "main.h"

using namespace okapi;


  std::shared_ptr<OdomChassisController> chassis =
    ChassisControllerBuilder()
      .withMotors({20,19}, {9,17})
      .withGains(
        {0.0017, 0, 0.0000025}, // distance controller gains
        {0.0055, 0, 0.000035}, // turn controller gains
        {0.0000, 0, 0.000}  // angle controller gains (helps drive straight)
      )
      .withDimensions({AbstractMotor::gearset::blue, 7.0/3.0}, {{4_in, 14.625_in}, imev5BlueTPR})
  		.withMaxVelocity(300)
      .withSensors(ADIEncoder{'A', 'B'}, ADIEncoder{{6, 'A','B'}, true}, {ADIEncoder{'G', 'H'}})
      .withOdometry({{2.75_in, 8.75_in , 2_in, 2.75_in}, quadEncoderTPR}, StateMode::FRAME_TRANSFORMATION)
      .buildOdometry();


	std::shared_ptr<ChassisController> myChassis =
		 ChassisControllerBuilder()
		  .withMotors({20,19}, {9,17})
	    .withDimensions({AbstractMotor::gearset::blue, 7.0/3.0}, {{4_in, 14.625_in}, imev5BlueTPR})
			.withSensors(ADIEncoder{'A', 'B'}, ADIEncoder{{6, 'A','B'}, true}, {ADIEncoder{'G', 'H'}})
      .build();

   std::shared_ptr<AsyncMotionProfileController> profileControllerFast =
     AsyncMotionProfileControllerBuilder()
       .withLimits({
         .75, // Maximum linear velocity of the Chassis in m/s
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



bool frontGoUp = false;
bool frontGoDown = false;


void front_mogo_up(void *param) {
  while (true) {
    while (frontGoUp) {
      if (!bumper_front_up.get_value()) {
        armLeft.move_velocity(200);
        armRight.move_velocity(200);
      } else {
        armLeft.move_velocity(0);
        armRight.move_velocity(0);
        frontGoUp = false;
      }
      pros::delay(80);
    }
    while (frontGoDown) {
      if (!bumper_front_down.get_value()) {
        armLeft.move_velocity(-200);
        armRight.move_velocity(-200);
      } else {
        armLeft.move_velocity(0);
        armRight.move_velocity(0);
        frontGoDown = false;
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
      } else {
        back_mogo.move_velocity(0);

        backGoUp = false;
      }
      pros::delay(80);
    }
    while (backGoDown) {
      if (!bumper_back_down.get_value()) {
        back_mogo.move_velocity(200);

      } else {
        back_mogo.move_velocity(0);

        backGoDown = false;
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


void left_side() {


  front_piston.set_value(false);
  back_piston.set_value(false);


  pros::task_t frontUpAutonTask = pros::c::task_create(front_mogo_up, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  TASK_STACK_DEPTH_DEFAULT, "My Task");

  pros::task_t frontGrabbingTask = pros::c::task_create(frontGrabbing, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  TASK_STACK_DEPTH_DEFAULT, "My Task");

  pros::task_t backGrabbingTask = pros::c::task_create(backGrabbing, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  TASK_STACK_DEPTH_DEFAULT, "My Task");


  pros::Task auton_task1 (frontUpAutonTask);

  pros::Task auton_task2 (frontGrabbingTask);
  pros::Task auton_task3 (backGrabbingTask);
  //
  //
  chassis->setState({0_ft,0_ft, 0_deg});
  // frontGoDown = true;
  //
  // profileControllerFast->generatePath({{0_ft, 0_ft, 0_deg}, {45_in, 0_in, 0_deg}}, "A");
  // profileControllerFast->setTarget("A");
  // profileControllerFast->waitUntilSettled();
  // frontGoDown = false;
  // frontGoUp = true;

  profileControllerFast->generatePath({{0_ft, 0_ft, 0_deg}, {0.5_ft, 3.5_ft, 0_deg}}, "B");
  profileControllerFast->setTarget("B", true, false);
  profileControllerFast->waitUntilSettled();







}

void right_side() {

}


void skills_260() {
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



  chassis->setState({0_ft,0_ft, 180_deg});
  backGoDown = true;
  chassis->driveToPoint({1.75_ft, 0_ft}, true);
  chassis->waitUntilSettled();
  backGoDown = false;
  backGoUp = true;


  profileControllerSlow->generatePath({{0_ft, 0_ft, 180_deg}, {1_ft, 1_ft, 90_deg}}, "B");
  profileControllerSlow->setTarget("B", false, true);
  profileControllerSlow->waitUntilSettled();

  frontGoDown = true;

  // profileControllerFast->generatePath({{0_ft, 0_ft, 0_deg}, {3.55_ft, 1.75_ft, 0_deg}}, "C");
  // profileControllerFast->setTarget("C", false, true);
  // profileControllerFast->waitUntilSettled();


    chassis->turnToPoint({3_ft, 1.5_ft});
    chassis->waitUntilSettled();


  chassis->setState({0_ft,0_ft,0_deg});
  chassis->driveToPoint({3_ft,1.5_ft});
  chassis->waitUntilSettled();

  frontGoDown = false;
  frontGoUp = true;



  // chassis->turnToAngle(270_deg);
  // chassis->waitUntilSettled();





  // pros::lcd::set_text(6, chassis->getState().str());





}


void skills_290() {

}
