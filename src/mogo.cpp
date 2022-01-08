#include "main.h"

int back_mogo_velocity = 0;

void backMogoControl(void *param) {


	while(true) {


		bool isPressedBackMogo = master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2);


	// 1 for up, -1 for down
		int modeTest = bumper_back_up.get_value() - bumper_back_down.get_value();


			while (isPressedBackMogo && bumper_back_up.get_value() && modeTest == 1) {
				pros::lcd::set_text(2, "bad PROS User!");
				if(!bumper_back_down.get_value()) {
					back_mogo_velocity = 200;
				}
			}



			while (isPressedBackMogo && bumper_back_down.get_value() && modeTest == -1) {
				pros::lcd::set_text(3, "bad PROS User!");
				if(!bumper_back_up.get_value()) {
					back_mogo_velocity = -200;
				}
			}

			std::cout << modeTest << std::endl;
			if (bumper_back_up.get_value() && back_mogo.get_voltage() < -1000) {
				pros::lcd::set_text(6, "WNRGOWEOGLEGKNWE");
				back_mogo_velocity = 0;
			}

			if (bumper_back_down.get_value() && back_mogo.get_voltage() > 1000) {
				pros::lcd::set_text(7, "WNRGOWEOGLEGKNWE");
				back_mogo_velocity = 0;
			}

			pros::delay(20);

		}

}





void front_mogo_up(void *param) {

	while (true) {
		if (!bumper_front_up.get_value() && frontGoUp) {
			armLeft.move_velocity(200);
			armRight.move_velocity(200);
		} else {
			armLeft.move_velocity(0);
			armRight.move_velocity(0);
		}
		pros::delay(80);
	}

}

void front_mogo_down(void *param) {
	while (true) {
		if (!bumper_front_down.get_value() && frontGoDown) {
			armLeft.move_velocity(-200);
			armRight.move_velocity(-200);
		} else {
			armLeft.move_velocity(0);
			armRight.move_velocity(0);
		}
		pros::delay(80);
	}
}
