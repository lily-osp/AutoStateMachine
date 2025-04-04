#include <AutoStateMachine.h>

AutoStateMachine<4, 5> fsm; // 4 states, 5 transitions max

// Pin definitions
const int RED_PIN = 8;
const int YELLOW_PIN = 9;
const int GREEN_PIN = 10;
const int BUTTON_PIN = 2;

// Event IDs
enum Events { BUTTON_PRESS = 1 };

// State functions
void redState()
{
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
}

void redYellowState()
{
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
}

void greenState()
{
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
}

void yellowState()
{
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
}

void buttonISR()
{
    fsm.handleEvent(BUTTON_PRESS);
}

void setup()
{
    // Setup pins
    pinMode(RED_PIN, OUTPUT);
    pinMode(YELLOW_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);

    // Add states
    fsm.addState(0, redState); // Red
    fsm.addState(1, redYellowState); // Red + Yellow
    fsm.addState(2, greenState); // Green
    fsm.addState(3, yellowState); // Yellow

    // Set initial state
    fsm.setInitialState(0);

    // Set timeout transitions
    fsm.setTimeout(0, 3000, 1); // Red for 3s → Red+Yellow
    fsm.setTimeout(1, 1000, 2); // Red+Yellow for 1s → Green
    fsm.setTimeout(2, 5000, 3); // Green for 5s → Yellow
    fsm.setTimeout(3, 2000, 0); // Yellow for 2s → Red

    // Add button press transition (skips to next phase)
    fsm.addTransition(2, BUTTON_PRESS, 3); // Green → Yellow on button press
}

void loop()
{
    fsm.update();
}
