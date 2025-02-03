## Instructions to run program.

### Using CLion
1. Open CLion
2. Go to File -> Open
3. Navigate to the project directory and click Open
4. CLion should automatically detect the CMake configuration
5. Click the Build button (hammer icon) or press Ctrl+F9 (Cmd+F9 on macOS)

### Using Command Line
```bash
# Create and enter build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the project
cmake --build .
