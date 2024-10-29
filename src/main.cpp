#include "main.h"

#include "config.hpp"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "pros/misc.h"
#include "util/slewRateLimiter.hpp"
#include "util/triggerUtil.hpp"

// TODO: Migrate Drivebase into another file

// Controller
pros::Controller master(pros::E_CONTROLLER_MASTER);

// Motor Ports
/*
pros::Motor front_left_motor(FRONT_LEFT_MOTOR_PORT, FRONT_LEFT_MOTOR_GEAR);
pros::Motor middle_left_motor(MIDDLE_LEFT_MOTOR_PORT, MIDDLE_LEFT_MOTOR_GEAR);
pros::Motor back_left_motor(BACK_LEFT_MOTOR_PORT, BACK_LEFT_MOTOR_GEAR);
pros::Motor front_right_motor(FRONT_RIGHT_MOTOR_PORT, FRONT_RIGHT_MOTOR_GEAR);
pros::Motor middle_right_motor(MIDDLE_RIGHT_MOTOR_PORT,
MIDDLE_RIGHT_MOTOR_GEAR); pros::Motor back_right_motor(BACK_RIGHT_MOTOR_PORT,
BACK_RIGHT_MOTOR_GEAR); std::vector<pros::Motor> motors_left{front_left_motor,
middle_left_motor, back_left_motor}; std::vector<pros::Motor>
motors_right{front_right_motor, middle_right_motor, back_right_motor};
*/
// Drive Motor Group
pros::MotorGroup left_mg({DriveConstants::FRONT_LEFT_MOTOR_PORT, DriveConstants::MIDDLE_LEFT_MOTOR_PORT,
              DriveConstants::BACK_LEFT_MOTOR_PORT},
             pros::MotorGearset::green);
pros::MotorGroup right_mg({DriveConstants::FRONT_RIGHT_MOTOR_PORT, DriveConstants::MIDDLE_RIGHT_MOTOR_PORT,
               DriveConstants::BACK_RIGHT_MOTOR_PORT},
              pros::MotorGearset::green);

lemlib::Drivetrain drivetrain(
    &left_mg,         // left motor group
    &right_mg,        // right motor group
    DriveConstants::TRACK_WIDTH,      // 10 inch track width
    DriveConstants::WHEEL_DIAMETER,   // using new 4" omnis
    DriveConstants::DRIVETRAIN_RPM,   // drivetrain rpm is 360
    DriveConstants::HORIZONTAL_DRIFT  // horizontal drift is 2 (for now)
);
// Drivetrain Settings
pros::Imu imu(DriveConstants::IMU_PORT);
pros::Rotation vertical_rotation(DriveConstants::TRACKING_WHEEL_PORT);

lemlib::TrackingWheel vertical_tracking_wheel(&vertical_rotation,
                                              lemlib::Omniwheel::NEW_2,
                                              DriveConstants::TRACKING_WHEEL_OFFSET);

// Odometry
lemlib::OdomSensors odom_sensors(&vertical_tracking_wheel, nullptr, nullptr,
                                 nullptr, &imu);

// Chassis
lemlib::Chassis chassis(drivetrain,          // drivetrain
                        DriveConstants::lateral_controller,  // lateral controller
                        DriveConstants::angular_controller,  // angular controller
                        odom_sensors         // odom sensors
);

// Drive Mode
Toggle tankDriveToggle([]() -> bool { return master.get_digital(DIGITAL_A); },
                       TriggerMode::RISING_EDGE);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  printf("Initializing...\n");
  master.print(0, 0, "State: INIT");
  pros::lcd::initialize();
  chassis.calibrate();

  // Set Motot Idle Mode
  right_mg.set_brake_mode(DriveConstants::BRAKE_MODE);
  left_mg.set_brake_mode(DriveConstants::BRAKE_MODE);

  pros::Task screen_task([&]() {
    while (true) {
      pros::lcd::print(0, "X: %f", chassis.getPose().x);          // x
      pros::lcd::print(1, "Y: %f", chassis.getPose().y);          // y
      pros::lcd::print(2, "Theta: %f", chassis.getPose().theta);  // heading
      pros::delay(STANDARD_DELAY);
    }
  });
  printf("Initialized\n");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  master.print(0, 0, "State: DISABLED");
  printf("Disabled\n");
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() { printf("Competition Initialized\n"); }

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
void autonomous() {
  printf("Starting auto\n");
  master.print(0, 0, "State: AUTO");
  chassis.setPose(0, 0, 0);
  chassis.turnToHeading(90, 100000);
}

void arcade() {
  // pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT)
  // >> 2,
  //                  (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
  //                  (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  //
  //                  Prints status of the emulated screen LCDs
  // Move the contoller out to variable
  double throttle = master.get_analog(ANALOG_LEFT_Y);
  double theta = master.get_analog(ANALOG_LEFT_X) * 0.5;
  // Artificial acceleration

  // Drive the robot
  chassis.arcade(throttle, theta);
}

void tank() {
  double left = master.get_analog(ANALOG_LEFT_Y);
  double right = master.get_analog(ANALOG_RIGHT_Y);
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
  printf("Starting opcontrol\n");
  while (true) {
    // Check if the robot is in tank drive mode

    bool tankDrive = tankDriveToggle.getState();
    // Print the drive mode to controller
    master.print(0, 0, "State: OPCTRL");
    master.print(1, 0, "Drive Mode: %s", tankDrive ? "Tank" : "Arcade");

    if (master.get_digital(DIGITAL_B)) {
      // chassis.moveToPose(3, 3, 90, 4000);
      chassis.turnToHeading(90, 4000);
      pros::delay(1000);
      chassis.turnToHeading(0, 4000);
      pros::delay(1000);
      chassis.turnToHeading(-90, 4000);
      pros::delay(1000);
      chassis.turnToHeading(0, 4000);
    } else if (master.get_digital(DIGITAL_Y)) {
      chassis.moveToPose(0, 0, 0, 5000);
    } else if (master.get_digital(DIGITAL_X)) {
      printf("Tuning PID\n");
      chassis.setPose(0, 0, 0);
      chassis.turnToHeading(90, 100000);
      printf("Done Tuning PID\n");
      // print kp, ki
      
    } else {
      // Drive the robot
      if (tankDrive) {
        tank();
      } else {
        arcade();
      }
    }

    pros::delay(20);  // Run for 20 ms then update
  }
}