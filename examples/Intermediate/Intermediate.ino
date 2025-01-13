#include <AutoStateMachine.h>

AutoStateMachine stateMachine;
int counter = 0;

void stateA()
{
    Serial.println("State A");
    delay(500);
    counter++;
    if (counter >= 5) {
        stateMachine.transitionTo(2);
        counter = 0;
    }
}

void stateB()
{
    Serial.println("State B");
    delay(500);
    stateMachine.transitionTo(1);
}

void setup()
{
    Serial.begin(9600);
    stateMachine.addState(1, stateA);
    stateMachine.addState(2, stateB);
    stateMachine.setInitialState(1);
}

void loop()
{
    stateMachine.run();
}
