#!/bin/bash
# Simple timeout script
TIMEOUT=30
COMMAND="$@"

# Run in background
$COMMAND &
PID=$!

# Wait for timeout
( sleep $TIMEOUT && kill -9 $PID 2>/dev/null ) &
KILLER=$!

# Wait for process to finish
wait $PID 2>/dev/null
EXIT_CODE=$?

# Kill the killer if process finished
kill -9 $KILLER 2>/dev/null

exit $EXIT_CODE
