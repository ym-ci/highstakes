#include "robot/auton.h"
#include "globals.h"
#include "main.h" // IWYU pragma: export

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
void Autonomous::auton1(Intake &intake, Latch &latch) {
   genericLeft(intake, latch);
}

// Red Right
void Autonomous::auton2(Intake &intake, Latch &latch) {
   genericRight(intake, latch);
}

// Blue Left
void Autonomous::auton3(Intake &intake, Latch &latch) {
   genericLeft(intake, latch);
}

// Blue Right
void Autonomous::auton4(Intake &intake, Latch &latch) {
   genericRight(intake, latch);
}

void Autonomous::auton5(Intake &intake, Latch &latch) {
   // Autonomous routine for the Skills challenge
}

// Takes in two parameters: The autonomous value as well as the puncher object.
void Autonomous::autoDrive(Intake &intake, Latch &latch) {
   // Keep the switcher running while the controller down button has not been pressed and the time period is not
   // autonomous Compare the current auton value to run the auton routine
   switch (Autonomous::auton) {
   case RED_LEFT:
      auton1(intake, latch);
      break;
   case RED_RIGHT:
      auton2(intake, latch);
      break;
   case BLUE_LEFT:
      auton3(intake, latch);
      break;
   case BLUE_RIGHT:
      auton4(intake, latch);
      break;
   case SKILLS:
      auton5(intake, latch);
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