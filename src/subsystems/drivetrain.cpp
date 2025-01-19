#include "robot/drivetrain.h"
#include "globals.h"

#define DEFAULT_DELAY_LENGTH 15

using namespace Robot;
using namespace Robot::Globals;

Drivetrain::DRIVE_MODE Drivetrain::driveMode = ARCADE_DRIVE;

bool Drivetrain::isReversed = false;

Drivetrain::Drivetrain() { Drivetrain::driveMode = ARCADE_DRIVE; }

void Drivetrain::curveDrive() {
   int throttle = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
   int theta = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X);

   chassis.curvature(thrustHandler(throttle), thrustHandler(theta));

   pros::delay(DEFAULT_DELAY_LENGTH);
}

void Drivetrain::arcadeDrive() {
   // Arcade Measurements
   int throttle = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
   int theta = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) * 0.80;

   chassis.arcade(thrustHandler(throttle), thrustHandler(theta));

   pros::delay(DEFAULT_DELAY_LENGTH);
}

void Drivetrain::tankDrive() {
   int left = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
   int right = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

   chassis.tank(thrustHandler(left), thrustHandler(right));

   pros::delay(DEFAULT_DELAY_LENGTH);
}

// Run the drivetrain depending on the control mode
void Drivetrain::run() {
   switch (Drivetrain::driveMode) {
   case CURVATURE_DRIVE:
      Drivetrain::curveDrive();
      break;
   case ARCADE_DRIVE:
      Drivetrain::arcadeDrive();
      break;
   case TANK_DRIVE:
      Drivetrain::tankDrive();
      break;
   }
}

// Cycle through each drivetrain control mode, overflows back to 0
std::string Drivetrain::toggleDrive() {
   // int driveMode = (Drivetrain::driveMode + 1) % (TANK_DRIVE + 1);
   // cycle between tank and arcade
   if (Drivetrain::driveMode == TANK_DRIVE) {
      Drivetrain::driveMode = ARCADE_DRIVE;
      return "Arcade Drive";
   } else {
      Drivetrain::driveMode = TANK_DRIVE;
      return "Tank Drive";
   }
   // return setDriveMode(driveMode);
}

std::string Drivetrain::getModeChar() {
   switch (Drivetrain::driveMode) {
   case CURVATURE_DRIVE:
      return "C";
   case ARCADE_DRIVE:
      return "A";
   case TANK_DRIVE:
      return "T";
   default:
      return "N";
   }
}

int Drivetrain::thrustHandler(int thrust) {
   if (Drivetrain::isReversed) {
      // Sets each motor to its opposite direction - see globals.cpp for motor ports
      return thrust * -1;
   }
   return thrust;
}

// Switch the drivetrain control mode between arcade and tank drive with the down button(between 1 and 2)
std::string Drivetrain::setDriveMode(int driveMode) {
   switch (driveMode) {
   case 0:
      Drivetrain::driveMode = CURVATURE_DRIVE;
      std::cout << "Curvature Drive" << std::endl;
      return "Curvature Drive";
   case 1:
      Drivetrain::driveMode = ARCADE_DRIVE;
      std::cout << "Arcade Drive" << std::endl;
      return "Arcade Drive";
   case 2:
      Drivetrain::driveMode = TANK_DRIVE;
      std::cout << "Tank Drive" << std::endl;
      return "Tank Drive";
   default:
      std::cout << "Not a valid drivetrain control mode!" << std::endl;
      return "Not a valid driveMode!";
   }
}