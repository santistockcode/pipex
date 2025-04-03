https://medium.com/@omimouni33/pipex-the-42-project-understanding-pipelines-in-c-71984b3f2103

EDGE CASES OR COMMON MISTAKES:

    Not using unlink(1) to remove temporary files.
    Using the wrong permissions when using open(2). The outfile needs to be opened with different permission depending on whether or not here_doc was used.
    Not appending NULL to the end of argv in execve(2). Doing so may lead to an invalid read.
    Not setting default values to your struct. This may lead to warnings from Valgrind (which shouldn’t cause a fail) if you use these properties in a conditional check (if, else, while, etc…).
    Mishandling invalid commands. You may not face this issue depending on how you developed your program, but in mine, it was possible to get NULL in cmd_paths, due to the command being invalid. If that’s your case too, it’s not a problem, just make sure you know what you’re doing.
    Not mimicking the behaviour of BASH. An invalid infile or command DOES NOT mean you exit the program.
    Special edge cases: /dev/urandom and /dev/stdin
