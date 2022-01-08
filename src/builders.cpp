#include "main.h"

using namespace okapi;


  std::shared_ptr<OdomChassisController> chassis =
    ChassisControllerBuilder()
      .withMotors({20,19}, {9,17})
      .withDimensions({AbstractMotor::gearset::blue, 7.0/3.0}, {{4_in, 14.625_in}, imev5BlueTPR})
  		.withMaxVelocity(200)
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
