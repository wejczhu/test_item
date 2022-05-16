cmake_minimum_required(VERSION 3.1.0)
project(test_item)

add_definitions(-std=c++11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# add the Hardware library
add_subdirectory(Hardware)
add_subdirectory(Core)
add_subdirectory(Storage)
add_subdirectory(StandardLib/jsoncpp/src/lib_json)

# add the executable
add_executable(test_item main.cpp)

# Add the library to test_item project
target_link_libraries(test_item PUBLIC
                      Hardware
                      Core
                      Storage
                      jsoncpp
                      pthread
                      sqlite3
                      )

target_include_directories(test_item PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/Hardware"
                          "${PROJECT_SOURCE_DIR}/Core"
                          "${PROJECT_SOURCE_DIR}/Storage"
                          "${PROJECT_SOURCE_DIR}/StandardLib/jsoncpp/src/lib_json"
                          )


