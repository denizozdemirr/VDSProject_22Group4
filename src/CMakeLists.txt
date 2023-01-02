add_subdirectory(test)
add_subdirectory(bench)
add_subdirectory(verify)

#set(CMAKE_CXX_FLAGS "{CMAKE_CXX_FLAGS} -Wall -Werror ")
add_library(Manager Manager.cpp)

