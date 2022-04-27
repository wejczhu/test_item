
set(SOURCE 
${CMAKE_SOURCE_DIR}/Storage/Src/DataHandler.cpp 
)

# Add the library
add_library(Storage "${SOURCE}")

# Add the library

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Storage PUBLIC
                          "./Inc"
                          )

