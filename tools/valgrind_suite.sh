#!/usr/bin/env bash



# Color codes
BLUE="\033[34m"
YELLOW="\033[93m"
GREEN="\033[32m"
RED="\033[31m"
RESET="\033[0m"


bin="$1"
mkdir -p tests/valgrind
fail=0

for in_file in tests/input/*.in; do
    base=$(basename "$in_file" .in)
    log="tests/valgrind/${base}.log"

    # build the command line as one string, then let *one* eval parse it
    cmd="$bin $(cat "$in_file") > /dev/null 2>&1"
    eval "valgrind --leak-check=full --trace-children=yes --log-file=\"$log\" $cmd" || fail=1


    # printf '✅ Executed -  %s\n' "$cmd"
    # printf 'Logfile -  %s\n' "$log"
    if grep -q "All heap blocks were freed -- no leaks are possible" "$log" &&
           grep -q "ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)" "$log"; then
           echo "OK"
        else
            echo -e "${RED}Memory leak detected!${RESET}"
            echo -e "${YELLOW}Failing ARG:${RESET} $ARG"
            echo "KO See: $LOG_FILE"
        fi

done

echo -e "${YELLOW}\nAll valgrind shit done!${RESET}"