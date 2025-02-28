#include "robot/intake.h"

#include "globals.h"

#include "pros/vision.h"
#include "util/velocityPID.h"
#include "robot/auton.h"

#define SLOWER_VELOCITY 425
#define FASTER_VELOCITY 800
using namespace Robot;
using namespace Robot::Globals;

Intake::Intake() : elevated(false), stopPosition(0), lastWrongRingTime(0) {
}


void Intake::run() {
   bool fwd = controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
   bool rev = controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);

   // Check if we're still in cooldown period (0.5 seconds = 500 milliseconds)
   uint32_t currentTime = pros::millis();
   if (currentTime - lastWrongRingTime > 500) {
      pros::vision_object_s_t obj = colorSensor.get_by_size(0);
      if (obj.width > 25) {
         bool isRed = obj.signature == 1;
         bool weAreRed = Autonomous::auton == Autonomous::RED_LEFT || Autonomous::auton == Autonomous::RED_RIGHT;
         std::cout << "Object detected: " << obj.signature << std::endl;
         if (isRed) {
            std::cout << "Red ring!!" << std::endl;
            // red rgb
            int32_t rgb = 0xFF0000;
            colorSensor.set_led(rgb);
         } else {
            std::cout << "Blue ring!!" << std::endl;
            // blue rgb
            int32_t rgb = 0x0000FF;
            colorSensor.set_led(rgb);
         }
         bool wrongRing = (isRed && !weAreRed) || (!isRed && weAreRed);
         if (wrongRing) {
            // Set stop position to current position + 25 when wrong ring detected
            stopPosition = conveyorMotor.get_position() + 25;
            lastWrongRingTime = currentTime;  // Start cooldown timer
            std::cout << "Wrong ring detected - setting stop position to: " << stopPosition << std::endl;
            return; // Exit the function to prevent further motor commands
         } else {
            // Increment stop position by 100 when correct ring or no ring detected
            stopPosition = conveyorMotor.get_position() + 100;
         }
      } else {
         // No object detected, increment stop position by 100
         stopPosition = conveyorMotor.get_position() + 100;
      }
   }

   if (stopPosition != 0) {
      // if we are at stop position (within an error margin), brake
      double currentPos = conveyorMotor.get_position();
      double delta = stopPosition - currentPos;
      std::cout << "delta: " << delta << " stopPosition: " << stopPosition << " currentPos: " << currentPos << std::endl;
      if (delta < 10) {
         conveyorMotor.brake();
         std::cout << "Stop position reached - setting stop position to 0" << std::endl;
         stopPosition = 0;
         // delay the next wrong ring detection by 30ms
         lastWrongRingTime = currentTime + 500;
         return;
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