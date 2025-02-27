#include "robot/ladybrown.h"
#include "globals.h"
#include "lemlib/pid.hpp"
#include "lemlib/timer.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include <cstdlib>

using namespace Robot;
using namespace Robot::Globals;

LadyBrown::LADYBROWN_STATE LadyBrown::current_state = LadyBrown::BASE_STATE;

LadyBrown::LadyBrown() : MoveToPointPID(50, 0, 0, 2, false) {
  ladyBrownMotor1.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  ladyBrownMotor2.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

void LadyBrown::run(bool async, int timeout) {
   LADYBROWN_STATE move_to;
   std::cout << "ladybrown encoder value: " << get_position() << " state " << current_state << std::endl;

   if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
      if (current_state == BASE_STATE) {
         move_to = LOAD_STATE;
      } else if (current_state == LOAD_STATE) {
         move_to = ATTACK_STATE;
      } else {
         move_to = BASE_STATE;
      }
      std::cout << "Moving to: " << move_to << std::endl;
      std::cout << "current state: " << current_state << std::endl;

      if (!async) {
         MoveToPoint(move_to);
      } else {

         pros::Task move([move_to, this]() { MoveToPoint(move_to); });
      }

      if (!isPIDRunning) {
         current_state = move_to;
      }

   } else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {

      if (current_state == ATTACK_STATE) {
         move_to = LOAD_STATE;
      } else if (current_state == LOAD_STATE) {
         move_to = BASE_STATE;
      } else {
         return;
      }

      std::cout << "Moving to: " << move_to << std::endl;
      std::cout << "current state: " << current_state << std::endl;

      if (!async) {
         MoveToPoint(move_to);
      } else {
         pros::Task move([move_to, this]() { MoveToPoint(move_to); }, "LadyBrownMove");
      }

      if (!isPIDRunning) {
         current_state = move_to;
      }
   }
}

int LadyBrown::get_target() { return target; }

double LadyBrown::get_position() {
  return (ladyBrownMotor1.get_position() + ladyBrownMotor2.get_position()) / 2;
}

void LadyBrown::MoveToPoint(LADYBROWN_STATE state, int max_error, int timeout) {

   std::cout << "state: " << current_state << std::endl;
   constexpr double baseLocation = 0;
   constexpr double loadLocation = 60;
   constexpr double attackLocation = 330;

   int target;

   std::cout << "state: " << state << std::endl;
   std::cout << "pid: " << isPIDRunning << std::endl;

   if (!isPIDRunning) {

      std::cout << "inner pid: " << isPIDRunning << std::endl;
      LadyBrown::isPIDRunning = true;

      switch (state) {
      case LADYBROWN_STATE::BASE_STATE:
         target = baseLocation;
         break;
      case LADYBROWN_STATE::LOAD_STATE:
         target = loadLocation;
         break;
      case LADYBROWN_STATE::ATTACK_STATE:
         target = attackLocation;
         break;
      }

      std::cout << "target: " << target << std::endl;

      MoveToPointPID.reset();

      lemlib::Timer timer(timeout);

      while (true) {
         double error = target - get_position();
         double motor_voltage = MoveToPointPID.update(error);

         // motor_voltage = lemlib::slew(motor_voltage, LadyBrownMotor.get_voltage(), 1500);

         if (std::abs(error) < max_error || timer.isDone()) {
            ladyBrownMotor1.brake();
            ladyBrownMotor2.brake();
            LadyBrown::isPIDRunning = false;
            LadyBrown::current_state = state;
            break;
         }

         ladyBrownMotor1.move_voltage(motor_voltage);
         ladyBrownMotor2.move_voltage(motor_voltage);
         LadyBrown::isPIDRunning = true;
         pros::delay(20);
      }
   }
}