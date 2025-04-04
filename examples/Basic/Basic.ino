#include <AutoStateMachine.h>

AutoStateMachine<3> fsm; // 3 states max

// State functions
void offState() { digitalWrite(LED_BUILTIN, LOW); }
void onState() { digitalWrite(LED_BUILTIN, HIGH); }

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    // Add states
    fsm.addState(0, offState); // OFF state
    fsm.addState(1, onState); // ON state

    // Set initial state
    fsm.setInitialState(0);

    // Set timeout transitions
    fsm.setTimeout(0, 500, 1); // After 500ms OFF, go to ON
    fsm.setTimeout(1, 500, 0); // After 500ms ON, go to OFF
}

void loop()
{
    fsm.update(); // Handles state timeouts and runs current state
}
