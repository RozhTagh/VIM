# VIM Text Editor

This project is a streamlined text editor inspired by Vim. Created in C, it features essential text manipulation capabilities that imitate certain functionalities of Vim's command-line interface (CLI). This project is part of a course assignment aimed at developing a file editing tool without a graphical user interface (GUI).

## Features

The editor offers the following commands and functionalities:

### General Text Editing Features:
- **Create File**: 
  ```bash
  createfile --file <file path>
  ```
  Generates a new file at the specified location, automatically creating any missing directories.

- **Insert Text**:
  ```bash
  insertstr --file <file path> --str <text> --pos <line number>:<start position>
  ```
  Inserts the provided text at the designated line and position within the file.

- **Display File Content**:
  ```bash
  cat --file <file path>
  ```
  Shows the content of the specified file in the terminal.

- **Remove Text**:
  ```bash
  removestr --file <file path> --pos <line number>:<start position> --size <number of characters> --b <backward/forward>
  ```
  Deletes a specified number of characters from the file, either forward or backward from the given position.

- **Copy Text**:
  ```bash
  copystr --file <file path> --pos <line number>:<start position> --size <number of characters> --b <backward/forward>
  ```
  Copies a defined number of characters from the file to the clipboard.

- **Cut Text**:
  ```bash
  cutstr --file <file path> --pos <line number>:<start position> --size <number of characters> --b <backward/forward>
  ```
  Cuts a specified number of characters from the file and saves them to the clipboard.

- **Paste Text**:
  ```bash
  pastestr --file <file path> --pos <line number>:<start position>
  ```
  Pastes the content from the clipboard into the file at the specified location.

- **Find Text**:
  ```bash
  find --str <text> --file <file path> [--count] [--at <num>] [--all] [--byword]
  ```
  Searches for a specific string within the file, with options to count occurrences, find the nth instance, or locate by word.

- **Replace Text**:
  ```bash
  replace --str1 <old text> --str2 <new text> --file <file path> [--at <num>] [--all]
  ```
  Replaces instances of a string in the file with another specified string.

- **Undo**:
  ```bash
  undo --file <file path>
  ```
  Reverts the last change made to the file.

- **Grep Functionality**:
  ```bash
  grep --str <pattern> --files [<file1> <file2> ...]
  ```
  Searches for a specific pattern in multiple files and displays the matching lines.

- **Text Comparison**:
  ```bash
  compare <file1> <file2>
  ```
  Compares two files line by line, highlighting any differences.

### Advanced Features:
- **Directory Tree Display**:
  ```bash
  tree <depth>
  ```
  Displays the directory structure up to a specified depth.

- **Auto Indentation**:
  ```bash
  auto-indent <file>
  ```
  Automatically formats the content of the file to meet coding standards.

### Error Handling:
The program provides relevant error messages for invalid commands or parameters, such as `invalid command` for unrecognized inputs.

## Installation

1. Clone the repository:
   ```bash
   git clone <repository-url>
   ```
   
2. Compile the project using:
   ```bash
   gcc -o vim_editor vim_editor.c
   ```
   
3. Run the program:
   ```bash
   ./vim_editor
   ```
   
## Usage

Use the commands outlined above to create, edit, and manage text files. The program operates in a command-line environment and simulates Vim-like behaviors.
