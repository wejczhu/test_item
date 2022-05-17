
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(SOURCE 
${CMAKE_SOURCE_DIR}/Core/Src/DataHandler.cpp
${CMAKE_SOURCE_DIR}/Core/Src/ClimateDataHandler.cpp
${CMAKE_SOURCE_DIR}/Core/Src/CommandDataHandler.cpp
${CMAKE_SOURCE_DIR}/Core/Src/CoreController.cpp
${CMAKE_SOURCE_DIR}/Core/Src/Timer.cpp
)

# Add the library
add_library(Core "${SOURCE}")

#add_library(somelib STATIC IMPORTED)

target_link_libraries(Core PUBLIC
                      Hardware
                      Storage
)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Core PUBLIC
                          "./Inc"
                          "${PROJECT_SOURCE_DIR}/Hardware/Inc"
                          "${PROJECT_SOURCE_DIR}/Storage/Inc"
                          )

