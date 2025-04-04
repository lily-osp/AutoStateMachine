template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::AutoStateMachine()
    : stateCount(0)
    , transitionCount(0)
    , currentStateId(255)
    , initialStateId(255)
    , stateEnterTime(0)
    , debugStream(nullptr)
    , currentEventData(nullptr)
{
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
bool AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::addState(uint8_t id, StateFunction func,
    TransitionCallback onEnter,
    TransitionCallback onExit)
{
    if (stateCount >= MAX_STATES) {
        log("Error: Maximum number of states reached");
        return false;
    }
    if (!func) {
        log("Error: State function cannot be null");
        return false;
    }
    if (stateExists(id)) {
        log("Error: State ID already exists");
        return false;
    }

    states[stateCount] = { id, func, onEnter, onExit, 0, 255 };
    stateCount++;
    return true;
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
void AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::setInitialState(uint8_t id)
{
    if (stateExists(id)) {
        initialStateId = id;
        currentStateId = id;
        stateEnterTime = millis();
    }
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
bool AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::addTransition(uint8_t fromState, uint8_t eventId,
    uint8_t toState,
    TransitionCondition condition)
{
    if (transitionCount >= MAX_TRANSITIONS) {
        log("Error: Maximum number of transitions reached");
        return false;
    }
    if (!stateExists(fromState) || !stateExists(toState)) {
        log("Error: Invalid state in transition");
        return false;
    }

    transitions[transitionCount++] = { fromState, eventId, toState, condition };
    return true;
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
bool AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::handleEvent(uint8_t eventId, void* eventData)
{
    currentEventData = eventData;

    if (debugStream) {
        debugStream->print("Handling event: ");
        debugStream->print(eventId);
        debugStream->print(" in state: ");
        debugStream->println(currentStateId);
    }

    // Check all transitions for matches
    for (uint8_t i = 0; i < transitionCount; i++) {
        const Transition& t = transitions[i];

        if (t.fromState == currentStateId && t.eventId == eventId) {
            if (!t.condition || t.condition()) {
                if (executeTransition(t)) {
                    currentEventData = nullptr;
                    return true;
                }
            }
        }
    }

    currentEventData = nullptr;
    return false;
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
bool AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::executeTransition(const Transition& transition)
{
    if (!stateExists(transition.toState)) {
        log("Error: Transition target state does not exist");
        return false;
    }

    if (debugStream) {
        debugStream->print("Executing transition from ");
        debugStream->print(transition.fromState);
        debugStream->print(" to ");
        debugStream->println(transition.toState);
    }

    transitionTo(transition.toState);
    return true;
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
void AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::transitionTo(uint8_t id)
{
    if (!stateExists(id)) {
        log("Error: Transition target state does not exist");
        return;
    }

    // Call onExit for current state
    for (uint8_t i = 0; i < stateCount; i++) {
        if (states[i].id == currentStateId) {
            callStateCallback(states[i].onExit);
            break;
        }
    }

    currentStateId = id;
    stateEnterTime = millis();

    // Call onEnter for new state
    for (uint8_t i = 0; i < stateCount; i++) {
        if (states[i].id == currentStateId) {
            callStateCallback(states[i].onEnter);
            break;
        }
    }

    if (debugStream) {
        debugStream->print("Transitioned to state: ");
        debugStream->println(currentStateId);
    }
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
void AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::run()
{
    for (uint8_t i = 0; i < stateCount; i++) {
        if (states[i].id == currentStateId && states[i].func) {
            states[i].func();
            break;
        }
    }
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
void AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::update()
{
    // Check for timeouts
    for (uint8_t i = 0; i < stateCount; i++) {
        if (states[i].id == currentStateId && states[i].timeoutMs > 0 && (millis() - stateEnterTime) >= states[i].timeoutMs) {
            transitionTo(states[i].timeoutState);
            break;
        }
    }
    run();
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
void AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::reset()
{
    if (initialStateId != 255) {
        transitionTo(initialStateId);
    }
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
void AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::setTimeout(uint8_t stateId, unsigned long timeoutMs, uint8_t timeoutState)
{
    for (uint8_t i = 0; i < stateCount; i++) {
        if (states[i].id == stateId) {
            states[i].timeoutMs = timeoutMs;
            states[i].timeoutState = timeoutState;
            return;
        }
    }
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
uint8_t AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::getCurrentState() const
{
    return currentStateId;
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
bool AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::stateExists(uint8_t id) const
{
    for (uint8_t i = 0; i < stateCount; i++) {
        if (states[i].id == id) {
            return true;
        }
    }
    return false;
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
void AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::enableDebug(Print& debugStream)
{
    this->debugStream = &debugStream;
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
void AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::disableDebug()
{
    this->debugStream = nullptr;
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
void AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::log(const char* message)
{
    if (debugStream) {
        debugStream->print("[AutoStateMachine] ");
        debugStream->println(message);
    }
}

template <uint8_t MAX_STATES, uint8_t MAX_TRANSITIONS>
void AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>::callStateCallback(TransitionCallback callback)
{
    if (callback) {
        callback();
    }
}
