#pragma once

namespace Robot {

/**
 * @brief The Doinker class represents a latching mechanism.
 */
class Doinker {
 public:
   Doinker();
   /**
    * @brief Runs doinker mechanism.
    */
   void run();

   /**
    * @brief Toggles doinker state.
    */
   void toggle();

    bool getState();
    void extend();
    void retract();
};
} // namespace Robot
