#ifndef AUTOSTATEMACHINE_H
#define AUTOSTATEMACHINE_H

#include <Arduino.h>

// Define function pointer type for state functions
typedef void (*StateFunction)();

class AutoStateMachine {
public:
    AutoStateMachine();

    void addState(uint8_t id, StateFunction func); // Add a state
    void setInitialState(uint8_t id); // Set the initial state
    void transitionTo(uint8_t id); // Transition to another state
    void run(); // Execute the current state

private:
    struct State {
        uint8_t id;
        StateFunction func;
    };

    static const uint8_t MAX_STATES = 10; // Maximum number of states
    State states[MAX_STATES];
    uint8_t stateCount;
    uint8_t currentStateId;
};

#endif // AUTOSTATEMACHINE_H
