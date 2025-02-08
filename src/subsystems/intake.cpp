#include "robot/intake.h"

#include "globals.h"

#include "util/velocityPID.h"

#define SLOWER_VELOCITY 425
#define FASTER_VELOCITY 800
using namespace Robot;
using namespace Robot::Globals;

Intake::Intake() : elevated(false) {
}

void Intake::run() {
   bool conveyorFwd = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
   bool conveyorRev = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
   bool intakeFwd = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) || conveyorFwd;
   bool intakeRev = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2) || conveyorRev;

   if (intakeFwd) {
      intakeMotor.move_velocity(-SLOWER_VELOCITY);
   } else if (intakeRev) {
      intakeMotor.move_velocity(SLOWER_VELOCITY);
   } else {
      intakeMotor.brake();
   }

   if (conveyorFwd) {
      conveyorMotor.move_velocity(FASTER_VELOCITY);
   } else if (conveyorRev) {
      conveyorMotor.move_velocity(-FASTER_VELOCITY);
   } else {
      conveyorMotor.brake();
   }
}

void Intake::moveConveyor(int velocity) {
   conveyorMotor.move_velocity(velocity);
}

void Intake::moveIntake(int velocity) {
   intakeMotor.move_velocity(velocity);
}

void Intake::stopIntake() {
   intakeMotor.move_velocity(0);
}

void Intake::stopConveyor() {
   conveyorMotor.move_velocity(0);
}

void Intake::moveAll(int intakeVelocity, int conveyorVelocity) {
   moveIntake(intakeVelocity);
   moveConveyor(conveyorVelocity);
}

void Intake::moveAll(){
   moveIntake(-SLOWER_VELOCITY);
   moveConveyor(FASTER_VELOCITY);
}

void Intake::reverseAll(){
   moveIntake(SLOWER_VELOCITY);
   moveConveyor(-FASTER_VELOCITY);
}

void Intake::cycle() {
   pros::delay(750);
   reverseAll();
   pros::delay(200);
   moveAll();
}

void Intake::stopAll() {
   stopIntake();
   stopConveyor();
}

void Intake::toggle() { elevated = !elevated; }