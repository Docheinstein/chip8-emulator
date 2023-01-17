cmake_minimum_required(VERSION 3.24)
project(chip8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

file(GLOB_RECURSE sources "src/*")

# Find: OpenGL
find_package(OpenGL)
find_package(GLEW)
find_package(GLFW)
if (OpenGL_FOUND AND GLEW_FOUND AND GLFW_FOUND)
    set(OPENGL_FOUND TRUE)
else()
    set(OPENGL_FOUND FALSE)
endif()

if (OPENGL_FOUND)
    message(STATUS "OpenGL found")
    add_compile_definitions(OPENGL_ENABLED)
else()
    message(WARNING "OpenGL not found")
endif()

# Find Curses
find_package(Curses)
if (CURSES_FOUND)
    message(STATUS "Curses found")
    add_compile_definitions(CURSES_ENABLED)
else()
    message(WARNING "Curses not found")
endif()


# Find: ALSA
find_package(ALSA)
if (ALSA_FOUND)
    message(STATUS "ALSA found")
    add_compile_definitions(SOLOUD_ENABLED)
    add_compile_definitions(WITH_ALSA) # required by SoLoud
    file(GLOB_RECURSE soloud_sources
            "external/soloud/include/*"
            "external/soloud/src/core/*"
            "external/soloud/src/audiosource/wav/*"
            "external/soloud/src/backend/alsa/*")
else()
    message(WARNING "ALSA not found")
endif()

add_executable(chip8 ${soloud_sources} ${sources})

include_directories(src)

# Include: OpenGL
if (OPENGL_FOUND)
    target_include_directories(chip8 PRIVATE ${OPENGL_INCLUDE_DIRS})
    target_include_directories(chip8 PRIVATE ${GLEW_INCLUDE_DIRS})
    target_include_directories(chip8 PRIVATE ${GLFW_INCLUDE_DIRS})
endif()

# Include: ALSA and SoLoud
if (ALSA_FOUND)
    target_include_directories(chip8 PRIVATE external/soloud/include)
    target_include_directories(chip8 PRIVATE ${ALSA_INCLUDE_DIRS})
endif()

# Include: Curses
if (CURSES_FOUND)
    target_include_directories(chip8 PRIVATE ${CURSES_INCLUDE_DIRS})
endif()

# Link: OpenGL
if (OPENGL_FOUND)
    target_link_libraries(chip8 PRIVATE ${OPENGL_LIBRARIES})
    target_link_libraries(chip8 PRIVATE ${GLEW_LIBRARIES})
    target_link_libraries(chip8 PRIVATE ${GLFW_LIBRARIES})
endif()

# Link: Curses
if (CURSES_FOUND)
    target_link_libraries(chip8 PRIVATE ${CURSES_LIBRARIES})
endif()

# Link: ALSA
if (ALSA_FOUND)
    target_link_libraries(chip8 PRIVATE ${ALSA_LIBRARIES})
endif()