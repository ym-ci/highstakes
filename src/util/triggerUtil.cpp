// The trigger class takes 2 functions as perameters, one the "event" will be a boolean supplier and the other will be a void function. The trigger class will call the void function when the boolean supplier returns true.
// The class will also have modes of operatopn: Rising edge, Falling edge, Toggle, and Repeat. There will be a ENUM for this.
// Repeat will call the void function every time the boolean supplier returns true.
// Rising edge will call the void function every time the boolean supplier returns true and the last time it was called the boolean supplier returned false.
// Falling edge will call the void function every time the boolean supplier returns false and the last time it was called the boolean supplier returned true.
// Make toggle another class as it is useful elsewhere.

#include <functional>
#include "triggerUtil.hpp"

// Trigger class that takes two functions as parameters
// class Trigger {
// private:
//     std::function<bool()> event;
//     std::function<void()> action;
//     TriggerMode mode;
//     bool lastEventState;

// public:
//     Trigger(std::function<bool()> event, std::function<void()> action, TriggerMode mode) 
//         : event(event), action(action), mode(mode), lastEventState(event()) {}

//     void update() {
//         bool currentEventState = event();

//         switch (mode) {
//             case TriggerMode::RISING_EDGE:
//                 if (currentEventState && !lastEventState) {
//                     action();
//                 }
//                 break;
//             case TriggerMode::FALLING_EDGE:
//                 if (!currentEventState && lastEventState) {
//                     action();
//                 }
//                 break;
//             case TriggerMode::REPEAT:
//                 if (currentEventState) {
//                     action();
//                 }
//                 break;
//         }

//         lastEventState = currentEventState;
//     }
// };

Trigger::Trigger(std::function<bool()> event, std::function<void()> action, TriggerMode mode)
    : event(event), action(action), mode(mode), lastEventState(event()) {}

void Trigger::update() {
    bool currentEventState = event();

    switch (mode) {
        case TriggerMode::RISING_EDGE:
            if (currentEventState && !lastEventState) {
                action();
            }
            break;
        case TriggerMode::FALLING_EDGE:
            if (!currentEventState && lastEventState) {
                action();
            }
            break;
        case TriggerMode::REPEAT:
            if (currentEventState) {
                action();
            }
            break;
    }

    lastEventState = currentEventState;
}




// class Toggle : public Trigger {
// private:
//         bool state;

//     public:
//         Toggle(std::function<bool()> event, TriggerMode mode)
//             : Trigger(event, [this]() { toggle(); }, mode), state(false) {}

//         bool toggle() {
//             state = !state;
//             return state;
//         }

//         bool getState() {
//             update();
//             return state;
//         }
// };

Toggle::Toggle(std::function<bool()> event, TriggerMode mode)
    : Trigger(event, [this]() { toggle(); }, mode), state(false) {}

bool Toggle::toggle() {
    state = !state;
    return state;
}

bool Toggle::getState() {
    update();
    return state;
}

