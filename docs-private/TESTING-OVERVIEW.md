# Testing Strategy Overview - Visual Summary

```
┌─────────────────────────────────────────────────────────────────────┐
│                    YOUR TESTING JOURNEY                              │
└─────────────────────────────────────────────────────────────────────┘

                PIPEX (NOW)                    MINISHELL (FUTURE)
                    │                                  │
            ┌───────┴───────┐                 ┌────────┴─────────┐
            │               │                 │                  │
         Simple         Finished          Complex          Starting
       2 processes      Working       Multiple pipes        Fresh
       Basic pipes      Good tests    Signals/heredoc   Need TDD
            │               │                 │                  │
            └───────┬───────┘                 └────────┬─────────┘
                    │                                  │
                    ▼                                  ▼
          
       ❌ Mocking NOT Worth It           ✅ Mocking IS Worth It
          
       Current Strategy ✓                Setup Needed:
       ├── Criterion (unit)               ├── Python + pytest
       └── Bash (integration)             ├── Function pointers
                                          ├── Mock syscalls
                                          └── TDD approach


┌─────────────────────────────────────────────────────────────────────┐
│                    TESTING DECISION TREE                             │
└─────────────────────────────────────────────────────────────────────┘

                        Start Here
                            │
                Is project simple (<1000 lines)?
                            │
                    ┌───────┴───────┐
                   YES              NO
                    │                │
            Are tests working?   Need mocking
            (except Docker)      for control
                    │                │
                   YES              YES
                    │                │
              Fix Docker         Set up mocking
              run as non-root    infrastructure
                    │                │
                    └────────┬───────┘
                             │
                        All tests pass ✓


┌─────────────────────────────────────────────────────────────────────┐
│              THE DOCKER PERMISSION PROBLEM                           │
└─────────────────────────────────────────────────────────────────────┘

   CAMPUS MACHINE                        DOCKER (ROOT)
   (Normal User)                         (Before Fix)
        │                                      │
        ▼                                      ▼
   $ chmod 000 file                    $ chmod 000 file
   $ ./pipex file cmd1 cmd2            $ ./pipex file cmd1 cmd2
        │                                      │
        ▼                                      ▼
   Permission denied ✓                 Opens successfully ✗
   (Test passes)                       (Test fails)
        │                                      │
        └──────────────┬───────────────────────┘
                       │
                       ▼
              
              SOLUTIONS:
              
    1. Run Docker as non-root  ←── Quick Fix (30 min)
       docker run --user 1000:1000 ...
    
    2. Mock open() syscall     ←── Proper Fix (for minishell)
       mock_set_open_errno(EACCES)
       Works even as root!


┌─────────────────────────────────────────────────────────────────────┐
│                TESTING PYRAMID COMPARISON                            │
└─────────────────────────────────────────────────────────────────────┘

    TRADITIONAL                    SYSTEMS PROGRAMMING
    (Web Apps)                    (Pipex, Minishell)
    
         /\                              /\
        /E2\                            /UN\
       /E2E \                          /UNIT\
      /──────\                        /──────\
     /  INTE  \                      /  INTE  \
    / GRATION \                    / GRATION  \
   /────────────\                /──────────────\
  /     UNIT     \              /      E2E       \
 /________________\            /__________________\
  
  70% Unit                     30% Unit
  20% Integration              60% Integration  
  10% E2E                      10% E2E
  
  Why inverted?                Why normal?
  - Pure business logic        - Most code interacts with OS
  - Minimal OS interaction     - Integration IS the value
  - Fast unit tests win        - Testing fork/pipe/exec


┌─────────────────────────────────────────────────────────────────────┐
│              WHAT TO TEST WHERE (MINISHELL)                          │
└─────────────────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────────────┐
│ UNIT TESTS (Criterion + Mocks)                          40%   │
├────────────────────────────────────────────────────────────────┤
│ • Tokenizer/Lexer          ✓ Pure logic                       │
│ • Parser (AST)             ✓ No system calls                  │
│ • Env var expansion        ✓ String manipulation              │
│ • Quote removal            ✓ String utilities                 │
│ • Path resolution          ✓ Uses access() (ok)               │
│ • Builtin: cd, echo        ✓ Minimal syscalls                 │
│ • Builtin: env, export     ✓ No process creation              │
│ • Error formatting         ✓ String building                  │
│                                                                │
│ WITH MOCKS:                                                    │
│ • Fork failure handling    ✓ Mock fork() -> -1                │
│ • Pipe failure handling    ✓ Mock pipe() -> -1                │
│ • Permission errors        ✓ Mock open() -> EACCES            │
└────────────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────────────┐
│ INTEGRATION TESTS (Python + pytest)                     50%   │
├────────────────────────────────────────────────────────────────┤
│ • Command execution        ✓ Real fork/exec                   │
│ • Simple pipes: A | B      ✓ Real pipe()                      │
│ • Multiple pipes: A|B|C|D  ✓ Chain testing                    │
│ • Redirections: <, >, >>   ✓ Real file ops                    │
│ • Here-doc: <<             ✓ Complex interaction              │
│ • Signals: Ctrl-C, Ctrl-D  ✓ Signal handling                  │
│ • Exit code propagation    ✓ Process sync                     │
│ • Command not found        ✓ Error paths                      │
│ • Permission denied        ✓ (with non-root Docker)           │
│ • Environment persistence  ✓ Stateful testing                 │
└────────────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────────────┐
│ E2E TESTS (Valgrind + Real Scenarios)                   10%   │
├────────────────────────────────────────────────────────────────┤
│ • Memory leaks             ✓ Valgrind                         │
│ • Real command sequences   ✓ Like real usage                  │
│ • Performance              ✓ Large inputs                     │
│ • Cross-environment        ✓ Docker vs campus                 │
└────────────────────────────────────────────────────────────────┘


┌─────────────────────────────────────────────────────────────────────┐
│                    TOOL COMPARISON                                   │
└─────────────────────────────────────────────────────────────────────┘

Feature                  Bash    Criterion    Python + pytest
──────────────────────────────────────────────────────────────────
Pure function tests      ❌      ✅ Excellent  ⚠️  Overkill
System integration       ✅ OK   ❌ Can't do   ✅ Excellent
Mocking                  ❌      ⚠️  Complex   ✅ Easy
Fixtures                 ❌      ⚠️  Manual    ✅ Built-in
Parameterization         ❌      ⚠️  Manual    ✅ @pytest.mark
Error reporting          ⚠️  OK  ✅ Good      ✅ Excellent
Test discovery           ❌      ✅ OK        ✅ Automatic
Coverage reports         ❌      ⚠️  Manual    ✅ pytest-cov
Timeout handling         ⚠️  OK  ❌           ✅ pytest-timeout
Parallel execution       ❌      ❌           ✅ pytest-xdist
Learning curve           Easy    Medium      Medium
Setup time               None    30 min      1-2 hours

Recommendation:
• Pipex:     Bash + Criterion ✓
• Minishell: Python + Criterion + Mocks ✓


┌─────────────────────────────────────────────────────────────────────┐
│              MOCKING PATTERN (FUNCTION POINTERS)                     │
└─────────────────────────────────────────────────────────────────────┘

BEFORE (Hard-coded)              AFTER (Mockable)
────────────────────            ────────────────────

int fd = open(file, ...);      int fd = g_syscalls->open(file, ...);
pid_t p = fork();               pid_t p = g_syscalls->fork();
execve(path, ...);              g_syscalls->execve(path, ...);

├─ Direct syscall               ├─ Function pointer
├─ Can't control                ├─ Can swap in tests
├─ Docker root issues           ├─ Works as root
└─ Hard to test errors          └─ Easy to test errors


IN PRODUCTION:                  IN TESTS:
g_syscalls = &real_ops;         g_syscalls = &mock_ops;
    ↓                               ↓
Uses actual syscalls            Uses your mocks
    ↓                               ↓
Real behavior                   Controlled behavior


MOCK EXAMPLE:
int mock_open(...) {
    if (should_fail)
        return -1, errno = EACCES;  ← You control this!
    return 42;
}


┌─────────────────────────────────────────────────────────────────────┐
│                    TIMELINE COMPARISON                               │
└─────────────────────────────────────────────────────────────────────┘

WITHOUT MOCKING (Minishell)           WITH MOCKING (Minishell)

Week 1:  Code tokenizer               Week 1:  Setup tests (1 day)
         No tests                              Code tokenizer
         Debug manually                        Write tests first ✓
         
Week 2:  Code parser                  Week 2:  TDD parser
         Manual testing                        All tests pass ✓
         Find bugs later                       Catch bugs early ✓
         
Week 3:  Code executor                Week 3:  TDD executor
         Works on campus ✓                     Works everywhere ✓
         Fails in Docker ✗                     Mock syscalls ✓
         
Week 4:  Debug Docker issues          Week 4:  Add pipes
         Debug fork failures                   Tests guide design ✓
         Debug race conditions                 No race conditions ✓
         
Week 5:  Still debugging...           Week 5:  Add redirections
         Permission issues                     All edge cases tested ✓
         Signal problems                       
         
Week 6:  More debugging...            Week 6:  Add signals
         Edge cases failing                    Clean implementation ✓
         
Week 7:  Finally working              Week 7:  Refinement
         But fragile                           Robust code ✓
         
Week 8:  Evaluation prep              Week 8:  Evaluation prep
         Pray it works                         Confident it works ✓
         
         Total: 8 weeks of stress              Total: 7 weeks + less stress
         Many bugs in evaluation               Few bugs in evaluation


┌─────────────────────────────────────────────────────────────────────┐
│                    YOUR ROADMAP                                      │
└─────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ THIS WEEK (Pipex)                                               │
├─────────────────────────────────────────────────────────────────┤
│ ✅ Read all documentation (done!)                               │
│ ☐ Fix Docker tests: Run as non-root (30 min)                   │
│ ☐ Verify all tests pass                                        │
│ ☐ Keep current test strategy (it's good!)                      │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ BEFORE MINISHELL                                                │
├─────────────────────────────────────────────────────────────────┤
│ ☐ Learn Python basics (if needed)                              │
│ ☐ Learn pytest:                                                │
│   └─ https://docs.pytest.org/en/stable/                        │
│ ☐ Read refactoring-example-mockable-syscalls.md                │
│ ☐ Understand function pointers in C                            │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ WEEK 1 OF MINISHELL                                             │
├─────────────────────────────────────────────────────────────────┤
│ ☐ Set up test infrastructure:                                  │
│   ├─ Create tests/{unit,integration,e2e} directories           │
│   ├─ Install pytest, pytest-cov, pytest-timeout                │
│   ├─ Create syscall_interface.h                                │
│   ├─ Implement real syscall wrappers                           │
│   ├─ Create mock syscalls                                      │
│   └─ Write example tests to verify setup                       │
│                                                                 │
│ ☐ Set up Docker with non-root user                             │
│                                                                 │
│ ☐ Write Makefile targets:                                      │
│   ├─ test-unit (Criterion)                                     │
│   ├─ test-integration (pytest)                                 │
│   ├─ test-e2e (valgrind)                                       │
│   └─ test-all                                                  │
└─────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────┐
│ WEEKS 2-8 OF MINISHELL                                          │
├─────────────────────────────────────────────────────────────────┤
│ For each feature:                                               │
│   1. Write unit test (if pure logic)                           │
│   2. Write integration test (Python)                           │
│   3. Implement feature                                          │
│   4. Verify tests pass                                          │
│   5. Refactor                                                   │
│   6. Repeat                                                     │
│                                                                 │
│ Run tests continuously:                                         │
│   make test-unit     # After each code change                  │
│   make test          # Before each commit                      │
│   make test-all      # Before pushing                          │
└─────────────────────────────────────────────────────────────────┘


┌─────────────────────────────────────────────────────────────────────┐
│                    KEY TAKEAWAYS                                     │
└─────────────────────────────────────────────────────────────────────┘

1. Your instinct is CORRECT ✓
   Mocking IS necessary for complex multiprocess projects

2. For Pipex: Don't mock
   - Fix Docker (run as non-root)
   - Keep current tests
   - Not worth refactoring

3. For Minishell: DO mock
   - Use function pointers pattern
   - Use Python + pytest
   - Set up Day 1
   - Will save weeks of debugging

4. Python has excellent mocking
   - unittest.mock
   - pytest fixtures
   - Much better than bash for complex tests

5. Testing pyramid is inverted
   - Normal for systems programming
   - More integration tests than unit tests
   - This is OK and expected

6. Test at the right level
   - Unit: Pure functions (Criterion)
   - Integration: System interaction (Python)
   - E2E: Real scenarios (Valgrind)

7. TDD is your friend
   - Write test first
   - Implement feature
   - Refactor
   - Catch bugs early


┌─────────────────────────────────────────────────────────────────────┐
│                    DOCUMENTATION INDEX                               │
└─────────────────────────────────────────────────────────────────────┘

1. testing-strategy.md
   → Comprehensive strategy (full details)

2. testing-quick-reference.md
   → Decision tree and cheat sheet

3. testing-examples.md
   → Concrete examples and refactoring patterns

4. mocking-strategy-for-minishell.md
   → When and how to mock system calls

5. refactoring-example-mockable-syscalls.md
   → Complete refactoring guide with code

6. ACTION-PLAN.md
   → Immediate steps and timeline

7. MOCKING-DECISION-SUMMARY.md
   → Answer to "Should I mock?" question

8. TESTING-OVERVIEW.md (this file)
   → Visual summary of everything


┌─────────────────────────────────────────────────────────────────────┐
│                    QUESTIONS ANSWERED                                │
└─────────────────────────────────────────────────────────────────────┘

Q: When does test become integration vs unit?
A: If it calls fork/exec/pipe or does file I/O → Integration
   If it's pure logic → Unit

Q: Should I mock system calls?
A: Pipex: No. Minishell: Yes.

Q: Why doesn't Criterion work for integration?
A: Fork creates child processes that confuse test runner
   Designed for single-process, pure function tests

Q: Does Python have mocking tools?
A: Yes! unittest.mock is excellent
   Much better than bash for complex scenarios

Q: How to fix Docker permission tests?
A: Run container as non-root user
   OR mock open() syscall

Q: Is mocking worth the effort?
A: For pipex: No (too simple)
   For minishell: Absolutely yes

Q: What's the testing pyramid for systems programming?
A: Inverted: More integration than unit
   This is normal and correct


Good luck with minishell! 🚀
You're thinking about testing the right way.
The setup time will pay off massively.
```

