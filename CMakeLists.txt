cmake_minimum_required(VERSION 3.24)
project(chip8)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(ALSA)

file(GLOB_RECURSE sources "src/*")

if (ALSA_FOUND)
    add_compile_definitions(SOUND_ENABLED)
    add_compile_definitions(WITH_ALSA)
    file(GLOB_RECURSE soloud_sources
            "external/soloud/include/*"
            "external/soloud/src/core/*"
            "external/soloud/src/audiosource/wav/*"
            "external/soloud/src/backend/alsa/*")
endif()

add_executable(chip8 ${soloud_sources} ${sources})

# INCLUDE

target_include_directories(chip8 PRIVATE ${OPENGL_INCLUDE_DIRS})

if (ALSA_FOUND)
    target_include_directories(chip8 PRIVATE external/soloud/include)
endif()

# LINK

target_link_libraries(chip8 PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(chip8 PRIVATE ${GLEW_LIBRARIES})
target_link_libraries(chip8 PRIVATE glfw)

if (ALSA_FOUND)
    target_link_libraries(chip8 PRIVATE ${ALSA_LIBRARIES})
endif()
