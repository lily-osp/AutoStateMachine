# AutoStateMachine Library

## Overview

The AutoStateMachine library simplifies the implementation and management of state machines in Arduino projects. A state machine is a computational model that transitions between a finite number of states based on predefined rules or inputs. This library provides an intuitive interface for creating, transitioning, and executing states, making it ideal for applications such as robotics, automation, and control systems.

## What is a State Machine?

A **state machine** is a conceptual model used to design systems that can exist in one of a finite number of states at any given time. Each state represents a specific condition or behavior, and the system transitions between these states based on events, inputs, or conditions.

State machines are commonly used in:

- Robotics: To control the behavior of robots based on sensor inputs.
- Automation: To manage workflows or processes.
- User Interfaces: To handle UI interactions and navigation logic.
- Game Development: To control game logic, AI behavior, or animations.

### Components of a State Machine

1. **States**: The distinct conditions or behaviors of the system (e.g., "Idle", "Processing", "Error").
2. **Events**: The triggers that cause the system to transition from one state to another (e.g., a button press, a timer expiration).
3. **Transitions**: The rules defining how the system moves from one state to another based on events or conditions.
4. **Actions**: The tasks or operations performed when entering, exiting, or within a state.

## Mathematical Explanation

A state machine can be formally defined as a **finite state automaton** (FSA) represented by the tuple:

$M = (Q, \Sigma, \delta, q_0, F)$

Where:

- **$Q$**: A finite set of states $ \{q_1, q_2, \dots, q_n\} $.
- **$\Sigma$**: A finite set of input symbols (the alphabet).
- **$\delta$**: A transition function $\delta: Q \times \Sigma \to Q$ that maps a state and input symbol to another state.
- **$q_0$**: The initial state, where $q_0 \in Q$.
- **$F$**: The set of final or accepting states, where $F \subseteq Q$.

### Transition Function

The transition function $\delta$ defines how the system moves between states based on inputs. For example, if the system is in state $q_1$ and receives input $a$, it transitions to state $q_2$:

$\delta(q_1, a) = q_2$

### Extended Transition Function

For a sequence of inputs $w = a_1a_2\dots a_n$, the extended transition function $\hat{\delta}$ can be defined recursively:

1. $\hat{\delta}(q, \varepsilon) = q$, where $\varepsilon$ is the empty string.
2. $\hat{\delta}(q, xa) = \delta(\hat{\delta}(q, x), a)$, where $x$ is a string and $a$ is a single input symbol.

### Deterministic vs. Non-Deterministic State Machines

- **Deterministic Finite Automata (DFA)**: Each state has exactly one transition for each input symbol.
- **Non-Deterministic Finite Automata (NFA)**: A state can have multiple transitions for a single input symbol, or even transitions without input ($\varepsilon$-transitions).

The AutoStateMachine library implements a deterministic model, where each state has one and only one transition for a given condition or event.

## Features of AutoStateMachine Library

- **Simple State Management**: Add, define, and manage states with ease.
- **Conditional Transitions**: Transition between states based on custom conditions.
- **Hierarchical State Machines**: Support for nested or sub-state machines.
- **Lightweight and Efficient**: Designed to work on resource-constrained Arduino platforms.
- **Flexibility**: Suitable for basic to advanced use cases, from simple toggling to complex workflows.

## Installation

1. Download the AutoStateMachine library.
2. Open the Arduino IDE.
3. Navigate to **Sketch > Include Library > Add .ZIP Library**.
4. Select the downloaded library .ZIP file.

## Getting Started

Here are the basic steps to create a state machine with AutoStateMachine:

1. **Include the Library**:
   
   ```cpp
   #include <AutoStateMachine.h>
   ```

2. **Declare the State Machine**:
   
   ```cpp
   AutoStateMachine stateMachine;
   ```

3. **Define State Functions**:
   Create functions to define the behavior for each state.
   
   ```cpp
   void stateOne() {
       // Actions for state one
   }
   void stateTwo() {
       // Actions for state two
   }
   ```

4. **Add States**:
   Register each state with a unique ID and associated function.
   
   ```cpp
   stateMachine.addState(1, stateOne);
   stateMachine.addState(2, stateTwo);
   ```

5. **Set the Initial State**:
   Define the starting state for the machine.
   
   ```cpp
   stateMachine.setInitialState(1);
   ```

6. **Run the State Machine**:
   Call the `run` method in the main loop to execute the current state.
   
   ```cpp
   void loop() {
       stateMachine.run();
   }
   ```

## Examples

### Basic Example

A simple two-state machine alternates between two states every second. See the "examples" folder for the full code.

### Intermediate Example

A state machine transitions based on a counter value. This demonstrates conditional transitions and dynamic behavior.

### Advanced Example

A hierarchical state machine where a main state machine manages a sub-state machine. This approach is useful for modular and scalable designs.

## API Reference

### `AutoStateMachine()`

Constructor to initialize the state machine.

### `void addState(uint8_t id, StateFunction func)`

Adds a state to the state machine.

- **`id`**: Unique identifier for the state.
- **`func`**: Function to execute when the state is active.

### `void setInitialState(uint8_t id)`

Sets the initial state of the state machine.

- **`id`**: Identifier of the initial state.

### `void transitionTo(uint8_t id)`

Transitions the state machine to the specified state.

- **`id`**: Identifier of the target state.

### `void run()`

Executes the current state function.

## Applications

The AutoStateMachine library can be applied to a variety of scenarios, including:

- **Robotics**: Implementing behavior control based on sensor inputs.
- **IoT Devices**: Managing device states based on network events or user inputs.
- **Games**: Handling AI or game logic with finite state machines.
- **Automation Systems**: Controlling workflows and processes efficiently.

## Contributing

Contributions are welcome! If you find bugs or want to propose new features, please create an issue or submit a pull request.

## License

This library is licensed under the MIT License. See the LICENSE file for details.

---

With AutoStateMachine, you can easily create robust and scalable state machines for your Arduino projects. Whether you're building a simple toggle system or a complex automation workflow, this library has you covered. Happy coding!
