
set(CMAKE_CXX_FLAGS -lsqlite3)

# include_directories(/usr/include)
# link_directories(/usr/lib)


find_library(SQLite3 NAMES sqlite3 PATHS ${LIB_DIR_LOCATION})
#find_package (SQLite3)

set(SOURCE 
${CMAKE_SOURCE_DIR}/Storage/Src/DataStorageUnit.cpp
)

# Add the library
add_library(Storage "${SOURCE}")




#add_library(somelib STATIC IMPORTED)

# target_link_libraries(Storage PUBLIC
#                       Hardware
# )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Storage PUBLIC
                          "./Inc"
                          "${PROJECT_SOURCE_DIR}/StandardLib/jsoncpp/include"
                          )
