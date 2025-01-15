#pragma once

namespace Robot {

/**
 * @brief The Intake class represents a robot intake system.
 */
class Intake {
 public:
   /**
    * @brief Runs the main function of the intake system.
    *
    * Takes optional user input to control the direction of the intake system in
    * autonomous.
    */
   void run();

   Intake();

   /**
    * @brief Toggles intake elevation.
    */
   void toggle();

 private:
   bool elevated;
};
} // namespace Robot