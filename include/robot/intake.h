#pragma once

#include <cstdint>

namespace Robot {

/**
 * @brief The Intake class represents a robot intake system.
 */
class Intake {
 public:
   Intake();
   
   /**
    * @brief Runs the main function of the intake system.
    */
   void run();

   // Basic motor control methods
   void moveConveyor(int velocity);
   void moveIntake(int velocity);
   void stopIntake();
   void stopConveyor();
   void stopAll();

   // Combined movement methods
   void moveAll(int intakeVelocity, int conveyorVelocity);
   void moveAll();
   void reverseAll();

   // Cycling methods
   void cycle();
   void longCycle();

   /**
    * @brief Toggles intake elevation.
    */
   void toggle();

   bool getFilterState();
   void setFilterState(bool state);

 private:
   bool elevated;
   double stopPosition;
   uint32_t lastWrongRingTime;
   bool filterState;
};
} // namespace Robot