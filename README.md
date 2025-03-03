# Raylib Installation Guide

Raylib is a simple and easy-to-use C library for game development. This guide provides installation instructions for Windows, macOS, and Linux.


## Installation and Integration into Project (on CLion)

### Windows
1. Download raylib Installer from [(raylib.com)](https://www.raylib.com/)
2. Copy and paste the CMakeLists.txt from "C:\raylib\raylib\projects\CMake" folder into your current project's folder.
3. Make sure to change the 'project(<project_name>)' and 'add_executable <filename.cpp>' in the CMakeLists.txt.
4. Include "raylib.h" in your implementation.
5. If it is not running, try to add the line "set(CMAKE_CXX_STANDARD 17)" to the CMakeLists.txt.

### Useful links
- [https://youtu.be/maR-qQqC0nM?si=URJMjyvOzh4_WDDw](https://youtu.be/maR-qQqC0nM?si=URJMjyvOzh4_WDDw)
- [https://youtu.be/PaAcVk5jUd8?si=z6xFGHu8isYZOVBM](https://youtu.be/PaAcVk5jUd8?si=z6xFGHu8isYZOVBM)
