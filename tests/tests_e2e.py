import os
import subprocess
from typing import List, Callable
"""
End-to-end testing suite for pipex binary.
This module provides a testing framework to validate the pipex program by comparing
its output against equivalent bash command pipelines. It creates test workspaces,
executes both pipex and bash versions of commands, and verifies that outputs match.
Classes:
    CasesSuite: Manages and executes multiple test scenarios.
    CaseScenario: Represents a single test case with workspace setup and execution.
Functions:
    main: Entry point that sets up and runs the test suite.
    assert_files_equal: Compares content of two files to verify they match.
Typical usage:
    python tests_e2e.py
The test framework:
1. Creates isolated workspaces for each test case
2. Generates input files and bash scripts
3. Executes both pipex binary and equivalent bash commands
4. Compares outputs to ensure correctness

"""

class CasesSuite:
    def __init__(self):
        self.cases: List["CaseScenario"] = []

    def add_case(self, case: "CaseScenario"):
        self.cases.append(case)

    def get_cases(self) -> List["CaseScenario"]:
        return self.cases

    def run_all(self):
        for case in self.cases:
            case.create_workspace()
            case.source_bash_script()
            case.better_call_pipex()
            case.function_assert(
                os.path.join(case.workspace_bash, case.bash_args[3]),
                os.path.join(case.workspace_pipex, case.pipex_args[3])
            )

class CaseScenario:
    def __init__(self, workspace: str, pipex_bin: str, pipex_args: list, bash_args: list, function_assert: Callable[[str, str], None]):
        # Normalize workspace to absolute path to avoid CWD-related issues
        self.workspace = os.path.abspath(workspace)
        self.workspace_bash = os.path.join(self.workspace, "bash")
        self.workspace_pipex = os.path.join(self.workspace, "pipex")
        self.pipex_bin = pipex_bin
        self.pipex_args = pipex_args
        self.bash_args = bash_args
        self.function_assert = function_assert

    
    def create_workspace(self):
        # Create isolated subfolders to avoid cross-artifacts between bash and pipex runs
        os.makedirs(self.workspace_bash, exist_ok=True)
        os.makedirs(self.workspace_pipex, exist_ok=True)
        # Common test inputs replicated to both environments
        with open(os.path.join(self.workspace_bash, "infile.txt"), 'w') as f:
            f.write("input data for pipex")
        with open(os.path.join(self.workspace_pipex, "infile.txt"), 'w') as f:
            f.write("input data for pipex")
        # Generic 'input' file used by additional e2e cases
        with open(os.path.join(self.workspace_bash, "input"), 'w') as f:
            f.write("hello world\nline two\nHELLO again\nfinal line\n")
        with open(os.path.join(self.workspace_pipex, "input"), 'w') as f:
            f.write("hello world\nline two\nHELLO again\nfinal line\n")
        return self.workspace

    def better_call_pipex(self):
        # Execute pipex to generate outfile_pipex
        cmd = [self.pipex_bin] + self.pipex_args
        subprocess.run(cmd, cwd=self.workspace_pipex, check=False)

    def source_bash_script(self):
        # Generate and execute bash pipeline to produce outfile_bash
        # Create script at case level, then cd into bash/ during execution
        script_path = os.path.join(self.workspace, "sh_pipex.sh")
        script_path_abs = os.path.abspath(script_path)
        with open(script_path_abs, 'w') as f:
            f.write("#!/bin/bash\n")
            # Ensure the bash script runs inside bash/ subfolder
            f.write(f"cd \"{self.workspace_bash}\"\n")
            infile_abs = os.path.abspath(os.path.join(self.workspace_bash, self.bash_args[0]))
            outfile_abs = os.path.abspath(os.path.join(self.workspace_bash, self.bash_args[3]))
            # Use absolute paths so running the script from any CWD works
            f.write(f"< \"{infile_abs}\" {self.bash_args[1]} | {self.bash_args[2]} > \"{outfile_abs}\"\n")
        os.chmod(script_path_abs, 0o755)
        # Run using absolute path; script itself cd's into bash/
        subprocess.run(["bash", script_path_abs], check=False)


