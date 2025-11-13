#!/bin/bash

# --- Configuration ---
EXECUTABLE="./test_suite"
OUTPUT_FILE="profile_output_$(date +%Y%m%d_%H%M%S).txt"
# INCREASED DURATION to ensure enough time for sampling all tests
SAMPLE_DURATION_SECONDS=40 

echo "--- BigInt Profiling Automation (macOS Sample) ---"

# 1. Check if the executable exists
if [ ! -x "$EXECUTABLE" ]; then
    echo "Error: Executable '$EXECUTABLE' not found or is not executable."
    echo "Please run 'make test' first to build the test_suite."
    exit 1
fi

# 2. Run the test suite in the background
echo "Starting test suite in background (expected runtime > 30s)..."
# Removed 'sleep 1' to allow immediate sampler attachment
"$EXECUTABLE" &
PID=$!
echo "Process ID (PID) for the test suite: $PID"

# 3. Attach the 'sample' profiler to the process
echo "Starting 'sample' for $SAMPLE_DURATION_SECONDS seconds to capture data..."
/usr/bin/sample "$PID" "$SAMPLE_DURATION_SECONDS" -file "$OUTPUT_FILE"

# Check the exit status of the sample command
if [ $? -eq 0 ]; then
    echo "Profiling complete."
else
    echo "Warning: 'sample' command encountered an issue (Exit code $?). The process may have terminated early."
fi

# 4. Clean up the background process
echo "Terminating process $PID and cleaning up..."
# Use -9 (SIGKILL) to ensure termination if it's still running
kill -9 "$PID" 2>/dev/null 
wait "$PID" 2>/dev/null

echo "--- Analysis Ready ---"
echo "Results saved to: $OUTPUT_FILE"

# 5. Provide instructions on viewing the file
echo "To view the analysis in your terminal:"
echo "less $OUTPUT_FILE"
echo "----------------------"
