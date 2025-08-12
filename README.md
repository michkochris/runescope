# Runescope

Runescope is a powerful command-line tool for analyzing the execution of programs on Linux. It acts as a wrapper around your target executable, allowing you to trace its interactions with the operating system and libraries, and to perform memory analysis. Runescope leverages existing, powerful tools like `strace`, `ltrace`, and `Valgrind` to provide a unified and easy-to-use interface for program analysis.

## Features

*   **System Call Tracing:** Use `strace` to trace the system calls made by a program. This is invaluable for understanding how a program interacts with the kernel, files, and network.
*   **Library Call Tracing:** Use `ltrace` to trace the calls a program makes to shared libraries. This helps in understanding the program's high-level logic and its dependencies.
*   **Memory Analysis:** Use `Valgrind`'s `memcheck` tool to detect memory leaks and other memory-related errors in your programs.
*   **Unified Interface:** Runescope provides a single, consistent command-line interface for all its analysis modes.
*   **Executable Discovery:** Runescope can automatically find the target executable in your system's `PATH`.
*   **Extensible:** The parsing and analysis logic is designed to be easily extended to support more detailed analysis and new tools.

## Getting Started

### Prerequisites

To use all of Runescope's features, you need to have the following tools installed on your system:

*   `strace`: For system call tracing.
*   `ltrace`: For library call tracing.
*   `valgrind`: For memory analysis.

You can typically install these using your distribution's package manager. For example, on Debian/Ubuntu:

```bash
sudo apt-get update
sudo apt-get install strace ltrace valgrind
```

### Building

To build Runescope, you will need a C compiler (like `gcc`) and `make`.

1.  Clone the repository:
    ```bash
    git clone https://github.com/your-username/runescope.git
    cd runescope
    ```

2.  Build the project using the provided `Makefile`:
    ```bash
    make
    ```

This will create the `runescope` executable in the current directory.

### Installation

To install `runescope` on your system (e.g., to `/usr/local/bin`), you can use the `install` target in the `Makefile`:

```bash
sudo make install
```

This will allow you to run `runescope` from any terminal.

## Usage

The basic syntax for using Runescope is:

```
runescope [options] <executable> [executable_options...]
```

### Options

*   `-s`, `--static`: Enable `strace` to trace system calls.
*   `-l`, `--ltrace`: Enable `ltrace` to trace library calls.
*   `-m`, `--memcheck`: Enable `Valgrind`'s `memcheck` for memory analysis.
*   `-v`, `--verbose`: Enable verbose output from Runescope.

### Examples

**Trace the system calls of `ls -l`:**

```bash
runescope -s /bin/ls -l
```

**Trace the library calls of a custom program:**

```bash
runescope -l ./my_program
```

**Check for memory leaks in a program:**

```bash
runescope -m ./my_program
```

**Combine `strace` and `ltrace` analysis:**

```bash
runescope -s -l ./my_program
```

## Output

Runescope will create log files in the current directory for each of the enabled analysis tools:

*   `runescope_strace.log`: The output from `strace`.
*   `runescope_ltrace.log`: The output from `ltrace`.
*   `runescope_valgrind.log`: The output from `Valgrind`.

After the target program has finished executing, Runescope will parse these log files and print a summary of the analysis to the console.

## How it Works

Runescope works by forking a new process and then using `execve` to run the selected analysis tool (`strace`, `ltrace`, or `Valgrind`), which in turn executes the target program. The output of the analysis tool is redirected to a log file, which Runescope then parses to provide its analysis.

## Contributing

Contributions to Runescope are welcome! If you have ideas for new features, improvements, or bug fixes, please feel free to open an issue or submit a pull request.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.
