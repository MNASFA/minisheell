# Minishell

> **As beautiful as a shell** 🐚

A simple yet powerful shell implementation that recreates the basic functionality of Bash. This project provides hands-on experience with processes, file descriptors, and system-level programming in C.

## 📋 Table of Contents

- [About](#about)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Implementation Details](#implementation-details)
- [Built-in Commands](#built-in-commands)
- [Requirements](#requirements)
- [Author](#author)

## 🎯 About

Minishell is a 42 school project that challenges students to create their own shell from scratch. This project takes you back to the early days of computing, where developers had to create software to communicate with computers using interactive command lines.

The goal is to gain extensive knowledge about:
- Process creation and management
- File descriptors and I/O redirection
- Signal handling
- Parsing and tokenization
- Environment variables

## ✨ Features

### Core Functionality

- **Interactive Prompt**: Displays a prompt when waiting for commands
- **Command History**: Navigate through previously executed commands
- **Command Execution**: Search and launch executables based on PATH or absolute/relative paths
- **Quote Handling**:
  - Single quotes (`'`) prevent interpretation of metacharacters
  - Double quotes (`"`) prevent interpretation except for `$`

### Redirections

- `<` : Redirect input
- `>` : Redirect output
- `<<` : Here-document (reads until delimiter)
- `>>` : Redirect output in append mode

### Operators

- `|` : Pipes (connect output of one command to input of another)

### Variable Expansion

- `$VAR` : Expand environment variables
- `$?` : Exit status of the last executed command

### Signal Handling

- `Ctrl-C` : Display new prompt on a new line
- `Ctrl-D` : Exit the shell
- `Ctrl-\` : Do nothing (ignored)

### Built-in Commands

- `echo` with `-n` option
- `cd` with relative or absolute path
- `pwd` (no options)
- `export` (no options)
- `unset` (no options)
- `env` (no options or arguments)
- `exit` (no options)

## 🚀 Installation

### Prerequisites

- GCC compiler
- GNU Readline library
- Make

### Build Instructions

```bash
# Clone the repository
git clone <your-repo-url> minishell
cd minishell

# Compile the project
make

# Clean object files
make clean

# Remove all generated files
make fclean

# Recompile everything
make re
```

## 💻 Usage

### Starting the Shell

```bash
./minishell
```

### Example Commands

```bash
# Basic command execution
minishell$ ls -la

# Using pipes
minishell$ cat file.txt | grep "pattern" | wc -l

# Input/Output redirection
minishell$ cat < input.txt > output.txt

# Here-document
minishell$ cat << EOF
> This is a test
> EOF

# Append mode
minishell$ echo "new line" >> file.txt

# Environment variable expansion
minishell$ echo $HOME
minishell$ echo $USER is in $PWD

# Exit status
minishell$ ls
minishell$ echo $?

# Built-in commands
minishell$ cd /tmp
minishell$ pwd
minishell$ export VAR=value
minishell$ env
minishell$ exit
```

## 📁 Project Structure

```
minishell/
├── Makefile
├── minishell.h
├── src/
│   ├── main.c                    # Entry point and main loop
│   ├── parsing/
│   │   ├── tokenizer.c          # Lexical analysis and tokenization
│   │   └── expand.c             # Variable expansion logic
│   └── utils/
│       ├── ft_split.c           # String splitting with quote handling
│       └── utils.c              # Helper functions (strcmp, strdup, etc.)
└── README.md
```

## 🔧 Implementation Details

### Tokenization

The tokenizer (`tokenizer.c`) breaks down the input string into tokens, identifying:
- **Words**: Commands, arguments, and filenames
- **Pipes**: `|`
- **Redirections**: `<`, `>`, `<<`, `>>`

Special handling for:
- Quoted strings (single and double quotes)
- Special characters (`|`, `<`, `>`)
- Whitespace separation

### Variable Expansion

The expansion module (`expand.c`) handles:
- Environment variable initialization from `envp`
- Variable lookup and substitution
- Dynamic memory allocation for expanded strings
- Calculation of expanded string length

### Token Types

```c
typedef enum e_token_type
{
    WORD,       // Commands, arguments, filenames
    PIPE,       // "|"
    REDIR_IN,   // "<"
    REDIR_OUT,  // ">"
    HEREDOC,    // "<<"
    APPEND      // ">>"
} t_token_type;
```

### Data Structures

**Token Structure**:
```c
typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
} t_token;
```

**Environment Variable Structure**:
```c
typedef struct s_env
{
    char         *key;
    char         *value;
    struct s_env *next;
} t_env;
```

## 🛠️ Built-in Commands

| Command | Description |
|---------|-------------|
| `echo [-n] [text]` | Display a line of text |
| `cd [path]` | Change the current directory |
| `pwd` | Print working directory |
| `export [VAR=value]` | Set environment variables |
| `unset [VAR]` | Remove environment variables |
| `env` | Display environment variables |
| `exit` | Exit the shell |

## 📝 Requirements

### Compilation

- Compiled with: `cc -Wall -Wextra -Werror`
- Links with: `-lreadline`

### Memory Management

- No memory leaks (except those from `readline()` function)
- All heap-allocated memory must be properly freed
- No segmentation faults, bus errors, or double frees

### Forbidden

- Interpreting unclosed quotes
- Special characters not required: `\` (backslash), `;` (semicolon)

### Notes

- Uses at most **one global variable** for signal handling
- Takes Bash as reference for any unclear requirements
- Readline memory leaks are acceptable

## 🎓 Learning Outcomes

This project teaches:
- **Process Management**: Using `fork()`, `execve()`, `wait()` family
- **File Descriptors**: Understanding `pipe()`, `dup()`, `dup2()`
- **Signal Handling**: Working with `signal()`, `sigaction()`
- **System Calls**: Practical use of various system functions
- **Parsing**: Lexical analysis and tokenization techniques
- **Memory Management**: Dynamic allocation and proper cleanup

## 👤 Author

**hmnasfa** - [@hmnasfa](https://github.com/hmnasfa)

## 📄 License

This project is part of the 42 school curriculum.

---

*"The only way to do great work is to love what you do." - Steve Jobs*
