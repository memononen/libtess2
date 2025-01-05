cmake_minimum_required(VERSION 3.16)
project(tess2 VERSION 1.0.1 LANGUAGES C)
include_directories(Include)

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

target_include_directories(tess2
    PUBLIC
        ${PROJECT_SOURCE_DIR}Include
)
