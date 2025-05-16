#!/usr/bin/env bash
set -e

IMAGE="pipex-dev"
PORT=${1:-1234}
BIN="unit_tests_dbg"

cid=$(docker ps --filter "ancestor=$IMAGE" --format "{{.ID}}" | head -n1)
if [ -z "$cid" ]; then
  echo "No running container from image $IMAGE"; exit 1
fi

exec docker exec -it "$cid" \
     gdb "/workspace/$BIN" \
     -ex "target remote :$PORT"
