// GENERAL NOTES FOR THINGS


//FRONT LIFT
// start from starting lift position
//top to bottom
// 	liftController_front->setTarget(-490);
// liftController_front->waitUntilSettled();

// bottom to top
// 	liftController_front->setTarget(0);

// from starting position to scoring position
// liftController_front->setTarget(-185);
// liftController_front->waitUntilSettled();




// // BACK LIFT
// // from starting to down position
// liftController_back->setTarget(-445);



/// WORKING ODOM + PROFILE MOTIONING
//

//
// chassis->setState({0_ft,0_ft,0_deg});
// profileController->generatePath({{0_ft, 0_ft, 0_deg},{2_ft,1_ft,0_deg}}, "A");
//
//
// profileController->setTarget("A");
// profileController->waitUntilSettled();
//
//
// // chassis->driveToPoint({0_ft,0_ft}, true);
// // chassis->waitUntilSettled();
//
//
// chassis->turnToAngle(0_deg);
// chassis->waitUntilSettled();
//
// chassis->driveToPoint({2_ft,-1_ft});
// chassis->waitUntilSettled();











  // 
  //
  //
  // resetAll();
  // drivePIDFn(-1350, -1350, 100, 100, .38);
  // reset_drive();
  //
  // pros::delay(1000);
  //
  //
  // drivePIDFn(850, 850, 100, 100, .38);
  // reset_drive();
  //
  //   backGoUp = false;
  //   backGoDown = true;
  //
  //   holdOnBack = false;
  //
  //   frontGoUp = false;
  //
  //     armLeft.move_relative(-725, 200);
  //     armRight.move_relative(-725, 200);
  //
  //   profileControllerFast->generatePath({{0_ft, 0_ft, 0_deg}, {1.5_ft, 0_ft, 0_deg}}, "D");
  //   profileControllerFast->setTarget("D", false, false);
  //   profileControllerFast->waitUntilSettled();
  //
  //   armLeft.move_relative(-800, 200);
  //   armRight.move_relative(-800, 200);
  //
  //   pros::delay(500);
  //
  //   holdOnFront = false;
  //
  //   pros::delay(500);
  //
  //   drivePIDFn(-850, -850, 50, 100, .1);
  //   reset_drive();
  //
  //   drivePIDFn(-800, 800, 25, 100, .3);
  //   reset_drive();
