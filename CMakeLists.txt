cmake_minimum_required(VERSION 3.10)
project(TodosApp)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
# Note that we are now including the source files from the src/ directory
add_executable(todos src/main.cpp src/Database.cpp src/Shell.cpp)

# Include directories for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Find the SQLite3 library within the lib/ directory
find_library(SQLITE3_LIBRARY NAMES sqlite3 libsqlite3 sqlite3.lib PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src/lib NO_DEFAULT_PATH)

# If the SQLite3 library was found, link the library
if(NOT SQLITE3_LIBRARY)
    message(FATAL_ERROR "SQLite3 library not found")
endif()

# Link the SQLite3 library
target_link_libraries(todos ${SQLITE3_LIBRARY})
