# 🏔️ Overview

Welcome to **Minishell**, a minimalistic shell implemented in C. Inspired by Bash, this project replicates essential shell functionalities, focusing on command parsing, execution, and environment management. Developed as part of the **42 Cursus**, this project explores the inner workings of Unix shells.

---

## 🔄 Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-ins](#built-ins)
- [Externals](#externals)
- [Project Structure](#project-structure)
- [Credits](#credits)

---

## ✨ Features

- **Command Parsing**: Parses user input, including quotes, arguments, and escape characters.
- **Built-in Commands**: Implements `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`.
- **Pipes and Redirections**: Handles pipes (`|`) and input/output redirections (`<`, `>`, `>>`, `<<`).
- **Environment Variables**: Manages and expands environment variables dynamically.
- **Command History**: Tracks session commands and integrates them into history.
- **Signal Handling**: Handles `Ctrl+C`, `Ctrl+D`, and `Ctrl+\`.
- **External Commands**: Executes commands from the system `PATH`.

---

## 🚀 Installation

### Prerequisites
- Unix-like operating system (Linux, macOS)
- C compiler (supports C99 or later)
- `readline` library

### Clone the Repository
```bash
git clone https://github.com/majakaszuba/minishell.git
cd minishell
```

### Build the Project
```bash
make
```

This generates an executable named `minishell`.

### Run Minishell
```bash
./minishell
```

---

## 🎀 Usage

### Custom Prompt
Once launched, you'll see the **Barbie Bash** prompt:
```bash
🎀 Barbie Bash 💅 :
```

### Example Commands
- **Run external commands:**
  ```bash
  🎀 Barbie Bash 💅 : ls -la
  ```
- **Use built-in commands:**
  ```bash
  🎀 Barbie Bash 💅 : echo "Hello, Barbie!"
  Hello, Barbie!
  ```

---

## 🛠️ Built-ins

### Supported Commands

| Command   | Description                               | Example                            |
|-----------|-------------------------------------------|------------------------------------|
| `echo`    | Prints arguments to the terminal          | `echo Hello, World!`              |
| `cd`      | Changes the current directory             | `cd /path/to/directory`           |
| `pwd`     | Prints the current directory              | `pwd`                             |
| `export`  | Sets environment variables                | `export VAR="value"`            |
| `unset`   | Unsets environment variables              | `unset VAR`                       |
| `env`     | Displays environment variables            | `env`                             |
| `exit`    | Exits the shell                           | `exit`                            |

---

## 📦 Externals

### Pipes and Redirections

- **Pipes (`|`)**
  ```bash
  🎀 Barbie Bash 💅 : ls -la | grep ".c"
  ```

- **Output Redirection (`>`, `>>`)**
  ```bash
  🎀 Barbie Bash 💅 : echo "Hello" > file.txt
  🎀 Barbie Bash 💅 : echo "World" >> file.txt
  ```

- **Input Redirection (`<`)**
  ```bash
  🎀 Barbie Bash 💅 : sort < unsorted.txt
  ```

- **Heredoc (`<<`)**
  ```bash
  🎀 Barbie Bash 💅 : cat << EOF
  > This is a heredoc test.
  > EOF
  ```

---

## 🌐 Project Structure

```plaintext
minishell/
├── include/          # Header files
├── src/              # Source files organized into:
│   ├── main.c        # Main logic
│   ├── parser.c      # Command parsing
│   ├── executor.c    # Command execution
│   ├── builtin.c     # Built-in command implementations
│   ├── redir.c       # Environment variable management
│   ├── signals.c     # Signal handling
│   └── utils.c       # Helper functions
└── Makefile          # Build script
```

---

## 🌟 Credits

Developed with ❄️[0h-laugh](https://github.com/0h-laugh) by 🎀[MajaKaszuba](https://github.com/MajaKaszuba) as part of the 42 Cursus. Special thanks to the 42 community for inspiration and support!

If you have any questions or suggestions, feel free to reach out to us! 💕

