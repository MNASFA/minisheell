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
│   ├── main.c                       # Entry point and main loop
│   ├── signals.c                    # Signal handling (Ctrl-C, Ctrl-D, Ctrl-\)
│   ├── parsing/                     # Parsing and tokenization
│   │   ├── tokenizer.c             # Lexical analysis
│   │   ├── expand.c                # Variable expansion
│   │   ├── expand_utils.c          # Expansion helper functions
│   │   ├── expand_heredoc.c        # Heredoc-specific expansion
│   │   ├── expanded_length.c       # Calculate expanded string size
│   │   ├── commands.c              # Command structure creation
│   │   ├── prepare_commands.c      # Command preparation pipeline
│   │   ├── parse_command.c         # Command parsing logic
│   │   ├── parse_help.c            # Parsing helper functions
│   │   ├── build_exec.c            # Build execution structure
│   │   ├── syntax_error.c          # Syntax validation
│   │   ├── heredoc.c               # Heredoc implementation
│   │   ├── heredoc2.c              # Additional heredoc functions
│   │   ├── fake_heredoc.c          # Heredoc for syntax checking
│   │   ├── envirement.c            # Environment initialization
│   │   ├── env_creation.c          # Environment variable creation
│   │   ├── free_utils.c            # Memory management
│   │   └── free_utils2.c           # Additional cleanup functions
│   ├── execution/                   # Command execution
│   │   ├── execution.c             # Main execution logic
│   │   ├── execution.h             # Execution structures and prototypes
│   │   ├── init_exe.c              # Execution initialization
│   │   ├── setup_path.c            # PATH resolution
│   │   ├── custom_execve.c         # Custom execve wrapper
│   │   ├── execute_child.c         # Child process execution
│   │   ├── handle_single.c         # Single command handling
│   │   ├── handle_pipeline.c       # Pipeline handling
│   │   ├── handle_pipline2.c       # Additional pipeline functions
│   │   ├── setup_io.c              # I/O setup for commands
│   │   ├── setup_lastio.c          # Last command I/O setup
│   │   ├── redirection1.c          # Input/output redirection
│   │   ├── redirection2.c          # Additional redirection handling
│   │   ├── close_clean.c           # File descriptor cleanup
│   │   ├── wait.c                  # Process waiting
│   │   ├── exit_status.c           # Exit status management
│   │   ├── env_list_array.c        # Environment list operations
│   │   └── spliiit.c               # String splitting for PATH
│   ├── built_in/                    # Built-in commands
│   │   ├── cd.c                    # Change directory
│   │   ├── cd1.c                   # CD helper functions
│   │   ├── cd2.c                   # Additional CD utilities
│   │   ├── echo.c                  # Echo command
│   │   ├── pwd.c                   # Print working directory
│   │   ├── env.c                   # Print environment
│   │   ├── export.c                # Export variables
│   │   ├── export1.c               # Export helpers
│   │   ├── export2.c               # Export utilities (sorting)
│   │   ├── export3.c               # Export update logic
│   │   ├── export4.c               # Export validation
│   │   ├── export5.c               # Additional export functions
│   │   ├── split_export.c          # Export string splitting
│   │   ├── unset.c                 # Unset variables
│   │   ├── exit.c                  # Exit command
│   │   └── exit2.c                 # Exit helpers
│   └── utils/                       # Utility functions
│       ├── utils.c                 # Basic utilities
│       ├── utils1.c                # Additional utilities
│       ├── utils2.c                # More helper functions
│       ├── ft_atoi.c               # String to integer
│       ├── ft_itoa.c               # Integer to string
│       ├── ft_strjoin.c            # String concatenation
│       └── split_tokens.c          # Token splitting
└── README.md
```

## 🔧 Implementation Details

### Architecture Overview

The shell is built with a clear separation of concerns:

1. **Parsing Phase**
   - Tokenization of input string
   - Quote handling (single and double)
   - Variable expansion
   - Syntax validation
   - Command structure creation

2. **Execution Phase**
   - Process creation and management
   - Pipe setup for pipelines
   - File descriptor management
   - Built-in command execution
   - External command execution via PATH

### Tokenization

The tokenizer (`tokenizer.c`) breaks down the input string into tokens, identifying:
- **Words**: Commands, arguments, and filenames
- **Pipes**: `|`
- **Redirections**: `<`, `>`, `<<`, `>>`

**Key Features**:
- Handles quoted strings (preserves spaces within quotes)
- Recognizes special characters
- Whitespace separation
- Quote state tracking to prevent interpretation of metacharacters

### Variable Expansion

The expansion module (`expand.c`, `expand_utils.c`, `expanded_length.c`) handles:
- Environment variable initialization from `envp`
- Variable lookup and substitution (`$VAR`, `$?`)
- Quote-aware expansion (no expansion in single quotes)
- Dynamic memory allocation for expanded strings
- Special handling for heredocs

**Expansion Process**:
1. Calculate required buffer size
2. Allocate memory
3. Expand variables while respecting quote context
4. Handle exit status (`$?`)
5. Remove quotes after expansion

### Heredoc Implementation

Heredocs (`heredoc.c`, `heredoc2.c`) support:
- Multiple heredocs per command
- Delimiter detection
- Quote handling on delimiters
- Variable expansion (disabled if delimiter is quoted)
- Temporary file management
- Signal handling during heredoc input

### Execution Engine

**Single Command** (`handle_single.c`):
- Direct execution for single commands
- Built-in commands execute in parent process
- External commands fork a child process
- I/O redirection setup

**Pipeline** (`handle_pipeline.c`, `handle_pipline2.c`):
- Creates pipes between commands
- Forks child processes for each command
- Sets up input/output redirection
- Closes unused file descriptors
- Waits for all processes to complete

**I/O Management** (`setup_io.c`, `setup_lastio.c`):
- First command: reads from stdin or redirected input
- Middle commands: read from previous pipe, write to next pipe
- Last command: writes to stdout or redirected output
- Proper file descriptor cleanup

### Built-in Commands

All built-in commands are implemented with proper error handling:

- **cd**: Directory navigation with OLDPWD/PWD management
- **echo**: Text output with `-n` flag support
- **pwd**: Current directory display with error recovery
- **export**: Environment variable management with validation
- **unset**: Variable removal
- **env**: Environment display
- **exit**: Shell termination with numeric argument support

### Signal Handling

Three different signal handlers (`signals.c`):

1. **Main prompt** (`sigint_handler_main`):
   - Ctrl-C: Display new prompt
   - Sets exit status to 130

2. **Heredoc input** (`sigint_handler_her`):
   - Ctrl-C: Abort heredoc
   - Clean up and return to prompt

3. **Command execution** (`sigint_handler_re`):
   - Ctrl-C: Terminate foreground process
   - Display newline

### Memory Management

Comprehensive cleanup functions:
- Token list freeing
- Command list freeing
- Execution structure freeing
- Environment list freeing
- Redirection list freeing
- Split array freeing

All allocations are tracked and freed to prevent memory leaks.

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
    int             quoted_flag;
    int             var_in_quotes;
    int             expanded_flag;
    char            *original_del;
    struct s_token  *next;
} t_token;
```

