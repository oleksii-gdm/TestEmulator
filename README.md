# Emulator Implementations

## Overview

This project provides two different implementations of an emulator for interacting with `PromptProgramm`. The emulator automates the interaction by either running the program within a pseudo-terminal (PTY) or using UNIX sockets for communication.

## Emulator Implementations

1. **Forkpty-based Emulator**: This approach runs `PromptProgramm` in a forked process using a pseudo-terminal (PTY), allowing direct interaction.
2. **Socket-based Emulator**: This method runs `PromptProgramm` and the emulator **independently**, using UNIX sockets for communication.

**Note:** The socket-based implementation requires `socat`. To install it, run:

```sh
sudo apt update && sudo apt install -y socat
```

## How It Works

### Forkpty-based Emulator

1. **Create a pseudo-terminal**: The emulator calls `forkpty()`, which creates a new PTY and forks the process.
2. **Execute PromptProgramm**: In the child process, `execl()` replaces the process with `PromptProgramm`.
3. **Read program output**: The parent process reads output from the pseudo-terminal.
4. **Detect input prompts**: The emulator scans the output for specific prompts (e.g., "Please enter Your Name:").
5. **Send input to PromptProgramm**: Once a prompt is detected, the emulator sends the appropriate input (name and age).
6. **Continue reading output**: The emulator keeps reading until `PromptProgramm` terminates.
7. **Cleanup**: After `PromptProgramm` exits, the emulator ensures all resources are released.

### Socket-based Emulator

1. **Start PromptProgramm with a UNIX socket**: The emulator uses `socat` to create a socket connection to `PromptProgramm`.
2. **Connect to the socket**: The emulator establishes a connection to the socket.
3. **Read program output**: The emulator listens for output from `PromptProgramm`.
4. **Detect input prompts**: It scans the output for specific prompts (e.g., "Please enter Your Name:").
5. **Send input to PromptProgramm**: Once a prompt is detected, the emulator sends the appropriate input (name and age).
6. **Read remaining output**: The emulator captures and displays the final output.
7. **Cleanup**: The emulator ensures the socket is closed properly.

## Usage

### Running the Emulator

#### Forkpty-based Emulator

```sh
./EmulatorFork <Name> <Age>
```

Example:

```sh
./EmulatorFork John 30
```

#### Socket-based Emulator

Run the emulator using the provided bash script:

```sh
./run_emulator_socket.sh <Name> <Age>
```

Example:

```sh
./run_emulator_socket.sh John 30
```

## Advantages

### Forkpty-based Emulator

- **Full terminal emulation**: Allows `PromptProgramm` to run as if it were in a real terminal.
- **Simpler interaction**: No need for pipes or sockets.
- **Direct process control**: The emulator can manage `PromptProgramm` execution directly.

### Socket-based Emulator

- **Separation of processes**: `PromptProgramm` and the emulator run independently.
- **Network compatibility**: Can be extended for remote execution.
- **More flexible**: Can interact with multiple clients.

## Conclusion

This project provides two approaches for automating interaction with `PromptProgramm`. The `forkpty` method ensures a direct, terminal-like experience, while the socket-based method offers greater flexibility and modularity. Both approaches facilitate testing and automation for applications requiring interactive input.
