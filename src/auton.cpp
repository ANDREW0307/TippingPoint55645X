#include "main.h"

using namespace okapi;


#include "main.h"

using namespace okapi;


  std::shared_ptr<OdomChassisController> chassis =
    ChassisControllerBuilder()
      .withMotors({20,19}, {9,17})
      .withDimensions({AbstractMotor::gearset::blue, 7.0/3.0}, {{4_in, 14.625_in}, imev5BlueTPR})
  		.withMaxVelocity(500)
      .withSensors(ADIEncoder{'A', 'B'}, ADIEncoder{{6, 'A','B'}, true}, {ADIEncoder{'G', 'H'}})
      .withOdometry({{2.75_in, 14.75_in , 3_in, 2.75_in}, quadEncoderTPR}, StateMode::FRAME_TRANSFORMATION)
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

bool holdOnBack = true;

void backGrabbing(void *param) {
  while (true) {
    if (lineTracker_back.get_value() < 2000 && holdOnBack) {
      back_piston.set_value(true);
    } else {
      back_piston.set_value(false);
    }
    pros::delay(100);
  }
}


void left_side() {


  // front_piston.set_value(false);
  // back_piston.set_value(false);
  //
  //
  // pros::task_t frontUpAutonTask = pros::c::task_create(front_mogo_up, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  // TASK_STACK_DEPTH_DEFAULT, "My Task");
  //
  // pros::task_t frontGrabbingTask = pros::c::task_create(frontGrabbing, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  // TASK_STACK_DEPTH_DEFAULT, "My Task");

  // pros::task_t backGrabbingTask = pros::c::task_create(backGrabbing, (void*)"PROS", TASK_PRIORITY_DEFAULT,
  // TASK_STACK_DEPTH_DEFAULT, "My Task");
  //
  //
  // pros::Task auton_task1 (frontUpAutonTask);
  // pros::Task auton_task2 (frontGrabbingTask);
  // pros::Task auton_task3 (backGrabbingTask);
  //
  //
  // chassis->setState({0_ft,0_ft, 0_deg});
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

}


void skills_290() {

}
