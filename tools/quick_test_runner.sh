#!/bin/bash

PIPEX_BIN=$1
CMD1=$2
CMD2=$3


# Test case: invalid input file with "grep hello | sleep 3"
INFILE="example"
OUTFILE_PIPEX="pipex_output.txt"
OUTFILE_SHELL="shell_output.txt"

# Run your pipex program
$PIPEX_BIN "$INFILE" "$CMD1" "$CMD2" "$OUTFILE_PIPEX" 2>/dev/null

# Run shell equivalent
eval "< \"$INFILE\" $CMD1 | $CMD2 > \"$OUTFILE_SHELL\" 2>/dev/null"

if diff -q "$OUTFILE_PIPEX" "$OUTFILE_SHELL" >/dev/null; then
    echo -e "\e[1;32m[OK]\e[0m Output matches shell behaviour."
else
    echo -e "\e[1;31m[FAIL]\e[0m Output differs:"
    diff "$OUTFILE_PIPEX" "$OUTFILE_SHELL"
fi
