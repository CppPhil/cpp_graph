#include <doctest.h>

#include "directionality.hpp"

TEST_CASE("(Directionality, ShouldNotBeEqual)")
{
  CHECK_NE(g::Directionality::Directed, g::Directionality::Undirected);
}
