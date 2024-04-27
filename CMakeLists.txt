cmake_minimum_required(VERSION 3.10)
project(TodosApp)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(todos src/main.cpp src/Database.cpp src/Command.cpp src/Shell.cpp src/API.cpp src/HttpRequest.cpp src/HttpResponse.cpp src/ContentType.cpp)

# Include directories for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Find the SQLite3 library
find_package(SQLite3 REQUIRED)  # Ensure it's required

# Include SQLite3 headers
include_directories(${SQLite3_INCLUDE_DIRS})

# Link the SQLite3 library
target_link_libraries(todos ${SQLite3_LIBRARIES})
