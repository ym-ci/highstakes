#include "util/velocityPID.h"
#include "globals.h"

using namespace Robot;
using namespace Robot::Globals;

VelocityPID::VelocityPID( float kP, float kI, float kD, float windupRange, bool signFlipReset, float kV )
    : kP( kP ), kI( kI ), kD( kD ), windupRange( windupRange ), signFlipReset( signFlipReset ), kV( kV ), controller( kP, kI, kD, windupRange, signFlipReset ) {}

float VelocityPID::feedforward( float target ) {
    // std::cout << "target: " << target << " kV: " << kV << " feedforward: " << target / kV << std::endl;
    return target / kV;
}

float VelocityPID::update( float target, float current ) {
    float error = target - current;
    return controller.update(error) + feedforward( target );
}

void VelocityPID::reset() {
    controller.reset();
}


    