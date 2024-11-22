#include "robot/intake.h"

#include "globals.h"

#include "util/velocityPID.h"

#define SLOWER_VELOCITY 175
#define FASTER_VELOCITY 600

using namespace Robot;
using namespace Robot::Globals;

VelocityPID intakePID(0.05, 0.0, 0.0, 0.0, false, 18.5);

Intake::Intake() {
   elevated = false;
   alliance_color = false;
}

void Intake::run() {

   double currentVelocity = intakeMotor.get_actual_velocity();

   if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
      // intakeMotor.move_velocity(-FASTER_VELOCITY);
      intakeMotor.move_velocity(intakePID.update(-SLOWER_VELOCITY, currentVelocity));
   } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
      // intakeMotor.move_velocity(FASTER_VELOCITY);
      intakeMotor.move_velocity(intakePID.update(FASTER_VELOCITY, currentVelocity));
   } else {
      intakeMotor.brake();
      // HookMotor.brake();
   }
}

void Intake::toggle() { elevated = !elevated; }

// Vision sensor only works with intake, therefore it should not on
void Intake::checkStop() {}
