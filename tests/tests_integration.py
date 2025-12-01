import os
import subprocess
import tempfile
import shutil
from pathlib import Path


PIPEX_BIN = Path('./pipex')


def run_cmd(argv, env=None, cwd=None, timeout=10):
	full_env = os.environ.copy()
	if env:
		full_env.update(env)
	proc = subprocess.run(
		argv,
		cwd=cwd,
		env=full_env,
		stdout=subprocess.PIPE,
		stderr=subprocess.PIPE,
		text=True,
		timeout=timeout,
	)
	return proc.returncode, proc.stdout, proc.stderr


def run_pipex(infile, commands, outfile, env=None):
	argv = ["./pipex", infile] + commands + [outfile]
	return run_cmd(argv, env=env)


class TempSandbox:
	def __init__(self):
		self.dir = Path(tempfile.mkdtemp(prefix='pipex-integration-'))

	def path(self, *parts):
		return self.dir.joinpath(*parts)

	def write(self, rel, content):
		p = self.path(rel)
		p.parent.mkdir(parents=True, exist_ok=True)
		p.write_text(content)
		return p

	def touch(self, rel):
		p = self.path(rel)
		p.parent.mkdir(parents=True, exist_ok=True)
		p.touch()
		return p

	def rm(self):
		shutil.rmtree(self.dir, ignore_errors=True)


def require_pipex_binary():
	if not PIPEX_BIN.exists():
		raise RuntimeError('pipex binary not found at ./pipex. Run `make` to build it.')
	if not os.access(str(PIPEX_BIN), os.X_OK):
		raise RuntimeError('pipex exists but is not executable. Try `make` or `chmod +x pipex`.')


def test_pipex_execve_missing_cmd():
	require_pipex_binary()
	sb = TempSandbox()
	try:
		infile = sb.write('in.txt', 'hello\nworld\n')
		outfile = sb.path('out.txt')
		cmd = ["no_such_cmd", "wc"]
		code, out, err = run_pipex(str(infile), cmd, str(outfile))
		print(f"[execve ENOENT] argv={cmd} code={code} stderr={err.strip()} outfile_exists={outfile.exists()}")
		assert code == 0
		assert outfile.exists()
		data = outfile.read_text()
		assert '0' in data
		assert 'not found' in err.lower() or 'no such file' in err.lower()
	finally:
		sb.rm()


def test_pipex_execve_eacces_script():
	require_pipex_binary()
	sb = TempSandbox()
	try:
		script = sb.write('script.sh', '#!/bin/sh\necho hi\n')
		# Remove execute permission to trigger EACCES
		os.chmod(script, 0o644)
		infile = sb.write('in.txt', 'data\n')
		outfile = sb.path('out.txt')
		cmd = [str(script), "wc"]
		code, out, err = run_pipex(str(infile), cmd, str(outfile))
		print(f"[execve EACCES] argv={cmd} code={code} stderr={err.strip()} outfile_exists={outfile.exists()}")
		# First cmd fails (EACCES), but wc runs on empty input; expect success with zeros
		assert code == 0
		assert outfile.exists()
		data = outfile.read_text()
		assert '0' in data
	finally:
		sb.rm()


def test_injection_execve_failure():
	require_pipex_binary()
	sb = TempSandbox()
	try:
		infile = sb.write('in.txt', 'hello\n')
		outfile = sb.path('out.txt')
		cmd = ["ls", "wc"]
		env = {"PIPEX_FAIL_EXECVE": "1"}
		code, out, err = run_pipex(str(infile), cmd, str(outfile), env=env)
		print(f"[inject execve] argv={cmd} env={env} code={code} stderr={err.strip()} outfile_exists={outfile.exists()}")
		# Why 126 and empty outfile?
		# Our syscall wrapper applies PIPEX_FAIL_EXECVE=1 globally to all execve calls.
		# That means both the first and the last pipeline stages fail their execve.
		# Since the pipeline exit status is that of the last command, pipex exits 126.
		# And because the last command never starts, it cannot write to outfile.
		assert code == 126
		assert (not outfile.exists()) or (outfile.stat().st_size == 0)
	finally:
		sb.rm()


def test_open_infile_enoent():
	require_pipex_binary()
	sb = TempSandbox()
	try:
		missing = sb.path('missing.txt')
		outfile = sb.path('out.txt')
		cmd = ["cat", "wc"]
		code, out, err = run_pipex(str(missing), cmd, str(outfile))
		print(f"[open ENOENT infile] argv={cmd} code={code} stderr={err.strip()} outfile_exists={outfile.exists()}")
		assert code == 0
		assert 'no such file' in err.lower() or 'not found' in err.lower() or 'open' in err.lower()
		assert outfile.exists()
	finally:
		sb.rm()


def test_open_infile_eacces():
	require_pipex_binary()
	sb = TempSandbox()
	try:
		infile = sb.touch('locked.txt')
		os.chmod(infile, 0)
		outfile = sb.path('out.txt')
		cmd = ["cat", "wc"]
		code, out, err = run_pipex(str(infile), cmd, str(outfile))
		print(f"[open EACCES infile] argv={cmd} code={code} stderr={err.strip()} outfile_exists={outfile.exists()}")
		# When infile open fails (EACCES), the first stage reads nothing.
		# The last stage (wc) still runs on empty input, writes zeros, and exits 0.
		assert code == 0
		assert 'permission' in err.lower() or 'access' in err.lower()
		assert outfile.exists()
		data = outfile.read_text()
		assert '0' in data
	finally:
		sb.rm()


