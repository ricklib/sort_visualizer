cmake_minimum_required(VERSION 3.14)

project(SortVisualizer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Download and add raylib
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com//raysan5/raylib.git
  GIT_TAG 5.0
)

# Build raylib without its examples/tests
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

# Create executable
add_executable(${PROJECT_NAME}
  src/main.cpp
)

# Add include directory
target_include_directories(SortVisualizer
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Link raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# On Linux, link extra system libraries
if(UNIX AND NOT APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    m pthread dl GL rt X11
  )
endif()

cmake_minimum_required(VERSION 3.14)

project(SortVisualizer CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Copy compile_commands.json to project root
add_custom_command(
  TARGET ${PROJECT_NAME}
  PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          ${CMAKE_BINARY_DIR}/compile_commands.json
          ${CMAKE_SOURCE_DIR}/compile_commands.json
)

include(FetchContent)

# Download and add raylib
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com//raysan5/raylib.git
  GIT_TAG 5.0
)

# Build raylib without its examples/tests
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

