#include <unordered_map>

#include <doctest.h>

#include "edge.hpp"

TEST_CASE("(Edge, ShouldBeAbleToCreateEdge)")
{
  g::Edge edge{1, 2};
  CHECK_EQ(1, edge.source());
  CHECK_EQ(2, edge.target());
}
