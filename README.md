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
### Compile

    gcc -o dash dash.c

### Run

Interactive Mode:

    ./dash

Batch Mode:

    ./dash batchfile.txt

## Sample Run 

![image](https://github.com/user-attachments/assets/6f11eb5c-d7c6-4e34-8311-027e4b31826c)

Notes
1) Only one output file is supported in redirection.
2) & should not be the first character in a command.
3) Commands are searched within the directories specified by the path command.

Limitation
   - No support for pipes (|), input redirection (<), or background jobs.
