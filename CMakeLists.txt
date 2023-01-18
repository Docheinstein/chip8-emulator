cmake_minimum_required(VERSION 3.24)
project(chip8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Disable GLFW tests and other stuff
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(GLOB_RECURSE sources "src/*")

# Find: OpenGL
find_package(OpenGL)
if (OPENGL_FOUND)
    message(STATUS "OpenGL found")
    add_compile_definitions(OPENGL_ENABLED)
    add_subdirectory(external/GLFW)
else()
    message(WARNING "OpenGL not found")
endif()

# Find: Curses
find_package(Curses)
if (CURSES_FOUND)
    message(STATUS "Curses found")
    add_compile_definitions(CURSES_ENABLED)
else()
    message(WARNING "Curses not found")
endif()

# Argumentum
add_subdirectory(external/argumentum)

# SoLoud
add_compile_definitions(SOLOUD_ENABLED)
add_compile_definitions(WITH_MINIAUDIO) # required by SoLoud
file(GLOB_RECURSE soloud_sources
        "external/soloud/include/*"
        "external/soloud/src/core/*"
        "external/soloud/src/audiosource/wav/*"
        "external/soloud/src/backend/miniaudio/*")


# Target
add_executable(chip8 ${glfw_sources} ${soloud_sources} ${sources})

# Include: src
target_include_directories(chip8 PRIVATE src)

# Include: OpenGL
if (OPENGL_FOUND)
    target_include_directories(chip8 PRIVATE ${OPENGL_INCLUDE_DIRS})
endif()

# Include: Curses
if (CURSES_FOUND)
    target_include_directories(chip8 PRIVATE ${CURSES_INCLUDE_DIRS})
endif()

# Include: SoLoud
target_include_directories(chip8 PRIVATE external/soloud/include)

# Include: Argumentum
target_include_directories(chip8 PRIVATE external/argumentum/include/argumentum)

# Link: OpenGL
if (OPENGL_FOUND)
    target_link_libraries(chip8 PRIVATE ${OPENGL_LIBRARIES})
    target_link_libraries(chip8 PRIVATE glfw)
endif()

# Link:
target_link_libraries(chip8 PRIVATE argumentum)

# Link: Curses
if (CURSES_FOUND)
    target_link_libraries(chip8 PRIVATE ${CURSES_LIBRARIES})
endif()