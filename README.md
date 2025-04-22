# Multithreading-Simulator
An implementation of a simulator for multithreaded processors in C++, supporting Fine-grained and Blocked MT configurations.

# Multithreading Simulator

## Description

The Multithreading Simulator project is designed to emulate a multithreaded processor running in two configurations: Fine-grained MT and Blocked MT. The simulator provides a detailed simulation of a multi-threaded CPU architecture, focusing on context switching, process states, and instruction execution. This project demonstrates an understanding of processor architecture, thread management, and system-level simulation.

## Technologies Used

- **Programming Languages:** C++
- **Libraries/Frameworks:** POSIX Threads (pthreads)
- **Tools:** Git for version control

## Features

- **Instruction Set:** Simulates a basic instruction set including arithmetic operations (ADD, ADDI, SUB, SUBI), memory access (LOAD, STORE), and special HALT commands.
- **Thread Management:** Implements context switching between threads for both Fine-grained MT and Blocked MT configurations.
- **Job Control:** Handles job scheduling, including context switching penalties in Blocked MT configuration.
- **Memory Management:** Supports loading and storing commands with defined latencies, simulating memory access delays.
- **Performance Metrics:** Measures performance using the CPI (Cycles Per Instruction) metric for evaluating system efficiency.

## Getting Started

### Prerequisites

- **Compiler:** GCC or any C++ compiler
- **Operating System:** Linux

### Installation

Clone the repository to your local machine:

```bash
git clone https://github.com/[YourGitHubUsername]/Multithreading-Simulator.git
cd Multithreading-Simulator
```

Compile the program:

```bash
make
```

### Usage

Run the simulator with a test file:

```bash
./main <test_file>
```

For example:

```bash
./main example1.img
```

### Example Commands

- **Execute Commands:**

    ```bash
    ./main example1.img
    ```

- **Commands in Test Files:**

    - Commands for simulation include memory operations (LOAD, STORE), arithmetic operations (ADD, SUB), and control operations (HALT).

## Contributing

Contributions to the Multithreading Simulator project are welcome! Feel free to fork the repository, make your changes, and submit a pull request with your improvements.

