#include "robot/auton.h"
#include "globals.h"
#include "main.h" // IWYU pragma: export
#include "robot/intake.h"
#include "robot/latch.h"

// https://github.com/calhighrobotics/over_under_b/blob/main/src/routines/auton.cpp#L190

using namespace Robot;
using namespace Robot::Globals;

Autonomous::AUTON_ROUTINE Autonomous::auton = RED_LEFT;
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
   chassis.setPose(0,0,300); // start
   // Grab MoGo
   chassis.moveToPose(13, -7.25, 300, 1000, {.forwards = false}, false);
   latch.extend();
   pros::delay(100);   
   intake.moveAll();
   intake.cycle();
   // Grab Second Ring
   chassis.turnToHeading(180, 1000);
   chassis.moveToPoint(14, -27, 1000, {}, false);
   intake.cycle();
   // Grab Third Ring
   chassis.moveToPoint(14, -40, 1000, {}, false);
   intake.cycle();
   pros::delay(500);
   // Fourth Ring
   chassis.moveToPoint(11.5, -16.7, 1000, {.forwards=false}, false);
   chassis.turnToHeading(215, 1000);
   chassis.moveToPoint(2.2, -29.5, 2000, {}, false);
   pros::delay(500);
   intake.cycle();
   // go to corner
   chassis.moveToPoint(11.5, -16.7, 1000, {.forwards=false}, false);
   latch.retract();
   chassis.moveToPose(-2, -42, 30, 4000, {.forwards=false}, false);
   // latch.retract();
   // pros::delay(10);
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