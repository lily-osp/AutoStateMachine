#include "AutoStateMachine.h"

AutoStateMachine::AutoStateMachine()
    : stateCount(0)
    , currentStateId(255)
{
} // Default invalid state

void AutoStateMachine::addState(uint8_t id, StateFunction func)
{
    if (stateCount < MAX_STATES) {
        states[stateCount++] = { id, func };
    }
}

void AutoStateMachine::setInitialState(uint8_t id)
{
    currentStateId = id;
}

void AutoStateMachine::transitionTo(uint8_t id)
{
    for (uint8_t i = 0; i < stateCount; i++) {
        if (states[i].id == id) {
            currentStateId = id;
            break;
        }
    }
}

void AutoStateMachine::run()
{
    for (uint8_t i = 0; i < stateCount; i++) {
        if (states[i].id == currentStateId && states[i].func) {
            states[i].func();
            break;
        }
    }
}
