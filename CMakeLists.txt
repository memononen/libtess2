project( tess2 )

cmake_minimum_required( VERSION 2.8.11 )

set( LIBNAME tess2 )

add_library( ${LIBNAME} SHARED
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

target_include_directories( ${LIBNAME}
	PUBLIC
	Include
)

install(
	FILES Include/tesselator.h
	DESTINATION include
)

install(
	TARGETS ${LIBNAME}
	DESTINATION lib
)

