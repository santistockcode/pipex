SUCIO:
(incluido en Dockerfile)
- apt-get update
- apt-get install gdb
- apt-get install -y gdbserver
- make unit-debug
(incluido en unit-debug recipe)
- ./unit_tests_dbg --debug=gdb -debug-transport=tcp:1234 --filter cmd_resolve_path/finds_ls_on_normal_path
(incluido en attach_gdb.sh)
- then I open a new terminal in my host
- docker ps
- I copy the id and run docker exec -it <container-id> bash
- gdb ./unit_tests_dbg
- target remote :1234
(aquí empieza el debugueo como tal)
- break cmd_resolve_path
- continue

# TLDR

con la receta del makefile: 
make unit-debug TEST=cmd_resolve_path/1

abro otra terminal y corro:
./attach_gdb.sh (que lo que hace es que conecta otra terminal al contenedor y corre gdb ./unit_tests_dbg con target remote :1234)

Y aquí ya puedo hacer algo como: 
break cmd_resolve_path
continue

(nota, si por lo que sea en otra máquina el .sh no va, puedo abrir otra terminal con
docker exec -it <container-id> bash)(ver pasitos arriba)

# EXPLICACIÓN


### Quick theory recap

| 🔍 Concept                        | Why we use it in this workflow                                                                                                                                                                                                                                                                                                                                                   |
| --------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **`gdbserver`**                   | A tiny helper that starts **inside the program you want to debug**. It loads the binary, stops at the program entry-point, and waits for an external GDB client to connect. Because it lives in the same process as the test, it sees the *exact* memory and thread state—even when Criterion forks a child for each test case.                                                  |
| **Two terminals / two processes** | *Terminal #1* runs your test binary plus `gdbserver`; once `gdbserver` is listening it can’t also run an interactive debugger—it’s busy hosting the program. *Terminal #2* runs the GDB client, which sends break-points, `step`, `continue`, etc. to the waiting server. Keeping them separate avoids race conditions and keeps the program under test paused until you attach. |
| **A TCP port (`tcp:1234`)**       | `gdbserver` can communicate over serial lines, pipes, or network sockets. TCP is the easiest in containers: it needs no extra privileges, doesn’t require you to map host ports, and lets multiple debug sessions coexist (1234, 1235, …). GDB simply does `target remote :1234` and the socket becomes a transparent command channel.                                           |

**Sequence in practice**

1. **Test + gdbserver**

   ```bash
   ./unit_tests_dbg --debug=gdb --debug-transport=tcp:1234
   # prints: “Waiting for debugger to attach…”
   ```
2. **Attach from another shell**

   ```bash
   gdb ./unit_tests_dbg
   (gdb) target remote :1234
   (gdb) break cmd_resolve_path
   (gdb) continue
   ```
3. When you quit GDB, `gdbserver` releases the test and Criterion
   finishes as normal.

That’s all: `gdbserver` gives you *inside-the-process* visibility,
the second terminal provides an interactive debugger, and the port is
just the rendez-vous point between the two.
