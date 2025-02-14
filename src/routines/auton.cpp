#include "robot/auton.h"
#include "globals.h"
#include "main.h" // IWYU pragma: export
#include "robot/intake.h"
#include "robot/latch.h"
#include "robot/doinker.h"

// https://github.com/calhighrobotics/over_under_b/blob/main/src/routines/auton.cpp#L190

using namespace Robot;
using namespace Robot::Globals;

Autonomous::AUTON_ROUTINE Autonomous::auton = SKILLS;
std::string Autonomous::autonName;

// -150, 60 -> -60, 0 
// Delta = (90, 60)
void Autonomous::genericLeft(Intake &intake, Latch &latch) {
   chassis.setPose(0,0,0); // start
   chassis.moveToPoint(90, 60, 1000);
}

void Autonomous::genericRight(Intake &intake, Latch &latch) {
   chassis.setPose(0,0,0); // start
   chassis.moveToPoint(-90, 60, 1000);
}

// Red Left
void Autonomous::redLeftAuton(Intake &intake, Latch &latch) {
   genericLeft(intake, latch);
}

// Red Right
void Autonomous::redRightAuton(Intake &intake, Latch &latch) {
   genericRight(intake, latch);
}

// Blue Left
void Autonomous::blueLeftAuton(Intake &intake, Latch &latch) {
   genericLeft(intake, latch);
}

// Blue Right
void Autonomous::blueRightAuton(Intake &intake, Latch &latch) {
   genericRight(intake, latch);
}

void Autonomous::skillsAuton(Intake &intake, Latch &latch) {
   // Autonomous routine for the Skills challenge
   // takes 30 seconds
   chassis.setPose(0,0,300); // start
   // Grab MoGo
   chassis.moveToPose(14, -7.52, 300, 1000, {.forwards = false}, false);
   latch.extend();
   pros::delay(100);   
   intake.moveAll();
   intake.cycle();
   // // Grab Second Ring
   chassis.turnToHeading(180, 1000);
   chassis.moveToPoint(13, -27, 1000, {}, false);
   intake.cycle();
   // // Grab Third Ring
   chassis.moveToPoint(13, -37, 1000, {}, false);
   intake.cycle();
   // // Fourth Ring
   chassis.moveToPoint(15, -12, 1000, {.forwards=false}, false);
   chassis.turnToHeading(230
      , 1000);
   chassis.moveToPoint(1.25, -28.2, 2000, {}, false);
   intake.cycle();
   // Fifth Ring
   chassis.moveToPose(14, -20, 90, 2000, {.forwards=false}, false);
   chassis.moveToPoint(32.47, -30.89, 2000, {}, false);
   intake.longCycle();
   // Sixth Ring
   chassis.moveToPose(14, -20, 90, 2000, {.forwards=false}, false);
   chassis.moveToPoint(32.6, -7.66, 2000, {}, false);
   intake.longCycle();


   // // go to corner
   chassis.moveToPoint(-0.9, -39, 10000 , {.forwards=false}, true);
   intake.longCycle();
   latch.retract();
   pros::delay(300);
   chassis.waitUntilDone();
   chassis.moveToPoint(11.5, -16.7, 1000, {}, false);
   pros::delay(10000);

}

// Takes in two parameters: The autonomous value as well as the puncher object.
void Autonomous::autoDrive(Intake &intake, Latch &latch) {
   // Keep the switcher running while the controller down button has not been pressed and the time period is not
   // autonomous Compare the current auton value to run the auton routine
   switch (Autonomous::auton) {
   case RED_LEFT:
      redLeftAuton(intake, latch);
      break;
   case RED_RIGHT:
      redRightAuton(intake, latch);
      break;
   case BLUE_LEFT:
      blueLeftAuton(intake, latch);
      break;
   case BLUE_RIGHT:
      blueRightAuton(intake, latch);
      break;
   case SKILLS:
      skillsAuton(intake, latch);
      break;
   }
}

void Autonomous::autonSwitcher(int autonNum) {
   switch (autonNum) {
   case 1:
      Autonomous::autonName = "Red Left";
      Autonomous::auton = RED_LEFT;
      break;
   case 2:
      Autonomous::autonName = "Red Right";
      Autonomous::auton = RED_RIGHT;
      break;
   case -1:
      Autonomous::autonName = "Blue Left";
      Autonomous::auton = BLUE_LEFT;
      break;
   case -2:
      Autonomous::autonName = "Blue Right";
      Autonomous::auton = BLUE_RIGHT;
      break;
   case 0:
      Autonomous::autonName = "Skills";
      Autonomous::auton = SKILLS;
   }
   std::cout << "Current auton: " + Autonomous::autonName << std::endl;
}