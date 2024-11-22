#include "lemlib/pid.hpp"
#include <cstdint>

namespace Robot {

class VelocityPID {
private:
    const float kP;
    const float kI;
    const float kD;
    const float windupRange;
    const bool signFlipReset;
    const float kV;
    lemlib::PID controller;

    uint16_t feedforward( float target );

public:
    VelocityPID( float kP, float kI, float kD, float windupRange = 0, bool signFlipReset = false, float kV = 0 );

    /**
     * Updates the PID controller with new target and current values
     * @param target The desired setpoint value
     * @param current The current measured value
     * @return Output in volts after PID calculation
     */
    float update( float target, float current );

    void reset();

};

}