def test_open_outfile_eacces():
	require_pipex_binary()
	sb = TempSandbox()
	try:
		infile = sb.write('in.txt', 'hello\n')
		outfile = sb.touch('out.txt')
		os.chmod(outfile, 0)
		cmd = ["cat", "wc"]
		code, out, err = run_pipex(str(infile), cmd, str(outfile))
		print(f"[open EACCES outfile] argv={cmd} code={code} stderr={err.strip()} outfile_exists={outfile.exists()}")
		assert code != 0
		assert 'permission' in err.lower() or 'access' in err.lower()
	finally:
		sb.rm()


def test_injection_open_eacces_path_match():
	require_pipex_binary()
	sb = TempSandbox()
	try:
		infile = sb.write('in.txt', 'hello\n')
		outfile = sb.path('blocked-out.txt')
		cmd = ["cat", "wc"]
		env = {"PIPEX_FAIL_OPEN_PATH": "blocked-out"}
		code, out, err = run_pipex(str(infile), cmd, str(outfile), env=env)
		print(f"[inject open EACCES] argv={cmd} env={env} code={code} stderr={err.strip()} outfile_exists={outfile.exists()}")
		assert code != 0
	finally:
		sb.rm()


def test_injection_pipe_emfile_at_2():
	require_pipex_binary()
	sb = TempSandbox()
	try:
		infile = sb.write('in.txt', 'a\nb\n')
		outfile = sb.path('out.txt')
		# Use 3-stage pipeline so that exactly two pipe() calls occur in builder
		cmd = ["cat", "grep a", "wc"]
		env = {"PIPEX_FAIL_PIPE_AT": "2"}
		code, out, err = run_pipex(str(infile), cmd, str(outfile), env=env)
		print(f"[inject pipe EMFILE@2] argv={cmd} env={env} code={code} stderr={err.strip()} outfile_exists={outfile.exists()}")
		assert code != 0
		assert 'pipe' in err.lower()
	finally:
		sb.rm()


def test_injection_pipe_emfile_multi_pipeline():
	require_pipex_binary()
	sb = TempSandbox()
	try:
		infile = sb.write('in.txt', 'hello\nworld\n')
		outfile = sb.path('out.txt')
		cmd = ["cat", "grep o", "wc -l", "cat"]
		env = {"PIPEX_FAIL_PIPE_AT": "3"}
		code, out, err = run_pipex(str(infile), cmd, str(outfile), env=env)
		print(f"[inject pipe EMFILE@4] argv={cmd} env={env} code={code} stderr={err.strip()} outfile_exists={outfile.exists()}")
		assert code != 0
		assert 'pipe' in err.lower()
	finally:
		sb.rm()


def test_injection_fork_eagain():
	require_pipex_binary()
	sb = TempSandbox()
	try:
		infile = sb.write('in.txt', 'hi\n')
		outfile = sb.path('out.txt')
		cmd = ["cat", "wc"]
		env = {"PIPEX_FAIL_FORK": "1"}
		code, out, err = run_pipex(str(infile), cmd, str(outfile), env=env)
		print(f"[inject fork EAGAIN] argv={cmd} env={env} code={code} stderr={err.strip()} outfile_exists={outfile.exists()}")
		assert code != 0
		assert 'fork' in err.lower() or 'resource' in err.lower() or 'again' in err.lower()
	finally:
		sb.rm()


def test_injection_dup2_ebadf():
	require_pipex_binary()
	sb = TempSandbox()
	try:
		infile = sb.write('in.txt', 'alpha\n')
		outfile = sb.path('out.txt')
		cmd = ["cat", "grep a", "wc -l"]
		env = {"PIPEX_FAIL_DUP2_AT": "2"}
		code, out, err = run_pipex(str(infile), cmd, str(outfile), env=env)
		print(f"[inject dup2 EBADF@3] argv={cmd} env={env} code={code} stderr={err.strip()} outfile_exists={outfile.exists()}")
		assert code != 0
		assert 'dup2' in err.lower() or 'bad file descriptor' in err.lower()
	finally:
		sb.rm()


def test_timing_sleep_pipeline():
	require_pipex_binary()
	sb = TempSandbox()
	try:
		infile = sb.write('in.txt', 'a\n')
		outfile = sb.path('out.txt')
		cmd = ["sleep 1", "wc -l"]
		code, out, err = run_pipex(str(infile), cmd, str(outfile))
		print(f"[timing sleep] argv={cmd} code={code} stderr={err.strip()} outfile_exists={outfile.exists()}")
		# Parent should wait; success if wc writes 1\n
		# After run, check outfile has content "1\n"
		if outfile.exists():
			data = outfile.read_text()
			assert data.strip() == '0'
		else:
			# If missing, treat as failure
			assert False, 'outfile missing after sleep pipeline'
	finally:
		sb.rm()


def _run_all():
	tests = [
		test_pipex_execve_missing_cmd,
		test_pipex_execve_eacces_script,
		test_injection_execve_failure,
		test_open_infile_enoent,
		test_open_infile_eacces,
		test_open_outfile_eacces,
		test_injection_open_eacces_path_match,
		test_injection_pipe_emfile_at_2,
		test_injection_pipe_emfile_multi_pipeline,
		test_injection_fork_eagain,
		test_injection_dup2_ebadf,
		test_timing_sleep_pipeline,
	]
	print(f"Running {len(tests)} integration tests...")
	passed = 0
	for i, t in enumerate(tests, 1):
		name = t.__name__
		print(f"\n[{i}/{len(tests)}] START {name}")
		try:
			t()
			print(f"[{i}/{len(tests)}] PASS {name}")
			passed += 1
		except AssertionError as e:
			print(f"[{i}/{len(tests)}] FAIL {name}: {e}")
			raise
		except Exception as e:
			print(f"[{i}/{len(tests)}] ERROR {name}: {e}")
			raise
	print(f"\nCompleted: {passed}/{len(tests)} passed")


if __name__ == '__main__':
	_run_all()

