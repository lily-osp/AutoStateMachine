#include <AutoStateMachine.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

AutoStateMachine<5, 10> fsm; // 5 states, 10 transitions

// Event IDs
enum Events {
    TEMP_HIGH = 1,
    TEMP_LOW = 2,
    MANUAL_OVERRIDE = 3,
    TIMEOUT = 4
};

// Shared data structure
struct ThermostatData {
    float currentTemp;
    float targetTemp;
    bool overrideActive;
};

ThermostatData thermostat;

// State functions with entry/exit actions
void idleState()
{
    // Normal operation - check temp periodically
    static unsigned long lastCheck = 0;
    if (millis() - lastCheck > 2000) {
        thermostat.currentTemp = dht.readTemperature();
        lastCheck = millis();

        if (thermostat.currentTemp > thermostat.targetTemp + 1.0) {
            fsm.handleEvent(TEMP_HIGH);
        } else if (thermostat.currentTemp < thermostat.targetTemp - 1.0) {
            fsm.handleEvent(TEMP_LOW);
        }
    }
}

void idleEnter()
{
    Serial.println("Entering idle mode");
    thermostat.overrideActive = false;
}

void coolingState()
{
    digitalWrite(8, HIGH); // AC on
}

void coolingEnter()
{
    Serial.println("Cooling activated");
}

void coolingExit()
{
    digitalWrite(8, LOW); // AC off
}

void heatingState()
{
    digitalWrite(9, HIGH); // Heater on
}

void heatingEnter()
{
    Serial.println("Heating activated");
}

void heatingExit()
{
    digitalWrite(9, LOW); // Heater off
}

void overrideState()
{
    // Manual override - maintain current state until timeout
}

void overrideEnter()
{
    Serial.println("Manual override active");
    thermostat.overrideActive = true;
}

bool checkOverrideCondition()
{
    // Only allow override if temp is within safe range
    return thermostat.currentTemp > 15.0 && thermostat.currentTemp < 30.0;
}

void setup()
{
    Serial.begin(9600);
    dht.begin();

    pinMode(8, OUTPUT); // AC control
    pinMode(9, OUTPUT); // Heater control
    pinMode(2, INPUT_PULLUP); // Override button

    thermostat.targetTemp = 22.0; // Default target

    // Add states
    fsm.addState(0, idleState, idleEnter); // Idle
    fsm.addState(1, coolingState, coolingEnter, coolingExit); // Cooling
    fsm.addState(2, heatingState, heatingEnter, heatingExit); // Heating
    fsm.addState(3, overrideState, overrideEnter); // Manual override

    // Set initial state
    fsm.setInitialState(0);

    // Add transitions
    fsm.addTransition(0, TEMP_HIGH, 1); // Idle → Cooling
    fsm.addTransition(0, TEMP_LOW, 2); // Idle → Heating
    fsm.addTransition(1, TEMP_LOW, 0); // Cooling → Idle
    fsm.addTransition(2, TEMP_HIGH, 0); // Heating → Idle
    fsm.addTransition(0, MANUAL_OVERRIDE, 3, checkOverrideCondition); // Conditional override
    fsm.addTransition(3, TIMEOUT, 0); // Override → Idle after timeout

    // Set override timeout (5 minutes)
    fsm.setTimeout(3, 300000, 0);
}

void loop()
{
    // Check override button
    static bool lastButtonState = HIGH;
    bool buttonState = digitalRead(2);
    if (buttonState == LOW && lastButtonState == HIGH) {
        fsm.handleEvent(MANUAL_OVERRIDE);
    }
    lastButtonState = buttonState;

    // Read temperature (handled in idle state)
    fsm.update();

    // Add delay to prevent flooding the serial monitor
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 5000) {
        Serial.print("Current temp: ");
        Serial.println(thermostat.currentTemp);
        lastPrint = millis();
    }
}
