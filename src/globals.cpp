#include "globals.h"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/vision.h"
#include "pros/vision.hpp"

/*
 * Although the following constants belong in their own seperate
 * files(auton.cpp, drivetriain.cpp), they are put here in order to maintain a
 * common location for all of the constants used by the program to belong in.
 * NOTE: This is the location where these variables are put into memory, but
 * they can be otherwise modified throughout the program.
 */

// Defines the objects that are used by the program for each of the individual
// subsystems.

namespace Robot {
namespace Globals {

pros::Controller controller(pros::E_CONTROLLER_MASTER);
pros::Controller partner(pros::E_CONTROLLER_PARTNER);

pros::Motor leftFront(-14, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor leftMid(-16, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor leftBack(-18, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor rightFront(2, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor rightMid(7, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor rightBack(20, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor intakeMotor(11, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);

pros::motor_brake_mode_e_t brakeMode = pros::E_MOTOR_BRAKE_BRAKE;


// pros::Motor hookMotor(2, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);

// placeholder port number

pros::adi::Pneumatics latchControl('A', false);
pros::adi::Pneumatics HangControl('B', false);

// pros::Rotation lateral_sensor(16);
pros::Rotation horizontalSensor(-10);

pros::Imu imu(5);

// Vision sensor configuration
pros::Vision colorSensor(3);

pros::vision_signature_s_t RED_SIG =
    pros::c::vision_signature_from_utility(1, -4653, -3619, -4136, 9831, 11725, 10778, 2.5, 0);

pros::vision_signature_s_t BLUE_SIG =
    pros::c::vision_signature_from_utility(2, 9187, 12161, 10674, -375, 1327, 476, 2.5, 0);
pros::vision_signature_s_t BLUE_DARK_SIG =
    pros::c::vision_signature_from_utility(3, -4793, -4173, -4483, 1069, 2765, 1917, 3, 0);

// Pros motor groups - most used by lemlib
pros::MotorGroup driveLeft({leftFront.get_port(), leftMid.get_port(), leftBack.get_port()});
pros::MotorGroup driveRight({rightFront.get_port(), rightMid.get_port(), rightBack.get_port()});
pros::MotorGroup drive({leftFront.get_port(), rightFront.get_port(), leftMid.get_port(), rightMid.get_port(),
                         leftBack.get_port(), rightBack.get_port()});

// Lemlib objects - Used by lemlib drive and odometry functions
lemlib::TrackingWheel horizontalTrackingWheel(
                                        &horizontalSensor, 
                                  lemlib::Omniwheel::NEW_2,
                                       1);
// lemlib::TrackingWheel vertical_tracking_wheel(&lateral_sensor, lemlib::Omniwheel::NEW_2, -1.45);

// Describes the lemlib objects that are used to control the autonomous
// functions of the robot.
lemlib::Drivetrain drivetrain{
    &driveLeft,  // left drivetrain motors
    &driveRight, // right drivetrain motors
    12.713702,        // track width
    lemlib::Omniwheel::NEW_4,
    250, // drivetrain rpm is 450
    2    // horizontal drift is 2
};

lemlib::OdomSensors sensors{
    nullptr,   // vertical tracking wheel 1
    nullptr,                    // vertical tracking wheel 2
    &horizontalTrackingWheel, // horizontal tracking wheel 1
    nullptr,                    // we don't have a second tracking wheel, so we set it to nullptr
    &imu            // inertial sensor
};

// forward/backward PID
// lateral PID controller
lemlib::ControllerSettings lateralController(10,  // proportional gain (kP)
                                              0,   // integral gain (kI)
                                              4.5, // derivative gain (kD)
                                              3,   // anti windup
                                              1,   // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3,   // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              25   // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angularController(2,    // proportional gain (kP)
                                              0,    // integral gain (kI)
                                              10.5, // derivative gain (kD)
                                              3,    // anti windup
                                              1,    // small error range, in degrees
                                              100,  // small error range timeout, in milliseconds
                                              2,    // large error range, in degrees
                                              500,  // large error range timeout, in milliseconds
                                              0     // maximum acceleration (slew)
);

lemlib::ExpoDriveCurve throttleCurve(3,    // joystick deadband out of 127
                                      10,   // minimum output where drivetrain will move out of 127
                                      1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3,    // joystick deadband out of 127
                                   10,   // minimum output where drivetrain will move out of 127
                                   1.019 // expo curve gain
);

lemlib::Chassis chassis(drivetrain, lateralController, angularController, sensors, &throttleCurve, &steerCurve);

} // namespace Globals

} // namespace Robot