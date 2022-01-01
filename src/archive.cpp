#include "main.h"


// void reset_drive(){
//
//
//
//   //wwrognwornowngoweg
//
//   int voltageLeft, voltageRight = 0;
//
//   frontRight = voltageRight;
//   backRight = voltageRight;
//
//   frontLeft = voltageLeft;
//   backLeft = voltageLeft;
//
// }
//
//
// int velCap;
// void drivePIDFn(int targetLeft, int targetRight, int tolerance, float kp){
//   leftEncoder.reset();
//   rightEncoder.reset();
//   int errorLeft = targetLeft;
//   int errorRight = targetRight;
//   float kpTurn = 0.1;
//   int acc = 5;
//   int voltageLeft = 0;
//   int voltageRight = 0;
//   int signLeft;
//   int signRight;
//
//   while(abs(errorLeft) > abs(tolerance) || abs(errorRight) > abs(tolerance)){
//     errorLeft = targetLeft - leftEncoder.get_value(); //error is target minus actual value
//     errorRight = targetRight - rightEncoder.get_value();
//
//     signLeft = (errorLeft > 0) - (errorLeft < 0); // + or -
//     signRight = (errorRight > 0) - (errorRight < 0);
//
//     pros::lcd::print(4, "%d", leftEncoder.get_value());
//     pros::lcd::print(5, "%d", rightEncoder.get_value());
//
//
//
//     if(signLeft == signRight){
//       voltageLeft = errorLeft * kp; //intended voltage is error times constant
//       voltageRight = errorRight * kp;
//     }
//     else {
//       voltageLeft = errorLeft * kpTurn; //same logic with different turn value
//       voltageRight = errorRight * kpTurn;
//     }
//
//     velCap = velCap + acc;  //slew rate
//     if(velCap > 95){
//       velCap = 95; //velCap cannot exceed 120
//     }
//     if(abs(voltageLeft) > velCap){ //limit the voltage
//       voltageLeft = velCap * signLeft;
//     }
//     if(abs(voltageRight) > velCap){ //ditto
//       voltageRight = velCap * signRight;
//     }
//     //set the motors to the intended speed
//     frontRight = voltageRight;
//     backRight = voltageRight;
//
//     frontLeft = voltageLeft;
//     backLeft = voltageLeft;
//
//     pros::delay(20);
//   }
//
// }





//
// chassis->setState({0_ft,0_ft,0_deg});
// profileController->generatePath({{0_ft, 0_ft, 0_deg},{2_ft,1_ft,0_deg}}, "A");
//
//
// profileController->setTarget("A");
// profileController->waitUntilSettled();
//
// chassis->driveToPoint({0_ft,0_ft}, true);
// chassis->turnToAngle(0_deg);
//

//
// while(false) {
// 	chassis->setState({0_m, 0_m, 0_deg});
//
// 	std::cout << "" << chassis->getState().str() << std::endl;
//
//
//
// 	profileController->generatePath({{0_m, 0_m, 0_deg},{0.35_m,0.35_m,35_deg}}, "A");
//
// 	profileController->setTarget("A");
// 	profileController->waitUntilSettled();
//
// 				std::cout << "" << chassis->getState().str() << std::endl;
// }



//
// while (true) {
// 			 	std::cout << chassis->getState().str() << std::endl;
// 				pros::delay(100);
// }
//
// 			// std::cout << 1 << std::endl;
// 			pros::delay(100);
// }



// pros::lcd::print(5, "%d", leftEncoder.get_value());
// pros::lcd::print(6, "%d", rightEncoder.get_value());
//
//
// pros::lcd::print(0, "%s",  chassis->getState().str());
//
// std::cout << leftEncoder.get_value() << std::endl;
// std::cout << chassis->getState().str() << std::endl;
