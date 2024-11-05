#include "main.h"

#include "robot/all.h"

using namespace Robot;
using namespace Robot::Globals;

struct RobotSubsystems {
  Robot::Drivetrain drivetrain;
} subsystem;

struct RobotScreen {

} screen;

void initialize() {
  pros::lcd::initialize();
  chassis.calibrate();
  chassis.setPose(0,0,0);

  // TODO: init selector screen
}

void competition_initialize() {}

void autonomous() {
  // TODO: autonomous
}

void opcontrol() {
  while (true) {
    
  } 
}