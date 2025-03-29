cmake_minimum_required(VERSION 2.8)
project(tess2)
file(GLOB SRCS "Source/*.cpp" "Source/*.c")
file(GLOB SRCS_INCL "Source/*.h" "Source/*.hpp")
include_directories("Include")
add_library(tess2 STATIC ${SRCS} ${SRCS_INCL})

# Specify where the library should be installed
install(TARGETS tess2
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

# Specify where the header files should be installed
install(FILES ${SRCS_INCL} DESTINATION include)
