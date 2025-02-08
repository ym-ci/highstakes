#pragma once

namespace Robot {

/**
 * @brief The Intake class represents a robot intake system.
 */
class Intake {
 public:
   Intake();
   /**
    * @brief Runs the main function of the intake system.
    *
    * Takes optional user input to control the direction of the intake system in
    * autonomous.
    */
   void run();

   /**
    * @brief Toggles intake elevation.
    */
   void toggle();

   // Added declarations for additional functionalities
   void moveConveyor(int velocity);
   void moveIntake(int velocity);
   void stopIntake();
   void stopConveyor();

   // New declarations for the added methods
   void moveAll(int intakeVelocity, int conveyorVelocity);
   void moveAll();
   void reverseAll();
   void stopAll();
   // Added declaration for the cycle method.
   void cycle();

 private:
   bool elevated;
};
} // namespace Robot