bin="$1"
cases=( "tests/input/"*.in )
for in_file in "${cases[@]}"; do
    args=$(cat "$in_file")
    log="tests/valgrind/$(basename "$in_file" .in).log"

    valgrind --leak-check=full --error-exitcode=42 \
             --log-file="$log" $bin $args || exit 1

    # (Valgrind already exits 42 on error, so reaching here means PASS)
    printf '✅  %s\n' "$in_file"
done