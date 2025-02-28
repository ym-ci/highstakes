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

pros::Motor leftFront(-16, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor leftBack(-20, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor rightFront(3, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor rightBack(9, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor intakeMotor(1, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor conveyorMotor(14, pros::v5::MotorGears::blue, pros::v5::MotorUnits::degrees);
// pros::Motor ladyBrownMotor1(15, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor ladyBrownMotor1(-18, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);

pros::motor_brake_mode_e_t brakeMode = pros::E_MOTOR_BRAKE_BRAKE;


// pros::Motor hookMotor(2, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);

// placeholder port number

pros::adi::Pneumatics latchControl('H', false);
pros::adi::Pneumatics doinkerControl('G', false);

pros::Rotation lateral_sensor(-10);
// pros::Rotation horizontalSensor(-7);

pros::Imu imu(6 );

// Vision sensor configuration
pros::Vision colorSensor(11);

pros::vision_signature_s_t RED_SIG =
    pros::c::vision_signature_from_utility(1, 9199, 11847, 10523, -3217, -1359, -2288, 2.5, 0);

pros::vision_signature_s_t BLUE_SIG =
    pros::c::vision_signature_from_utility(2, -5099, -4103, -4601, 843, 8649, 4746, 2.5, 0);
// pros::vision_signature_s_t BLUE_DARK_SIG =
//     pros::c::vision_signature_from_utility(3, -4793, -4173, -4483, 1069, 2765, 1917, 3, 0);

// Pros motor groups - most used by lemlib
pros::MotorGroup driveLeft({leftFront.get_port(), leftBack.get_port()});
pros::MotorGroup driveRight({rightFront.get_port(), rightBack.get_port()});
pros::MotorGroup drive({leftFront.get_port(), rightFront.get_port(), leftBack.get_port(), rightBack.get_port()});

// Lemlib objects - Used by lemlib drive and odometry functions
// lemlib::TrackingWheel horizontalTrackingWheel(
//                                         &horizontalSensor, 
//                                   lemlib::Omniwheel::NEW_2,
//                                        1);
lemlib::TrackingWheel vertical_tracking_wheel(&lateral_sensor, lemlib::Omniwheel::NEW_2, -3.5);

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
    &vertical_tracking_wheel,   // vertical tracking wheel 1
    nullptr,                    // vertical tracking wheel 2
    nullptr, // horizontal tracking wheel 1
    nullptr,                    // we don't have a second tracking wheel, so we set it to nullptr
    &imu            // inertial sensor
};

// forward/backward PID
// lateral PID controller
lemlib::ControllerSettings lateralController(16,  // proportional gain (kP)
                                              1.5,   // integral gain (kI)
                                              0.4, // derivative gain (kD)
                                              3,   // anti windup
                                              1,   // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3,   // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              25   // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angularController(2.5,    // proportional gain (kP)
                                              0,    // integral gain (kI)
                                              9.5, // derivative gain (kD)
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