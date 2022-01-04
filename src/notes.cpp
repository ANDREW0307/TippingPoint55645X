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
