# Simple File System

This project implements a simple shell file system in C++, enabling users to interact with a virtual file system through a command-line interface. The shell facilitates directory navigation, file creation, editing, removal, listing, and quitting functionalities.

## Overview

The shell comprises the following main components:

1. **Shell Class (`shell.h`, `shell.cpp`):** Defines the shell interface and provides methods to interact with the virtual file system.

2. **Manager Class (`man.h`):** Manages the file system operations, including initializing the file system, maintaining the directory structure, handling file operations, and managing memory allocation.

3. **Memory Class (`memory.h`):** Manages memory operations such as reading from and writing to disk blocks, and maintaining the file system's memory state.

4. **File and Directory Classes (`fd.h`):** Define the structures for files and directories in the file system.

## Features

- **Navigation:** Users can navigate through directories using commands like `cd` and `..`.
- **File Operations:** Users can create files (`touch`), edit files (`vi`), and remove files (`rm`).
- **Directory Operations:** Users can create directories (`mkdir`) and remove directories (`rm`).
- **Listing Contents:** Users can list the contents of the current directory (`ls`).
- **Quitting:** Users can exit the shell using the `quit` command.

## Usage

To use the file system, follow these steps:

1. Navigate to the project directory: `cd filesystem`
2. Run the executable: `./filesystem`

### Commands

- `cd <directory>`: Change directory.
- `ls`: List files and directories.
- `mkdir <directory>`: Create a new directory.
- `rm <file/directory>`: Remove a file or directory.
- `touch <file>`: Create a new file.
- `vi <file>`: Edit a file using the built-in text editor.
- `quit`: Exit the shell.

## Installation

To install and run the file system:

1. Clone the repository to your local machine.
2. Compile the source files using a C++ compiler.
3. Execute the compiled executable to start the shell.

## License

This project is licensed under the [MIT License](LICENSE). You are free to use, modify, and distribute the code for personal and commercial purposes.

Feel free to enhance and customize the file system according to your requirements!