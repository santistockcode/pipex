# build once
docker build -t pipex-dev .

# hack on your code
docker run --rm -it -v "$PWD":/workspace --security-opt seccomp=unconfined pipex-dev

