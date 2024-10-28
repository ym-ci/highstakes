// config.hpp
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/motors.h"

// Motor Ports
// left
const int FRONT_LEFT_MOTOR_PORT = -10;
const int MIDDLE_LEFT_MOTOR_PORT = -9;
const int BACK_LEFT_MOTOR_PORT = -8 ;
// right
const int FRONT_RIGHT_MOTOR_PORT = 1;
const int MIDDLE_RIGHT_MOTOR_PORT = 2;
const int BACK_RIGHT_MOTOR_PORT = 3;

// Sensor Ports
const int IMU_PORT = 5;
const int TRACKING_WHEEL_PORT = 6;
const int TRACKING_WHEEL_OFFSET = 1;

// Motor Gearing
const pros::v5::MotorGears FRONT_LEFT_MOTOR_GEAR = pros::v5::MotorGears::green;
const pros::v5::MotorGears MIDDLE_LEFT_MOTOR_GEAR = pros::v5::MotorGears::green;
const pros::v5::MotorGears BACK_LEFT_MOTOR_GEAR = pros::v5::MotorGears::green;
const pros::v5::MotorGears FRONT_RIGHT_MOTOR_GEAR = pros::v5::MotorGears::green;
const pros::v5::MotorGears MIDDLE_RIGHT_MOTOR_GEAR =
    pros::v5::MotorGears::green;
const pros::v5::MotorGears BACK_RIGHT_MOTOR_GEAR = pros::v5::MotorGears::green;

// Drivetrain Settings
const float WHEEL_DIAMETER = lemlib::Omniwheel::NEW_4;
const float TRACK_WIDTH = 12.713702;
const float DRIVETRAIN_RPM = 160;

/* horizontal drift controls how fast the robot should move through turns, and it relates to how easily the robot can be pushed side to side 
A value of 2 is good for Omni drivetrains
8 is good for tractions
higher number -> greater speed in curves */
const float HORIZONTAL_DRIFT = 4; // we are half and half so 4 is good

// Other Drivetrain Settings
const pros::motor_brake_mode_e_t BRAKE_MODE = pros::E_MOTOR_BRAKE_BRAKE;

// lateral PID controller
const lemlib::ControllerSettings lateral_controller(
    10,   // proportional gain (kP)
    6,    // integral gain (kI)
    0,    // derivative gain (kD)
    3,    // anti windup
    1,    // small error range, in inches
    100,  // small error range timeout, in milliseconds
    3,    // large error range, in inches
    500,  // large error range timeout, in milliseconds
    20    // maximum acceleration (slew)
);

// angular PID controller
const lemlib::ControllerSettings angular_controller(
    4,    // proportional gain (kP)
    0,    // integral gain (kI)
    35,   // derivative gain (kD)
    0,    // anti windup
    0,    // small error range, in degrees
    0,  // small error range timeout, in milliseconds
    0,    // large error range, in degrees
    0,  // large error range timeout, in milliseconds
    0     // maximum acceleration (slew)
);

#endif  // CONFIG_HPP