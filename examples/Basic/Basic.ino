#include <AutoStateMachine.h>

// Declare the state machine
AutoStateMachine stateMachine;

// Define state functions
void stateOne()
{
    Serial.println("State One");
    delay(1000);
    stateMachine.transitionTo(2);
}

void stateTwo()
{
    Serial.println("State Two");
    delay(1000);
    stateMachine.transitionTo(1);
}

void setup()
{
    Serial.begin(9600);

    // Add states
    stateMachine.addState(1, stateOne);
    stateMachine.addState(2, stateTwo);

    // Set initial state
    stateMachine.setInitialState(1);
}

void loop()
{
    stateMachine.run();
}
