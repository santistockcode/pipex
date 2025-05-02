FROM ubuntu:22.04

# Compiler, debugger, leak checker
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
        build-essential valgrind lldb vim && \
    rm -rf /var/lib/apt/lists/*

# Work in a write‑able folder that we’ll mount from the host
WORKDIR /workspace
CMD ["bash"]

