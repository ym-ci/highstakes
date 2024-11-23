#pragma once

namespace Robot {

/**
 * @brief The Latch class represents a latching mechanism.
 */
class Doinker {
 public:
   /**
    * @brief Runs latching mechanism.
    */
   void run();

   Doinker();

   /**
    * @brief Toggles latch state.
    */
   void toggle();

   bool getState();
};
} // namespace Robot