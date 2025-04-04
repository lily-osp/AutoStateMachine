# AutoStateMachine Library

A robust, flexible state machine implementation for Arduino and embedded systems that supports event-driven transitions, timed states, and hierarchical state management.

## Features

- **Template-based configuration** - Set maximum states and transitions at compile time
- **Multiple transition types** - Event-driven, timed, and conditional transitions
- **State lifecycle hooks** - Entry and exit callbacks for states
- **Event system** - Handle external events with optional data payload
- **Timeouts** - Automatic state transitions after specified durations
- **Debug support** - Optional verbose logging via Serial or other Print-compatible output
- **Memory efficient** - No dynamic allocation, fixed memory footprint

## Installation

1. Download the latest release from GitHub
2. Install via Arduino Library Manager (search for "AutoStateMachine")
3. Or manually install by placing in your Arduino/libraries folder

## API Reference

### State Machine Configuration

#### `AutoStateMachine<MAX_STATES, MAX_TRANSITIONS>`
- Template parameters:
  - `MAX_STATES`: Maximum number of states (default: 10)
  - `MAX_TRANSITIONS`: Maximum number of transitions (default: 20)

### State Management

#### `bool addState(uint8_t id, StateFunction func, TransitionCallback onEnter = nullptr, TransitionCallback onExit = nullptr)`
- Adds a new state to the state machine
- Parameters:
  - `id`: Unique state identifier
  - `func`: Function called repeatedly while in this state
  - `onEnter`: Optional callback when entering state
  - `onExit`: Optional callback when exiting state
- Returns: true if state was added successfully

#### `void setInitialState(uint8_t id)`
- Sets the initial state for the state machine
- Must be called before running the state machine

### Transition Management

#### `bool addTransition(uint8_t fromState, uint8_t eventId, uint8_t toState, TransitionCondition condition = nullptr)`
- Adds an event-triggered transition between states
- Parameters:
  - `fromState`: Source state ID
  - `eventId`: Event identifier that triggers this transition
  - `toState`: Destination state ID
  - `condition`: Optional condition function that must return true for transition to occur
- Returns: true if transition was added successfully

#### `void setTimeout(uint8_t stateId, unsigned long timeoutMs, uint8_t timeoutState)`
- Configures a timeout transition for a state
- Parameters:
  - `stateId`: State to apply timeout to
  - `timeoutMs`: Duration in milliseconds before automatic transition
  - `timeoutState`: State to transition to after timeout

### Event Handling

#### `bool handleEvent(uint8_t eventId, void* eventData = nullptr)`
- Processes an event through the state machine
- Parameters:
  - `eventId`: Event identifier
  - `eventData`: Optional pointer to event-specific data
- Returns: true if event triggered a state transition

### State Machine Control

#### `void run()`
- Executes the current state's function
- Should be called frequently in main loop

#### `void update()`
- Handles timeouts and executes current state
- Combines timeout checking and state execution

#### `void reset()`
- Returns the state machine to its initial state
- Calls exit/enter callbacks as appropriate

### Debugging

#### `void enableDebug(Print& debugStream)`
- Enables debug output
- Parameters:
  - `debugStream`: Print-compatible output stream (e.g., Serial)

#### `void disableDebug()`
- Disables debug output

### State Information

#### `uint8_t getCurrentState() const`
- Returns: Current state ID

#### `bool stateExists(uint8_t id) const`
- Checks if a state exists
- Returns: true if state exists

## Usage Pattern

1. Create state machine instance with desired template parameters
2. Add all states with their functions and optional callbacks
3. Configure transitions (event-based and timeout-based)
4. Set initial state
5. In main loop:
   - Call `handleEvent()` for any events
   - Call `update()` to handle timeouts and run current state

## Best Practices

- Keep state functions short and non-blocking
- Use event data pointers carefully (ensure proper lifetime)
- For complex conditions, use separate condition functions
- Enable debug during development
- Consider state timeouts for fault recovery
- Document your state/event IDs with enums

## Examples

See the `examples/` folder for complete usage examples:
1. `Basic/` - Simple LED blinker
2. `Intermediate/` - Traffic light controller
3. `Advanced/` - Thermostat with temperature control

## Limitations

- Fixed maximum states/transitions (set at compile time)
- No hierarchical state support
- No built-in serialization for state persistence

## License

MIT License - Free for personal and commercial use
