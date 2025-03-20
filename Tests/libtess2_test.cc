#include <cmath>
#include <cstdlib>
#include <limits>
#include <vector>

#include "gtest/gtest.h"
#include "tesselator.h"

namespace {

constexpr int kComponentCount = 2;
constexpr int kNumTriangleVertices = 3;

struct Vector2f {
  float x;
  float y;
};

void* HeapAlloc(void* user_data, unsigned int size) { return malloc(size); }
void* HeapRealloc(void* user_data, void* ptr, unsigned int size) {
  return realloc(ptr, size);
}
void HeapFree(void* user_data, void* ptr) { free(ptr); }

// Adds a polygon with a hole to the tessellator.
//
// a = outer loop
// b = inner loop
// x = interior
// o = exterior
//
// +aaaaaaaaaaaaaa+
// a xx | xx | xx a
// a xx | xx | xx a
// a----+bbbb+----a
// a xx b oo b xx a
// a xx b oo b xx a
// a----+bbbb+----a
// a xx | xx | xx a
// a xx | xx | xx a
// +aaaaaaaaaaaaaa+
//
// This should tessellate to 8 triangles.
void AddPolygonWithHole(TESStesselator* tess) {
  std::vector<Vector2f> outer_loop = {
      {0, 0},
      {3, 0},
      {3, 3},
      {0, 3},
  };
  std::vector<Vector2f> inner_hole = {
      {1, 1},
      {2, 1},
      {2, 2},
      {1, 2},
  };

  tessSetOption(tess, TESS_REVERSE_CONTOURS, 0);
  tessAddContour(tess, kComponentCount, outer_loop.data(),
                 kComponentCount * sizeof(TESSreal), outer_loop.size());
  tessSetOption(tess, TESS_REVERSE_CONTOURS, 1);
  tessAddContour(tess, kComponentCount, inner_hole.data(),
                 kComponentCount * sizeof(TESSreal), inner_hole.size());
}

void AddPolyline(TESStesselator* tess, const std::vector<Vector2f>& polyline) {
  tessAddContour(tess, kComponentCount, polyline.data(),
                 kComponentCount * sizeof(TESSreal), polyline.size());
}

// Tests that tessellation succeeds when the default allocator is used.
TEST(Libtess2Test, DefaultAllocSuccess) {
  TESStesselator* tess = tessNewTess(nullptr);
  ASSERT_NE(tess, nullptr);

  // Add the polygon and tessellate it.
  AddPolygonWithHole(tess);
  EXPECT_NE(tessTesselate(tess, TESS_WINDING_POSITIVE, TESS_POLYGONS,
                          kNumTriangleVertices, kComponentCount, nullptr),
            0);

  // It should take 8 triangles to represent the polygon with the hole.
  EXPECT_EQ(tessGetElementCount(tess), 8);

  tessDeleteTess(tess);
}

TEST(Libtess2Test, CustomAllocSuccess) {
  TESSalloc alloc = {
      HeapAlloc,
      HeapRealloc,
      HeapFree,
      /*user_data=*/nullptr,
      /*meshEdgeBucketSize=*/512,
      /*meshVertexBucketSize=*/512,
      /*meshFaceBucketSize=*/256,
      /*dictNodeBucketSize=*/512,
      /*regionBucketSize=*/256,
      /*extraVertices=*/0,
  };
  TESStesselator* tess = tessNewTess(&alloc);
  ASSERT_NE(tess, nullptr);

  // Add the polygon and tessellate it.
  AddPolygonWithHole(tess);
  EXPECT_NE(tessTesselate(tess, TESS_WINDING_POSITIVE, TESS_POLYGONS,
                          kNumTriangleVertices, kComponentCount, nullptr),
            0);

  // It should take 8 triangles to represent the polygon with the hole.
  EXPECT_EQ(tessGetElementCount(tess), 8);

  tessDeleteTess(tess);
}

TEST(Libtess2Test, EmptyPolyline) {
  TESStesselator* tess = tessNewTess(nullptr);
  ASSERT_NE(tess, nullptr);
  AddPolyline(tess, {});
  EXPECT_NE(tessTesselate(tess, TESS_WINDING_POSITIVE, TESS_POLYGONS,
                          kNumTriangleVertices, kComponentCount, nullptr),
            0);
  EXPECT_EQ(tessGetElementCount(tess), 0);
}

TEST(Libtess2Test, SingleLine) {
  TESStesselator* tess = tessNewTess(nullptr);
  ASSERT_NE(tess, nullptr);
  AddPolyline(tess, {{0, 0}, {0, 1}});
  EXPECT_NE(tessTesselate(tess, TESS_WINDING_POSITIVE, TESS_POLYGONS,
                          kNumTriangleVertices, kComponentCount, nullptr),
            0);
  EXPECT_EQ(tessGetElementCount(tess), 0);
}

TEST(Libtess2Test, SingleTriangle) {
  TESStesselator* tess = tessNewTess(nullptr);
  ASSERT_NE(tess, nullptr);
  AddPolyline(tess, {{0, 0}, {0, 1}, {1, 0}});
  EXPECT_NE(tessTesselate(tess, TESS_WINDING_POSITIVE, TESS_POLYGONS,
                          kNumTriangleVertices, kComponentCount, nullptr),
            0);
  EXPECT_EQ(tessGetElementCount(tess), 1);
}

TEST(Libtess2Test, UnitQuad) {
  TESStesselator* tess = tessNewTess(nullptr);
  ASSERT_NE(tess, nullptr);
  AddPolyline(tess, {{0, 0}, {0, 1}, {1, 1}, {1, 0}});
  EXPECT_NE(tessTesselate(tess, TESS_WINDING_POSITIVE, TESS_POLYGONS,
                          kNumTriangleVertices, kComponentCount, nullptr),
            0);
  EXPECT_EQ(tessGetElementCount(tess), 2);
}

TEST(Libtess2Test, FloatOverflowQuad) {
  TESStesselator* tess = tessNewTess(nullptr);
  ASSERT_NE(tess, nullptr);

  constexpr float kFloatMin = std::numeric_limits<float>::min();
  constexpr float kFloatMax = std::numeric_limits<float>::max();

  // Add the polygon and tessellate it.
  AddPolyline(tess, {{kFloatMin, kFloatMin},
                     {kFloatMin, kFloatMax},
                     {kFloatMax, kFloatMax},
                     {kFloatMax, kFloatMin}});
  EXPECT_NE(tessTesselate(tess, TESS_WINDING_POSITIVE, TESS_POLYGONS,
                          kNumTriangleVertices, kComponentCount, nullptr),
            0);
  EXPECT_EQ(tessGetElementCount(tess), 0);
}

TEST(Libtess2Test, SingularityQuad) {
  TESStesselator* tess = tessNewTess(nullptr);
  ASSERT_NE(tess, nullptr);
  AddPolyline(tess, {{0, 0}, {0, 0}, {0, 0}, {0, 0}});
  EXPECT_NE(tessTesselate(tess, TESS_WINDING_POSITIVE, TESS_POLYGONS,
                          kNumTriangleVertices, kComponentCount, nullptr),
            0);
  EXPECT_EQ(tessGetElementCount(tess), 0);
}

TEST(Libtess2Test, DegenerateQuad) {
  TESStesselator* tess = tessNewTess(nullptr);
  ASSERT_NE(tess, nullptr);
  // A quad that's extremely close to a giant triangle, with an extra sliver.
  // Caused a segfault previously.
  AddPolyline(tess, {{0.f, 3.40282347e+38f},
                     {0.64113313f, -1.f},
                     {-0.f, -0.f},
                     {-3.40282347e+38f, 1.f}});
  EXPECT_NE(tessTesselate(tess, TESS_WINDING_POSITIVE, TESS_POLYGONS,
                          kNumTriangleVertices, kComponentCount, nullptr),
            0);
  EXPECT_EQ(tessGetElementCount(tess), 2);
}

}  // namespace
