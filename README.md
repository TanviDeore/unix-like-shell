# dash - A Unix Shell

## Overview

`dash` is a minimal Unix shell implemented in C. It supports basic command execution, parallel execution using `&`, output redirection using `>`, and built-in commands like `cd`, `exit`, and `path`.

## Features

- **Command Execution**: Runs standard commands from specified paths.
- **Built-in Commands**:
  - `cd <dir>`: Change directory.
  - `exit`: Exit the shell.
  - `path <dir1> <dir2> ...`: Set the path for executables.
- **Output Redirection**: `command > file` redirects stdout and stderr to a file.
- **Parallel Execution**: `command1 & command2` runs commands in parallel.
- **Batch Mode**: Execute commands from a file.
- **Interactive Mode**: Standard shell prompt with real-time command entry.

## Usage

![image](https://github.com/user-attachments/assets/16a3cc8a-637e-4df5-bd96-8b309f2bc772)

### Compile

    gcc -o dash dash.c

### Run

Interactive Mode:

    ./dash

Batch Mode:

    ./dash batchfile.txt

Notes
1) Only one output file is supported in redirection.
2) & should not be the first character in a command.
3) Commands are searched within the directories specified by the path command.

Limitation
   - No support for pipes (|), input redirection (<), or background jobs.
