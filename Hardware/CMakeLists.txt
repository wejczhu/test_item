
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
set(CMAKE_CXX_FLAGS -pthread)
find_package(Threads REQUIRED)


set(SOURCE 
${CMAKE_SOURCE_DIR}/Hardware/Src/Uart.cpp 
${CMAKE_SOURCE_DIR}/Hardware/Src/UartUser.cpp
${CMAKE_SOURCE_DIR}/Hardware/Src/Ethernet.cpp
)

# Add the library
add_library(Hardware "${SOURCE}")

target_link_libraries(Hardware PUBLIC
                      Core
                      Threads::Threads
)

#target_link_libraries(myprog Threads::Threads )

# Add the library

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Hardware PUBLIC
                          "./Inc"
                          "${PROJECT_SOURCE_DIR}/Core/Inc"
                          )
