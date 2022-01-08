#include "main.h"

using namespace okapi;



  // odometry chassis
  extern std::shared_ptr<OdomChassisController> chassis;


  // 2d motion profiling fast and slow
  extern std::shared_ptr<AsyncMotionProfileController> profileControllerFast;
	extern	std::shared_ptr<AsyncMotionProfileController> profileControllerSlow;

  // chassis for the fast and slow motion profilers
  extern std::shared_ptr<ChassisController> myChassis;
