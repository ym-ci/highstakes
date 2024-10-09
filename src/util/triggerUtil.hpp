#ifndef TRIGGER_UTIL_H
#define TRIGGER_UTIL_H

#include <functional>

// Enum for different trigger modes
enum class TriggerMode {
    RISING_EDGE,
    FALLING_EDGE,
    REPEAT
};

// Trigger class that takes two functions as parameters
class Trigger {
private:
    std::function<bool()> event;
    std::function<void()> action;
    TriggerMode mode;
    bool lastEventState;

public:
    Trigger(std::function<bool()> event, std::function<void()> action, TriggerMode mode);

    void update();
};


class Toggle : public Trigger {
private:
    bool state = false;

public:
    Toggle(std::function<bool()> event, TriggerMode mode);

    bool toggle();

    bool getState();
};


#endif // TRIGGER_UTIL_H