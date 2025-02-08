#include "robot/latch.h"

#include "globals.h"

using namespace Robot;
using namespace Robot::Globals;

void Latch::run() {
   if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
      Latch::toggle();
   }
}

Latch::Latch() { ; }

void Latch::toggle() { latchControl.toggle(); }

bool Latch::getState() { return latchControl.is_extended(); }
void Latch::extend() { latchControl.extend(); }
void Latch::retract() { latchControl.retract(); }