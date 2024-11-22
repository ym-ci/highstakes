#include "robot/intake.h"

#include "globals.h"

#include "util/velocityPID.h"

#define SLOWER_VELOCITY 177
#define FASTER_VELOCITY 600

using namespace Robot;
using namespace Robot::Globals;

VelocityPID intakePID(0.01, 0.0, 0.007, 0.0, false, 17.5);

Intake::Intake() {
   elevated = false;
   alliance_color = false;
}

void Intake::run() {

   double currentVelocity = intakeMotor.get_actual_velocity();
   if (currentVelocity != 0) {
      std::cout << " Current Velocity: " << currentVelocity << std::endl;
   }
   if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      // intakeMotor.move_velocity(-FASTER_VELOCITY);
      float val = intakePID.update(-SLOWER_VELOCITY, currentVelocity) * 1000;
      // printf("VAL: %f\n", val);
      // std::cout << " VAL: " << val;
      intakeMotor.move_voltage(val);
   } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      // intakeMotor.move_velocity(FASTER_VELOCITY);
      intakeMotor.move_voltage(intakePID.update(SLOWER_VELOCITY, currentVelocity) * 1000);
   } else {
      intakeMotor.brake();
      // HookMotor.brake();
   }
}

void Intake::toggle() { elevated = !elevated; }

// Vision sensor only works with intake, therefore it should not on
void Intake::checkStop() {}
