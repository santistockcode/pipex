#!/usr/bin/env bash
bin="$1"
mkdir -p tests/valgrind
fail=0

for in_file in tests/input/*.in; do
    base=$(basename "$in_file" .in)
    log="tests/valgrind/${base}.log"

    # build the command line as one string, then let *one* eval parse it
    cmd="$bin $(cat "$in_file")"
    eval "valgrind --leak-check=full \
          --track-origins=yes --error-exitcode=42 --trace-children=yes \
          --log-file=\"$log\" $cmd" || fail=1

    if [[ $fail -eq 0 ]]; then
        printf '✅  %s\n' "$base"
    else
        printf '❌  %s (see %s)\n' "$base" "$log"
    fi
done

exit $fail
