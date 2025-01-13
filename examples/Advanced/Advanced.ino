#include <AutoStateMachine.h>

AutoStateMachine mainStateMachine;
AutoStateMachine subStateMachine;

void subState1()
{
    Serial.println("Sub-State 1");
    delay(1000);
    subStateMachine.transitionTo(2);
}

void subState2()
{
    Serial.println("Sub-State 2");
    delay(1000);
    subStateMachine.transitionTo(1);
}

void mainState1()
{
    Serial.println("Main State 1");
    subStateMachine.run(); // Run the sub-state machine
}

void mainState2()
{
    Serial.println("Main State 2");
    delay(2000);
    mainStateMachine.transitionTo(1);
}

void setup()
{
    Serial.begin(9600);

    // Configure sub-state machine
    subStateMachine.addState(1, subState1);
    subStateMachine.addState(2, subState2);
    subStateMachine.setInitialState(1);

    // Configure main state machine
    mainStateMachine.addState(1, mainState1);
    mainStateMachine.addState(2, mainState2);
    mainStateMachine.setInitialState(1);
}

void loop()
{
    mainStateMachine.run();
}
