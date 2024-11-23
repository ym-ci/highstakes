#pragma once

#include "api.h"
#include "lemlib/api.hpp"
#include "pros/vision.hpp"

// The following files are imported in order to provide type definitions into
// the compiler that allows for the objecys to

/**
 * @file globals.h
 * @brief Contains global variables and type definitions for the Robot
 * namespace.
 */

/**
 * @namespace Robot
 * @details In order to construct the robot, the Robot namespace is used to
 * contain all of the objects that are used to control the robot. This includes
 * the subsystems, methods, and global objects. We use PROS extensively for the
 * robot, it can be found at <A HREF="https://pros.cs.purdue.edu">PROS</A>
 * @authors Absozero, Jaytb, 1516X
 * @brief Contains all objects generated by the team - Contains all of the
 * subsystems, methods, and global objects.
 */
namespace Robot {

/**
 * @brief Contains global variables and type definitions for the Robot
 * namespace.
 * @details The majority of the global variables are defined in the Globals
 * namespace. This is to allow for easy access to the variables from any file in
 * the project. The Globals namespace is also used to hold lemlib objects that
 * are used to control the autonomous functions of the robot. It contains
 * parameters that are used to control the PID that lemlib uses, additionally.
 */
namespace Globals {

extern pros::Controller partner;
extern pros::Controller controller;

extern pros::Motor rightFront;
extern pros::Motor leftFront;
extern pros::Motor leftBack;
extern pros::Motor rightBack;
extern pros::Motor leftMid;

extern pros::motor_brake_mode_e_t brakeMode;

extern pros::Motor rightMid;
extern pros::Motor intakeMotor;
// extern pros::Motor hookMotor;

extern pros::MotorGroup punchers;
extern pros::MotorGroup driveLeft;
extern pros::MotorGroup driveRight;
// Used as a motor group meant to be passed into lemlib, a full drivetrain
// group.
extern pros::MotorGroup drive;

extern lemlib::Drivetrain drivetrain;

extern lemlib::OdomSensors sensors;

extern pros::Vision colorSensor;

extern pros::vision_signature_s_t RED_SIG;

extern pros::vision_signature_s_t BLUE_SIG;
extern pros::vision_signature_s_t BLUE_DARK_SIG;

extern pros::Rotation lateral_sensor;
extern pros::Rotation horizontalSensor;

extern pros::Imu imu;

// forward/backward PID
extern lemlib::ControllerSettings lateralController;

// turning PID
extern lemlib::ControllerSettings angularController;

// curvature
extern lemlib::ExpoDriveCurve steerCurve;
extern lemlib::ExpoDriveCurve throttleCurve;

extern lemlib::Chassis chassis;

extern lemlib::ExpoDriveCurve throttleCurve;

extern lemlib::ExpoDriveCurve steerCurve;

extern pros::adi::DigitalIn drivetrainToggleSwitch;
extern pros::adi::DigitalIn autonToggleSwitch;

extern pros::adi::Pneumatics latchControl;
extern pros::adi::Pneumatics doinkerControl;
extern pros::adi::Pneumatics HangControl;

} // namespace Globals
} // namespace Robot
