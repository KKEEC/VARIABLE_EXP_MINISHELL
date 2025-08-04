# Minishell Project Tracker

> Author: [Your Name]
> School 42 Project — Minishell

---

## Setup & Shell Loop

- [X] Set up project folder structure
- [X] Create basic `main.c` with `readline()` loop
- [X] Add command history with `add_history()`
- [X] Handle `Ctrl+D` (EOF exit)
- [X] Create `Makefile`

---

## Lexer (Tokenizer)

- [X] Define token struct
- [X] Split input into tokens (commands, args, operators)
- [X] Handle quotes (`'`, `"`)
- [X] Detect operators: `|`, `<`, `>`, `>>`, `<<`
- [X] Handle escape characters
- [X] Return list of tokens

---

## Parser

- [X] Define AST or command node structure
- [X] Parse tokens into command blocks
- [X] Identify redirections and attach to commands
- [X] Handle pipe chains
- [X] Handle syntax errors (e.g., unexpected `|`, etc.)

---

## Execution (Basic)

- [ ] Implement command execution with `fork()` and `execve()`
- [ ] Search for binaries using `PATH`
- [ ] Pass environment (`envp`) to child process
- [ ] Handle return codes (`$?`)
- [ ] Handle command not found

---

## Pipes & Redirections

- [ ] Implement pipe chaining (`|`)
- [ ] Handle input redirection (`<`)
- [ ] Handle output redirection (`>`)
- [ ] Handle append (`>>`)
- [ ] Manage file descriptors with `dup2()`
- [ ] Clean up unused pipe ends

---

## Built-in Commands

- [ ] Implement `cd`
- [ ] Implement `echo`
- [ ] Implement `pwd`
- [ ] Implement `env`
- [ ] Implement `export`
- [ ] Implement `unset`
- [ ] Implement `exit`
- [ ] Integrate built-ins into executor without forking where needed

---

## Environment & Expansion

- [ ] Store environment in custom structure
- [ ] Implement `$VAR` expansion
- [ ] Support special variables like `$?`
- [ ] Handle expansions inside double quotes only
- [ ] Merge expanded strings into command args

---

## Signals & Heredoc

- [ ] Implement signal handling for `SIGINT`, `SIGQUIT`
- [ ] Ignore signals during heredoc
- [ ] Implement heredoc (`<<`)
- [ ] Store heredoc content in temporary file or pipe
- [ ] Clean up heredoc resources properly

---

## Polish & Testing

- [ ] Run full test suite with custom and bash comparison
- [ ] Handle edge cases: redirection errors, syntax, etc.
- [ ] Free all malloc'd memory (valgrind clean)
- [ ] Document known limitations
- [ ] Final cleanup, comments, README

---

## Bonus Features (Optional)

- [ ] Implement logical operators (`&&`, `||`)
- [ ] Add wildcard/globbing support (`*`)
- [ ] Implement `alias` support
- [ ] Add job control (`bg`, `fg`, `jobs`)
- [ ] Command history file (`~/.minishell_history`)