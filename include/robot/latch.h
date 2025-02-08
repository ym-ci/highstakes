#pragma once

namespace Robot {

/**
 * @brief The Latch class represents a latching mechanism.
 */
class Latch {
 public:
   Latch();
   /**
    * @brief Runs latching mechanism.
    */
   void run();

   /**
    * @brief Toggles latch state.
    */
   void toggle();

    bool getState();
    void extend();
  void retract();
};
} // namespace Robot