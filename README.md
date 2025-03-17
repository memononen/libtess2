*This project is minimally maintained.*

# Libtess2

Version 1.0.3

This is refactored version of the original libtess which comes with the GLU
reference implementation. The code is good quality polygon tesselator and
triangulator. The original code comes with rather horrible interface and its'
performance suffers from lots of small memory allocations. The main point of the
refactoring has been the interface and memory allocation scheme.

A lot of the GLU tesselator documentation applies to Libtess2 too (apart from
the API), check out The OpenGL Programming Guide (the "Red Book"), Chapter 11:
Tessellators and Quadrics.

Simple bucketed memory allocator (see Graphics Gems III for reference) was added
which speeds up the code by order of magnitude (tests showed 15 to 50 times
improvement depending on data). The API allows the user to pass his own
allocator to the library. It is possible to configure the library so that the
library runs on predefined chunk of memory.

The API was changed to loosely resemble the OpenGL vertex array API. The
processed data can be accessed via getter functions. The code is able to output
contours, polygons and connected polygons. The output of the tesselator can be
also used as input for new run. I.e. the user may first want to calculate an
union all the input contours and the triangulate them.

The code is released under SGI FREE SOFTWARE LICENSE B Version 2.0.
https://directory.fsf.org/wiki/License:SGIFreeBv2

[Premake](https://premake.github.io/docs/Using-Premake/) can be used to generate
Makefiles or project definition files in various configurations, which it puts
in a `Build/` subdirectory in the checkout. The project/directory structure is:

*   `libtess2`
    -   `Source/` contains the source files and internal headers.
    -   `Include/` contains the header files in the public API.
*   `example`
    -   `Example/` contains a GUI example app.
    -   `Bin/` contains binary assets for the example app.
    -   `Contrib/` contains dependencies for the example app.
*   There is a separate Bazel build, to support testing.
    -   `Tests/` contains the tests.

Mikko Mononen memon@inside.org
