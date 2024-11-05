#pragma once
#include "api.h"
#include "lemlib/api.hpp"

namespace Robot {
namespace Globals {
// Controllers
extern pros::Controller master;

// Motors
// left
extern pros::Motor frontLeftMotor;
extern pros::Motor middleLeftMotor;
extern pros::Motor backLeftMotor;

// right
extern pros::Motor frontRightMotor;
extern pros::Motor middleRightMotor;
extern pros::Motor backRightMotor;

// Intake
extern pros::Motor intakeMotor;

// Groups
extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;

// Sensors
extern pros::Imu imu;
extern pros::Rotation verticalRotation;

// Drivetrain
extern lemlib::TrackingWheel verticalTrackingWheel;
extern lemlib::Drivetrain drivetrain;

// Odometry
extern lemlib::OdomSensors odomSensors;

extern lemlib::Chassis chassis;

// TODO: pneumatics once we get them hooked up
}  // namespace Globals
}  // namespace Robot