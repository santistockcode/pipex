#!/bin/bash

PIPEX_BIN=$1
INPUT_DIR=$2
OUTPUT_DIR=$3

# Test case: invalid input file with "grep hello | sleep 3"
INFILE="$INPUT_DIR/example"
OUTFILE_PIPEX="$OUTPUT_DIR/pipex_output.txt"
OUTFILE_SHELL="$OUTPUT_DIR/shell_output.txt"

CMD1="ls"
CMD2="wc"

# Run your pipex program
$PIPEX_BIN "$INFILE" "$CMD1" "$CMD2" "$OUTFILE_PIPEX" 2>/dev/null

# Run shell equivalent
< "$INFILE" ls | wc > "$OUTFILE_SHELL" 2>/dev/null

# Compare outputs
if diff "$OUTFILE_PIPEX" "$OUTFILE_SHELL" > /dev/null; then
    echo -e "\033[1;32m[OK]\033[0m Output matches shell behavior."
else
    echo -e "\033[1;31m[FAIL]\033[0m Output differs from shell behavior."
    echo "Diff:"
    diff "$OUTFILE_PIPEX" "$OUTFILE_SHELL"
fi
