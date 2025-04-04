#ifndef AUTOSTATEMACHINE_H
#define AUTOSTATEMACHINE_H

#include <Arduino.h>

// Define callback function pointer types
typedef void (*StateFunction)();
typedef void (*TransitionCallback)();
typedef bool (*TransitionCondition)();

template <uint8_t MAX_STATES = 10, uint8_t MAX_TRANSITIONS = 20>
class AutoStateMachine {
public:
    AutoStateMachine();

    // State management
    bool addState(uint8_t id, StateFunction func,
        TransitionCallback onEnter = nullptr,
        TransitionCallback onExit = nullptr);
    void setInitialState(uint8_t id);

    // State machine control
    void transitionTo(uint8_t id);
    bool handleEvent(uint8_t eventId, void* eventData = nullptr);
    void run();
    void update();
    void reset();

    // Transition configuration
    bool addTransition(uint8_t fromState, uint8_t eventId, uint8_t toState,
        TransitionCondition condition = nullptr);
    void setTimeout(uint8_t stateId, unsigned long timeoutMs, uint8_t timeoutState);

    // Debugging
    void enableDebug(Print& debugStream);
    void disableDebug();

    // State information
    uint8_t getCurrentState() const;
    bool stateExists(uint8_t id) const;

private:
    struct State {
        uint8_t id;
        StateFunction func;
        TransitionCallback onEnter;
        TransitionCallback onExit;
        unsigned long timeoutMs;
        uint8_t timeoutState;
    };

    struct Transition {
        uint8_t fromState;
        uint8_t eventId;
        uint8_t toState;
        TransitionCondition condition;
    };

    State states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    uint8_t stateCount;
    uint8_t transitionCount;
    uint8_t currentStateId;
    uint8_t initialStateId;
    unsigned long stateEnterTime;

    Print* debugStream;
    void* currentEventData;

    void log(const char* message);
    void callStateCallback(TransitionCallback callback);
    bool executeTransition(const Transition& transition);
};

#include "AutoStateMachine.tpp"

#endif // AUTOSTATEMACHINE_H