def main():
    # Assume we always run from repo root under tox or Makefile (fixed path)
    pipex_bin = os.path.abspath("./pipex")
    if os.environ.get("PIPEX_DEBUG") == "1":
        import pdb; pdb.set_trace()
    if not os.path.exists(pipex_bin):
        raise FileNotFoundError(f"Pipex binary not found: {pipex_bin}")

    # Case 1: cat wc
    cases_suite = CasesSuite()
    case1 = CaseScenario(
        workspace="./tests/e2e/case_cat_wc",
        pipex_bin=pipex_bin,
        pipex_args=["infile.txt", "cat", "wc", "outfile.txt"],
        bash_args=["infile.txt", "cat", "wc", "outfile.txt"],
        function_assert=lambda outfile_bash, outfile_pipex: assert_files_equal(outfile_bash, outfile_pipex)
    )
    # register case1 into cases_suite
    cases_suite.add_case(case1)

    # Case 2: < input ls | ls > output
    case_ls_ls = CaseScenario(
        workspace="./tests/e2e/case_ls_ls",
        pipex_bin=pipex_bin,
        pipex_args=["input", "ls", "ls", "output.txt"],
        bash_args=["input", "ls", "ls", "output.txt"],
        function_assert=lambda outfile_bash, outfile_pipex: assert_files_equal(outfile_bash, outfile_pipex)
    )
    cases_suite.add_case(case_ls_ls)

    # Case 3: < input grep hello | wc > output
    case_grep_wc = CaseScenario(
        workspace="./tests/e2e/case_grep_wc",
        pipex_bin=pipex_bin,
        pipex_args=["input", "grep hello", "wc", "output.txt"],
        bash_args=["input", "grep hello", "wc", "output.txt"],
        function_assert=lambda outfile_bash, outfile_pipex: assert_files_equal(outfile_bash, outfile_pipex)
    )
    cases_suite.add_case(case_grep_wc)

    # Case 4: < input grep hello | sleep 3 > output (sleep produces no output; both should be empty files)
    case_grep_sleep = CaseScenario(
        workspace="./tests/e2e/case_grep_sleep",
        pipex_bin=pipex_bin,
        pipex_args=["input", "grep hello", "sleep 3", "output.txt"],
        bash_args=["input", "grep hello", "sleep 3", "output.txt"],
        function_assert=lambda outfile_bash, outfile_pipex: assert_files_equal(outfile_bash, outfile_pipex)
    )
    cases_suite.add_case(case_grep_sleep)

    # Case 5: < input sleep 3 | ls > output (sleep produces nothing; ls lists cwd)
    case_sleep_ls = CaseScenario(
        workspace="./tests/e2e/case_sleep_ls",
        pipex_bin=pipex_bin,
        pipex_args=["input", "sleep 3", "ls", "output.txt"],
        bash_args=["input", "sleep 3", "ls", "output.txt"],
        function_assert=lambda outfile_bash, outfile_pipex: assert_files_equal(outfile_bash, outfile_pipex)
    )
    cases_suite.add_case(case_sleep_ls)

    # Case 6: < input ls | foo > outfile (foo is invalid command; both should produce empty outfile)
    case_ls_foo = CaseScenario(
        workspace="./tests/e2e/case_ls_foo",
        pipex_bin=pipex_bin,
        pipex_args=["input", "ls", "foo", "output.txt"],
        bash_args=["input", "ls", "foo", "output.txt"],
        function_assert=lambda outfile_bash, outfile_pipex: assert_files_equal(outfile_bash, outfile_pipex)
    )
    cases_suite.add_case(case_ls_foo)

    # Case 7: < input foo | ls > outfile (foo is invalid command; both should produce empty outfile)
    case_foo_ls = CaseScenario(
        workspace="./tests/e2e/case_foo_ls",
        pipex_bin=pipex_bin,
        pipex_args=["input", "foo", "ls", "output.txt"],
        bash_args=["input", "foo", "ls", "output.txt"],
        function_assert=lambda outfile_bash, outfile_pipex: assert_files_equal(outfile_bash, outfile_pipex)
    )
    cases_suite.add_case(case_foo_ls)

    # run all cases
    cases_suite.run_all()

# function assert outfile, or fd, or whatever
def assert_files_equal(outfile_bash, outfile_pipex):
    with open(outfile_bash, 'r') as f_bash, open(outfile_pipex, 'r') as f_pipex:
        bash_content = f_bash.read()
        pipex_content = f_pipex.read()
        assert bash_content == pipex_content, "Outputs do not match!"

if __name__ == "__main__":
    main()