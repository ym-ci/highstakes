// config.hpp
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "lemlib/chassis/chassis.hpp"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/motors.h"
#include <sys/_intsup.h>

class DriveConstants {
public:
    // Motor Ports
    static const int FRONT_LEFT_MOTOR_PORT = -10;
    static const int MIDDLE_LEFT_MOTOR_PORT = -9;
    static const int BACK_LEFT_MOTOR_PORT = -8;
    
    static const int FRONT_RIGHT_MOTOR_PORT = 11;
    static const int MIDDLE_RIGHT_MOTOR_PORT = 2;
    static const int BACK_RIGHT_MOTOR_PORT = 12;

    // Sensor Ports
    static const int IMU_PORT = 1;
    constexpr static const   float TRACKING_WHEEL_PORT = 2.5;
    static const int TRACKING_WHEEL_OFFSET = 1;

    // Motor Gearing
    static const pros::v5::MotorGears FRONT_LEFT_MOTOR_GEAR = pros::v5::MotorGears::green;
    static const pros::v5::MotorGears MIDDLE_LEFT_MOTOR_GEAR = pros::v5::MotorGears::green;
    static const pros::v5::MotorGears BACK_LEFT_MOTOR_GEAR = pros::v5::MotorGears::green;
    static const pros::v5::MotorGears FRONT_RIGHT_MOTOR_GEAR = pros::v5::MotorGears::green;
    static const pros::v5::MotorGears MIDDLE_RIGHT_MOTOR_GEAR = pros::v5::MotorGears::green;
    static const pros::v5::MotorGears BACK_RIGHT_MOTOR_GEAR = pros::v5::MotorGears::green;

    // Drivetrain Settings
    constexpr static const float WHEEL_DIAMETER = lemlib::Omniwheel::NEW_4;
    constexpr static const float TRACK_WIDTH = 12.713702;
    constexpr static const float DRIVETRAIN_RPM = 160;
    constexpr static const float HORIZONTAL_DRIFT = 4;

    // Other Drivetrain Settings
    static const pros::motor_brake_mode_e_t BRAKE_MODE= pros::E_MOTOR_BRAKE_BRAKE;
    // lateral PID controller
    static const lemlib::ControllerSettings lateral_controller;
    
    // angular PID controller
    static const lemlib::ControllerSettings angular_controller;
};

// Definition of the PID controller settings outside the class
const lemlib::ControllerSettings DriveConstants::lateral_controller(
    20,   // proportional gain (kP)
    6,    // integral gain (kI)
    0,    // derivative gain (kD)
    3,    // anti windup
    1,    // small error range, in inches
    100,  // small error range timeout, in milliseconds
    3,    // large error range, in inches
    500,  // large error range timeout, in milliseconds
    20    // maximum acceleration (slew)
);

const lemlib::ControllerSettings DriveConstants::angular_controller(
    4,    // proportional gain (kP)
    0,    // integral gain (kI)
    35,   // derivative gain (kD)
    0,    // anti windup
    0,    // small error range, in degrees
    0,    // small error range timeout, in milliseconds
    0,    // large error range, in degrees
    0,    // large error range timeout, in milliseconds
    0     // maximum acceleration (slew)
);

// General Configuration

const int STANDARD_DELAY = 20;



#endif  // CONFIG_HPP