# qt_sandbox
A basic Qt Hello World application **to learn and practice** the fundamentals of Qt GUI development.

## Project Structure

- `main.cpp` - Main application source code with a simple window and label
- `CMakeLists.txt` - Build configuration using CMake

## Prerequisites

Before building, ensure you have:

- **Qt6** installed (or Qt5, with minor adjustments to CMakeLists.txt)
- **CMake** 3.16 or higher
- A C++ compiler (gcc, clang, or MSVC)

### Installing Qt

**Ubuntu/Debian:**
```bash
sudo apt-get install qt6-base-dev
```

**macOS (using Homebrew):**
```bash
brew install qt@6
```

**Windows:**
Download and install from https://www.qt.io/download-open-source

## Building

1. Create a build directory:
```bash
mkdir build && cd build
```

2. Configure with CMake:
```bash
cmake ..
```

3. Build the project:
```bash
cmake --build .
```

## Running

From the build directory, run:
```bash
./hello_world
```

On macOS, you may need:
```bash
./hello_world
```

On Windows (MSVC):
```bash
./hello_world.exe
```

## What's in the Code

- **QApplication** - Manages the application's control flow and GUI settings
- **QMainWindow** - The main application window
- **QWidget** - The central widget that contains child widgets
- **QVBoxLayout** - Arranges widgets vertically
- **QLabel** - Displays the "Hello World!" text

This example demonstrates the basic pattern of creating a Qt GUI application with a window, layout, and widget.