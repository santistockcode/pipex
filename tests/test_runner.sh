#!/usr/bin/env bash
# Usage: test_runner.sh ./pipex tests/input tests/output

bin="$1"        # ./pipex
indir="$2"      # tests/input   (00_basic.in, 00_basic.status, 00_basic.exp)
outdir="$3"     # tests/output  (generated *.out)

mkdir -p "$outdir"
fail=0

for in_file in "$indir"/*.in; do
    base=$(basename "$in_file" .in)

    exp_file="$indir/$base.exp"       # expected stdout/stderr
    exp_status_file="$indir/$base.status"  # (optional) expected exit code
    out_file="$outdir/$base.out"

    #################################################################
    # run pipex
    #################################################################
    eval "$bin $(cat "$in_file")" >"$out_file" 2>&1
    status=$?

    #################################################################
    # compare output
    #################################################################
    if diff -u "$exp_file" "$out_file" >/dev/null; then
        out_ok=1
    else
        out_ok=0
        printf '❌  %s (output differs)\n' "$base"
        diff -u "$exp_file" "$out_file"
    fi

    #################################################################
    # compare exit status (if .status file exists)
    #################################################################
    if [[ -f "$exp_status_file" ]]; then
        exp_status=$(cat "$exp_status_file")
        if [[ "$status" -eq "$exp_status" ]]; then
            status_ok=1
        else
            status_ok=0
            printf '❌  %s (exit status %s ≠ expected %s)\n' \
                   "$base" "$status" "$exp_status"
        fi
    else
        status_ok=1    # no status file ⇒ ignore
    fi

    #################################################################
    # final verdict for this test case
    #################################################################
    if [[ $out_ok -eq 1 && $status_ok -eq 1 ]]; then
        printf '✅  %s\n' "$base"
    else
        fail=1
    fi
done

exit $fail
