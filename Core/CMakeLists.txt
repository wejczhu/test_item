
set(SOURCE 
${CMAKE_SOURCE_DIR}/Core/Src/DataHandler.cpp
)

# Add the library
add_library(Core "${SOURCE}")

# Add the library

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Core PUBLIC
                          "./Inc"
                          )

