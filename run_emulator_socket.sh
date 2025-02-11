#!/bin/bash

# Define UNIX socket path
SOCKET_PATH="/tmp/prompt_socket"
PROMPT_PROGRAM="./build/PromptProgramm"
EMULATOR="./build/EmulatorSocket"

# Kill previous instances and remove socket
pkill -f PromptProgramm 2>/dev/null
rm -f "$SOCKET_PATH"

# Check if arguments are provided
if [[ $# -ne 2 ]]; then
    echo "ERROR: Name and age are required! Usage: $0 <name> <age>"
    exit 1
fi

NAME="$1"
AGE="$2"

# Ensure required programs exist
if [[ ! -x "$PROMPT_PROGRAM" || ! -x "$EMULATOR" ]]; then
    exit 1
fi

# Start UNIX socket server (Suppress socat logs)
socat UNIX-LISTEN:"$SOCKET_PATH",fork,reuseaddr,unlink-early EXEC:"stdbuf -oL $PROMPT_PROGRAM",pty,echo=0,raw >/dev/null 2>&1 &

# Store process ID and wait for startup
PROMPT_PID=$!
sleep 0.5

# Run EmulatorSocket (Only output PromptProgramm responses)
"$EMULATOR" "$SOCKET_PATH" "$NAME" "$AGE"

# Cleanup
kill $PROMPT_PID 2>/dev/null
wait $PROMPT_PID 2>/dev/null
rm -f "$SOCKET_PATH"
