cmake_minimum_required(VERSION 3.16)
project(tess2 VERSION 1.0.2 LANGUAGES C)

install(FILES Include/tesselator.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_library(tess2 STATIC
    Source/bucketalloc.c
    Source/bucketalloc.h
    Source/dict.c
    Source/dict.h
    Source/geom.c
    Source/geom.h
    Source/mesh.c
    Source/mesh.h
    Source/priorityq.c
    Source/priorityq.h
    Source/sweep.c
    Source/sweep.h
    Source/tess.c
    Source/tess.h
)

target_include_directories(
    tess2
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
)

install(
  TARGETS tess2
  EXPORT tess2
  LIBRARY DESTINATION  "${CMAKE_INSTALL_LIBDIR}"
  INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
