#include "robot/globals.h"

namespace Robot {
namespace Constants {
constexpr static const float WHEEL_DIAMETER = lemlib::Omniwheel::NEW_4;
constexpr static const float TRACK_WIDTH = 12.713702;
constexpr static const float DRIVETRAIN_RPM = 160;
constexpr static const float HORIZONTAL_DRIFT = 4;

}  // namespace Constants
namespace Globals {

pros::Controller master(pros::E_CONTROLLER_MASTER);

// Motors
pros::Motor frontLeftMotor(-10, pros::v5::MotorGears::green,
                           pros::v5::MotorUnits::degrees);
pros::Motor middleLeftMotor(-9, pros::v5::MotorGears::green,
                            pros::v5::MotorUnits::degrees);
pros::Motor backLeftMotor(-8, pros::v5::MotorGears::green,
                          pros::v5::MotorUnits::degrees);

pros::Motor frontRightMotor(1, pros::v5::MotorGears::green,
                            pros::v5::MotorUnits::degrees);
pros::Motor middleRightMotor(2, pros::v5::MotorGears::green,
                             pros::v5::MotorUnits::degrees);
pros::Motor backRightMotor(3, pros::v5::MotorGears::green,
                           pros::v5::MotorUnits::degrees);

// Intake
pros::Motor intakeMotor(7, pros::v5::MotorGears::green,
                        pros::v5::MotorUnits::degrees);

// pros::Motor climbMotor(5, pros::v5::MotorGears::red,
// pros::v5::MotorUnits::degrees);

// Groups
pros::MotorGroup leftMotors({frontLeftMotor.get_port(),
                             middleLeftMotor.get_port(),
                             backLeftMotor.get_port()});
pros::MotorGroup rightMotors({frontRightMotor.get_port(),
                              middleRightMotor.get_port(),
                              backRightMotor.get_port()});

// Sensors
pros::Imu imu(5);
pros::Rotation verticalRotation(6);

// Drivetrain
lemlib::TrackingWheel verticalTrackingWheel(&verticalRotation,
                                            lemlib::Omniwheel::NEW_2,
                                            1  // OFFSET
);

lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, Constants::TRACK_WIDTH,
                              Constants::WHEEL_DIAMETER,
                              Constants::DRIVETRAIN_RPM,
                              Constants::HORIZONTAL_DRIFT);

// Odometry
lemlib::OdomSensors odomSensors(&verticalTrackingWheel,  // vertical 1
                                nullptr,                 // vertical 2
                                nullptr,                 // horizontal 1
                                nullptr,                 // horizontal 2
                                &imu                     // imu
);

// Definition of the PID controller settings outside the class
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

const lemlib::ControllerSettings angularController(
    4,   // proportional gain (kP)
    0,   // integral gain (kI)
    35,  // derivative gain (kD)
    0,   // anti windup
    0,   // small error range, in degrees
    0,   // small error range timeout, in milliseconds
    0,   // large error range, in degrees
    0,   // large error range timeout, in milliseconds
    0    // maximum acceleration (slew)
);

lemlib::Chassis chassis(drivetrain, lateral_controller, angularController,
                        odomSensors)

}  // namespace Globals
}  // namespace Robot