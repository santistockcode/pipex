Everytime I call a command shell calls to execve. 
Lo puedo comprobar con el comando strace. Por ejemplo:
strace "ls"
execve("/usr/bin/ls", ["ls"], 0x7ffca7549190 /* 48 vars */) = 0
brk(NULL)                               = 0x55caf6c5c000
arch_prctl(0x3001 /* ARCH_??? */, 0x7ffec5688620) = -1 EINVAL (Invalid argument)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f0e81a12000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache"
...etc


¿qué parámetros admite?
int execve(const char *__path, char *const __argv[], char *const __envp[])