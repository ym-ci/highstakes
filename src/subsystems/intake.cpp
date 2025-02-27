#include "robot/intake.h"

#include "globals.h"

#include "pros/vision.h"
#include "util/velocityPID.h"
#include "robot/auton.h"

#define SLOWER_VELOCITY 425
#define FASTER_VELOCITY 800
using namespace Robot;
using namespace Robot::Globals;

Intake::Intake() : elevated(false) {
}

void Intake::run() {
   bool fwd = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
   bool rev = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);

   pros::vision_object_s_t obj = colorSensor.get_by_size(0);
   if (obj.width > 100 && false) {
      std::cout << "Object detected: " << obj.signature << std::endl;
      bool isRed = obj.signature == 1;
      bool weAreRed = Autonomous::auton == Autonomous::RED_LEFT || Autonomous::auton == Autonomous::RED_RIGHT;
      
      bool wrongRing = (isRed && !weAreRed) || (!isRed && weAreRed);
      if (wrongRing) {
         // TODO: reject ring
      }
   }

   if (rev) {
      intakeMotor.move_velocity(SLOWER_VELOCITY);
   } else if (fwd) {
      intakeMotor.move_velocity(-SLOWER_VELOCITY);
   } else {
      intakeMotor.brake();
   }

   if (rev) {
      conveyorMotor.move_velocity(-FASTER_VELOCITY);
   } else if (fwd) {
      conveyorMotor.move_velocity(FASTER_VELOCITY);
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
   // moveIntake(SLOWER_VELOCITY);
   moveConveyor(-FASTER_VELOCITY);
}

void Intake::cycle() {
   pros::delay(400);
   reverseAll();
   pros::delay(250);
   moveAll();
}

void Intake::longCycle() {
   pros::delay(400);
   reverseAll();
   pros::delay(400);
   moveAll();
}

void Intake::stopAll() {
   stopIntake();
   stopConveyor();
}

void Intake::toggle() { elevated = !elevated; }