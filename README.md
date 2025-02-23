# Raylib Installation Guide

Raylib is a simple and easy-to-use C library for game development. This guide provides installation instructions for Windows, macOS, and Linux.

## Prerequisites
- A C compiler (GCC, Clang, or MSVC)
- CMake (optional but recommended)

## Installation

### Windows
#### Using MSYS2 (Recommended)
1. Install MSYS2 from [https://www.msys2.org/](https://www.msys2.org/)
2. Open MSYS2 terminal and update packages:
   ```sh
   pacman -Syu
   ```
3. Install Raylib:
   ```sh
   pacman -S mingw-w64-x86_64-raylib
   ```
4. To compile a program with Raylib, use:
   ```sh
   gcc -o mygame mygame.c -lraylib -lglfw3 -lopengl32 -lgdi32 -lwinmm
   ```

#### Using vcpkg
1. Install vcpkg from [https://github.com/microsoft/vcpkg](https://github.com/microsoft/vcpkg)
2. Install Raylib:
   ```sh
   vcpkg install raylib
   ```
3. Link Raylib in your project using vcpkg paths.

### macOS
#### Using Homebrew (Recommended)
1. Install Homebrew if not installed:
   ```sh
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```
2. Install Raylib:
   ```sh
   brew install raylib
   ```
3. To compile a program:
   ```sh
   clang -o mygame mygame.c -lraylib
   ```

### Linux
#### Ubuntu/Debian
1. Install dependencies:
   ```sh
   sudo apt update && sudo apt install build-essential libgl1-mesa-dev libegl1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libasound2-dev
   ```
2. Install Raylib:
   ```sh
   sudo apt install libraylib-dev
   ```
3. Compile a program:
   ```sh
   gcc -o mygame mygame.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
   ```

#### Arch Linux
1. Install Raylib:
   ```sh
   sudo pacman -S raylib
   ```
2. Compile your program as shown in the Ubuntu section.

### Building from Source (For Any OS)
1. Clone the Raylib repository:
   ```sh
   git clone https://github.com/raysan5/raylib.git
   cd raylib
   ```
2. Build using CMake:
   ```sh
   mkdir build && cd build
   cmake ..
   make
   sudo make install  # (Optional) Install system-wide
   ```

## Verifying Installation
Try running the provided Raylib examples or compiling a simple test program.

## More Information
For more details, visit the official Raylib documentation: [https://www.raylib.com/](https://www.raylib.com/)

