##
# @file CMakeLists.txt
# @brief Main CMake configuration file for the defenseGUI project
# @details This file configures the build environment, sets C++ standards,
#          manages dependencies (especially raylib), and defines build targets.
##

# Require minimum CMake version 3.11 for FetchContent support
cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+

##
# @brief Define the project name
##
project(defenseGUI)

##
# @brief Configure C++ standard settings
# @details Sets the C++ standard to C++17 and makes it required
##
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

##
# @brief Enable generation of compile_commands.json for IDE integration
##
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

##
# @brief Configure raylib dependency
# @details Attempts to find installed raylib or downloads and builds it if not found
##
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED

##
# @brief Fetch and build raylib if not found on the system
##
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    endif()
endif()

##
# @brief Define the project's executable and source files
# @details Creates the main executable and links all necessary source files
##
add_executable(${PROJECT_NAME} main.cpp
        Map.h
        MapEditor.h
        Map.cpp
        MapEditor.cpp
        Game.h
        Game.cpp
        Tower.cpp
        Tower.h
        TowerObserver.h
        TowerManager.cpp
        TowerManager.h
        IObserver.h
        MapObserver.cpp
        MapObserver.h
        CritterWave.h
        CritterWave.cpp
        critter.h
        critter.cpp
        CritterObserver.h
        TargetingStrategy.cpp
        TargetingStrategy.h
        TowerDecorators.cpp
)

# Uncomment to enable verbose raylib output
#set(raylib_VERBOSE 1)

##
# @brief Link raylib to the project
##
target_link_libraries(${PROJECT_NAME} raylib)

##
# @brief Configure web platform specific settings
# @details Sets additional properties and flags for Emscripten when targeting web platform
##
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

##
# @brief Configure MacOS specific settings
# @details Links required frameworks when building on Apple platforms
##
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
