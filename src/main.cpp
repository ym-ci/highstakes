#include "main.h"

#include "config.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/misc.h"
#include "util/slewRateLimiter.hpp"
#include "util/triggerUtil.hpp"

// Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Motor Ports
pros::Motor front_left_motor(FRONT_LEFT_MOTOR_PORT, FRONT_LEFT_MOTOR_GEAR);
// pros::Motor middle_left_motor(MIDDLE_LEFT_MOTOR_PORT,
// MIDDLE_LEFT_MOTOR_GEAR); pros::Motor back_left_motor(BACK_LEFT_MOTOR_PORT,
// BACK_LEFT_MOTOR_GEAR); pros::Motor front_right_motor(FRONT_RIGHT_MOTOR_PORT,
// FRONT_RIGHT_MOTOR_GEAR); pros::Motor
// middle_right_motor(MIDDLE_RIGHT_MOTOR_PORT, MIDDLE_RIGHT_MOTOR_GEAR);
// pros::Motor back_right_motor(BACK_RIGHT_MOTOR_PORT, BACK_RIGHT_MOTOR_GEAR);
// Drive Motor Group
pros::MotorGroup left_mg({FRONT_LEFT_MOTOR_PORT, BACK_LEFT_MOTOR_PORT});
pros::MotorGroup right_mg({FRONT_RIGHT_MOTOR_PORT, BACK_RIGHT_MOTOR_PORT});

// Drivetrain Settings
pros::Imu imu(IMU_PORT);
pros::Rotation vertical_rotation(TRACKING_WHEEL_PORT);

lemlib::TrackingWheel vertical_tracking_wheel(&vertical_rotation,
                                              lemlib::Omniwheel::NEW_2,
                                              TRACKING_WHEEL_OFFSET);

lemlib::Drivetrain drivetrain(
    &left_mg,         // left motor group
    &right_mg,        // right motor group
    TRACK_WIDTH,      // 10 inch track width
    WHEEL_DIAMETER,   // using new 4" omnis
    DRIVETRAIN_RPM,   // drivetrain rpm is 360
    HORIZONTAL_DRIFT  // horizontal drift is 2 (for now)
);

// Odometry
lemlib::OdomSensors odom_sensors(&vertical_tracking_wheel, nullptr, nullptr,
                                 nullptr, &imu);

// Chassis
lemlib::Chassis chassis(drivetrain,          // drivetrain
                        lateral_controller,  // lateral controller
                        angular_controller,  // angular controller
                        odom_sensors         // odom sensors
);

// Speed Toggle
Toggle speedToggle([]() -> bool { return master.get_digital(DIGITAL_X); },
                   TriggerMode::RISING_EDGE);

// Arrtificial acceleration
SlewRateLimiter throttleSlew(0.1);
SlewRateLimiter turnSlew(0.1);
SlewRateLimiter leftSlew(0.1);
SlewRateLimiter rightSlew(0.1);

// Artificial acceleration toggle
Toggle accelToggle([]() -> bool { return master.get_digital(DIGITAL_Y); },
                   TriggerMode::RISING_EDGE);

// Drive Mode
Toggle tankDriveToggle([]() -> bool { return master.get_digital(DIGITAL_A); },
                       TriggerMode::RISING_EDGE);

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
  static bool pressed = false;
  pressed = !pressed;
  if (pressed) {
    pros::lcd::set_text(2,
                        "I'm a little teapot short and st(d)out. Here is my "
                        "handle here is my spout!");
  } else {
    pros::lcd::clear_line(2);
  }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  pros::lcd::initialize();
	chassis.calibrate();
  pros::lcd::set_text(1, "Hello PROS User!");

  pros::lcd::register_btn1_cb(on_center_button);

  // Set Motot Idle Mode
  right_mg.set_brake_mode(BRAKE_MODE);
  left_mg.set_brake_mode(BRAKE_MODE);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

void arcade() {
  // pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT)
  // >> 2,
  //                  (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
  //                  (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  //
  //                  Prints status of the emulated screen LCDs
  // Move the contoller out to variable
  double throttle = master.get_analog(ANALOG_LEFT_Y);
  double theta = master.get_analog(ANALOG_LEFT_X) * 0.5;
  // Speed Multiplier
  double speedMultiplier = speedToggle.getState() ? 0.5 : 1;
  // Apply the speed multiplier
  throttle *= speedMultiplier;
  theta *= speedMultiplier;
  // Artificial acceleration
  if (accelToggle.getState()) {
    throttle = throttleSlew.slew(throttle);
    theta = turnSlew.slew(theta);
  }
  // Print the Analog Joystick values
  pros::lcd::print(0, "Left Joystick: %s", std::to_string(throttle).c_str());
  pros::lcd::print(1, "Right Joystick: %s", std::to_string(theta).c_str());
  pros::lcd::print(2, "Speed Multiplier: %s",
                   std::to_string(speedMultiplier).c_str());
  pros::lcd::print(3, "Acceleration: %d", accelToggle.getState());

  // Drive the robot
  chassis.arcade(throttle, theta);
}

void tank() {
  double left = master.get_analog(ANALOG_LEFT_Y);
  double right = master.get_analog(ANALOG_RIGHT_Y);

  // Speed Multiplier
  double speedMultiplier = speedToggle.getState() ? 0.5 : 1.0;
  // Apply the speed multiplier
  left *= speedMultiplier;
  right *= speedMultiplier;
  // Artificial acceleration
  if (accelToggle.getState()) {
    left = leftSlew.slew(left);
    right = rightSlew.slew(right);
  }

  // Print the Analog Joystick values
  pros::lcd::print(0, "Left Joystick: %s", std::to_string(left).c_str());
  pros::lcd::print(1, "Right Joystick: %s", std::to_string(right).c_str());
  pros::lcd::print(2, "Speed Multiplier: %s",
                   std::to_string(speedMultiplier).c_str());
  pros::lcd::print(3, "Acceleration: %d", accelToggle.getState());

  // Drive the robot
  chassis.tank(left, right);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  while (true) {
    // Check if the robot is in tank drive mode

    bool tankDrive = tankDriveToggle.getState();
    // Print the drive mode to controller
    master.print(0, 0, "Drive Mode: %s", tankDrive ? "Tank" : "Arcade");

    // Drive the robot
    if (tankDrive) {
      tank();
    } else {
      arcade();
    }

    pros::delay(20);  // Run for 20 ms then update
  }
}