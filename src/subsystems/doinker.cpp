#include "robot/doinker.h"

#include "globals.h"

using namespace Robot;
using namespace Robot::Globals;

void Doinker::run() {
   if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
      Doinker::toggle();
   }
}

Doinker::Doinker() { ; }

void Doinker::toggle() { doinkerControl.toggle(); }

bool Doinker::getState() { return doinkerControl.is_extended(); }