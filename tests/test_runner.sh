#!/usr/bin/env bash
bin="$1"        # ./pipex
indir="$2"      # tests/input
outdir="$3"     # tests/output

fail=0
for in_file in "$indir"/*.in; do
    base=$(basename "$in_file" .in)
    exp="$indir/$base.exp"
    out="$outdir/$base.out"

    eval "$bin $(cat "$in_file")" >"$out" 2>&1 # eval is a workarround to parse double quotes
    if diff -u "$exp" "$out"; then
        printf '✅  %s\n' "$base"
    else
        printf '❌  %s\n' "$base"; fail=1
    fi
done
exit $fail