**Environment Variable Structure**:
```c
typedef struct s_env
{
    char         *key;
    char         *value;
    char         *full;
    int          is_first;
    int          is_print;
    int          fd_in;
    int          fd_out;
    struct s_env *next;
} t_env;
```

**Execution Structure**:
```c
typedef struct s_exec
{
    char         **args;
    t_redir      *redirections;
    char         *cmd;
    int          var_in_quotes;
    int          expanded_flag;
    int          outfd;
    int          infd;
    struct s_exec *next;
} t_exec;
```

**Redirection Structure**:
```c
typedef struct s_redir
{
    char            *filename;
    int             herdoc_fd;
    int             heredoc_count;
    int             quoted_flag;
    char            *delimiter;
    t_token_type    type;
    struct s_redir  *next;
} t_redir;
```

**Execution Engine Structure**:
```c
typedef struct s_exee
{
    int  cmd_count;
    int  **pipes;
    int  *pids;
    int  infile;
    int  outfile;
    int  cd_in;
    int  cd_out;
    int  saved_in;
    int  saved_out;
    int  fd_in;
    int  fd_out;
} t_exee;
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

- Compiled with: `cc -Wall -Wextra -Werror -fsanitize=address -g3`
- Links with: `-lreadline`

### Memory Management

- No memory leaks (except those from `readline()` function)
- All heap-allocated memory must be properly freed
- No segmentation faults, bus errors, or double frees
- Address sanitizer enabled during development

### Global Variable

- Uses **one global variable** `g_signum` for signal handling
- Stores only signal numbers (0, 130 for SIGINT)
- Does not access main data structures

### Forbidden

- Interpreting unclosed quotes
- Special characters not required: `\` (backslash), `;` (semicolon)

### Notes

- Uses at most **one global variable** for signal handling
- Takes Bash as reference for any unclear requirements
- Readline memory leaks are acceptable
- Maximum of 16 heredocs per command line
- Supports up to 100 tokens per command (expandable)

## 🎓 Learning Outcomes

This project teaches:
- **Process Management**: Using `fork()`, `execve()`, `wait()` family, process synchronization
- **File Descriptors**: Understanding `pipe()`, `dup()`, `dup2()`, proper FD management
- **Signal Handling**: Working with `signal()`, `sigaction()`, handling SIGINT/SIGQUIT
- **System Calls**: Practical use of `open()`, `close()`, `read()`, `write()`, `access()`
- **Parsing**: Lexical analysis, tokenization, syntax tree building
- **Memory Management**: Dynamic allocation, proper cleanup, leak prevention
- **String Manipulation**: Splitting, joining, searching, and copying strings
- **State Management**: Handling quotes, expansions, redirections
- **Error Handling**: Proper error messages, exit codes, edge cases
- **Shell Behavior**: Understanding how shells work internally

## 🐛 Testing

The project has been tested with:
- Basic command execution
- Complex pipelines (multiple pipes)
- Multiple redirections
- Heredocs with and without expansion
- Environment variable manipulation
- Built-in commands in various contexts
- Signal handling during execution
- Edge cases (empty commands, only spaces, quotes, etc.)
- Memory leaks (valgrind, fsanitize=address)

## 👤 Author

**hmnasfa** - [@hmnasfa](https://github.com/hmnasfa)  
**aboukhmi** - [@aboukhmi](https://github.com/aboukhmi)

## 🙏 Acknowledgments

- 42 Network for the project specifications
- The Bash manual for reference behavior
- All the peers who helped during development and testing

## 📄 License

This project is part of the 42 school curriculum.

---

*"The only way to do great work is to love what you do." - Steve Jobs*
