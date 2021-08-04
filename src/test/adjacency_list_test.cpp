#include <doctest.h>

#include "adjacency_list.hpp"

TEST_CASE("(AdjacencyList, ShouldConstructEmptyList)")
{
  g::AdjacencyList list{};

  for (g::VertexIdentifier i{0}; i < 100; ++i) {
    for (g::VertexIdentifier j{0}; j < 100; ++j) {
      REQUIRE_FALSE(list.isDirectlyReachable(i, j));
    }

    REQUIRE_UNARY(list.directlyReachables(i).empty());
  }
}

TEST_CASE("(AdjacencyList, ShouldNotBeAbleToReachAnythingWithAnEmptyList)")
{
  g::AdjacencyList list{};
  CHECK_FALSE(list.isDirectlyReachable(1, 2));
}

TEST_CASE("(AdjacencyList, ShouldBeAbleToReachAnInsertedPair)")
{
  g::AdjacencyList list{};
  list.addTo(/* source */ 1, /* target */ 2);
  CHECK_UNARY(list.isDirectlyReachable(/* source */ 1, /* target */ 2));
  CHECK_FALSE(list.isDirectlyReachable(/* source */ 2, /* target */ 1));

  const std::vector<g::VertexIdentifier>& reachables{
    list.directlyReachables(/* source */ 1)};
  REQUIRE_EQ(1, reachables.size());
  CHECK_EQ(2, reachables.front());

  CHECK_UNARY(list.directlyReachables(2).empty());
}

TEST_CASE("(AdjacencyList, ShouldBeAbleToAddAnAdjacent)")
{
  g::AdjacencyList list{};
  REQUIRE_FALSE(list.isDirectlyReachable(1, 2));
  list.addTo(1, 2);
  REQUIRE_UNARY(list.isDirectlyReachable(1, 2));
}

TEST_CASE(
  "(AdjacencyList, ShouldDoNothingIfAnAdjacentThatIsAlreadyInThereIsAdded)")
{
  g::AdjacencyList list{};
  REQUIRE_FALSE(list.isDirectlyReachable(1, 2));
  list.addTo(1, 2);
  REQUIRE_UNARY(list.isDirectlyReachable(1, 2));
  list.addTo(1, 2);
  CHECK_UNARY(list.isDirectlyReachable(1, 2));
}

TEST_CASE("(AdjacencyList, ShouldBeAbleToAddReverseAdjacency)")
{
  g::AdjacencyList list{};
  REQUIRE_FALSE(list.isDirectlyReachable(1, 2));
  REQUIRE_FALSE(list.isDirectlyReachable(2, 1));

  list.addTo(1, 2);

  REQUIRE_UNARY(list.isDirectlyReachable(1, 2));
  REQUIRE_FALSE(list.isDirectlyReachable(2, 1));

  list.addTo(2, 1);

  REQUIRE_UNARY(list.isDirectlyReachable(1, 2));
  REQUIRE_UNARY(list.isDirectlyReachable(2, 1));
}

TEST_CASE("(AdjacencyList, ShouldNotGetAnyReachablesForAnEmptyList)")
{
  g::AdjacencyList list{};
  REQUIRE_UNARY(list.directlyReachables(1).empty());
}

TEST_CASE("(AdjacencyList, ShouldBeAbleToGetSingleAdjacentValue)")
{
  g::AdjacencyList list{};
  list.addTo(1, 2);
  const std::vector<g::VertexIdentifier>& vec{list.directlyReachables(1)};
  REQUIRE_EQ(1, vec.size());
  CHECK_EQ(2, vec.front());
}

TEST_CASE("(AdjacencyList, ShouldBeAbleToGetMultipleAdjacents)")
{
  g::AdjacencyList list{};
  for (g::VertexIdentifier i{1}; i <= 100; ++i) { list.addTo(0, i); }
  const std::vector<g::VertexIdentifier>& v{list.directlyReachables(0)};
  REQUIRE_EQ(100, v.size());

  for (g::VertexIdentifier i{1}; i <= 100; ++i) { REQUIRE_EQ(i, v[i - 1]); }
}

TEST_CASE(
  "(AdjacencyList, ShouldNotBeAbleToGetReverseAdjacentIfItIsNotInThere)")
{
  g::AdjacencyList list{};
  list.addTo(1, 2);
  CHECK_UNARY(list.directlyReachables(2).empty());
}

TEST_CASE("(AdjacencyList, ShouldBeAbleToGetReverseAdjacentIfItWasAdded)")
{
  g::AdjacencyList list{};
  list.addTo(1, 2);
  list.addTo(2, 1);

  const std::vector<g::VertexIdentifier> fromOne{list.directlyReachables(1)};
  const std::vector<g::VertexIdentifier> fromTwo{list.directlyReachables(2)};

  REQUIRE_EQ(1, fromOne.size());
  REQUIRE_EQ(1, fromTwo.size());

  CHECK_EQ(2, fromOne.front());
  CHECK_EQ(1, fromTwo.front());
}

TEST_CASE("(AdjacencyList, ShouldNotBeAbleToRemoveNonExistantAdjacency)")
{
  g::AdjacencyList list{};
  REQUIRE_UNARY(list.directlyReachables(1).empty());
  list.removeAdjacentFrom(1, 2);
  REQUIRE_UNARY(list.directlyReachables(1).empty());
}

TEST_CASE("(AdjacencyList, ShouldBeAbleToRemoveAdjacency)")
{
  g::AdjacencyList list{};
  list.addTo(1, 2);
  const std::vector<g::VertexIdentifier> v{list.directlyReachables(1)};
  REQUIRE_EQ(1, v.size());
  CHECK_EQ(2, v.front());
  list.removeAdjacentFrom(1, 2);
  const std::vector<g::VertexIdentifier> vNew{list.directlyReachables(1)};
  CHECK_UNARY(vNew.empty());
}

TEST_CASE("(AdjacencyList, ShouldNotAutomaticallyRemoveReverseAdjacency)")
{
  g::AdjacencyList list{};
  list.addTo(1, 2);
  list.addTo(2, 1);
  list.removeAdjacentFrom(1, 2);
  REQUIRE_UNARY(list.directlyReachables(1).empty());
  const std::vector<g::VertexIdentifier>& verts{list.directlyReachables(2)};
  REQUIRE_EQ(1, verts.size());
  CHECK_EQ(1, verts.front());
}

TEST_CASE("(AdjacencyList, ShouldDoNothingIfNonExistantVertexIsRemoved)")
{
  g::AdjacencyList list{};
  list.addTo(1, 2);
  list.removeVertex(3);
  const std::vector<g::VertexIdentifier>& v{list.directlyReachables(1)};
  REQUIRE_EQ(1, v.size());
  CHECK_EQ(2, v.front());
}

TEST_CASE("(AdjacencyList, ShouldBeAbleToRemoveVertex)")
{
  g::AdjacencyList list{};
  list.addTo(1, 2);
  list.addTo(3, 4);
  list.addTo(4, 3);
  list.removeVertex(1);
  REQUIRE_UNARY(list.directlyReachables(1).empty());

  list.removeVertex(3);
  REQUIRE_UNARY(list.directlyReachables(3).empty());
  REQUIRE_UNARY(list.directlyReachables(4).empty());

  list.addTo(4, 5);
  list.addTo(4, 6);
  list.removeVertex(5);

  const std::vector<g::VertexIdentifier>& v{list.directlyReachables(4)};
  REQUIRE_EQ(1, v.size());
  CHECK_EQ(6, v.front());
}

TEST_CASE("(AdjacencyList, ShouldDoNothingWhenClearingEmptyList)")
{
  g::AdjacencyList list{};
  list.clear();

  for (g::VertexIdentifier i{0}; i < 100; ++i) {
    for (g::VertexIdentifier j{0}; j < 100; ++j) {
      REQUIRE_FALSE(list.isDirectlyReachable(i, j));
    }

    REQUIRE_UNARY(list.directlyReachables(i).empty());
  }
}

TEST_CASE("(AdjacencyList, ShouldBeAbleToClear)")
{
  g::AdjacencyList list{};
  list.addTo(1, 2);
  list.addTo(2, 1);
  list.addTo(5, 6);
  list.addTo(5, 7);
  list.addTo(5, 8);
  list.addTo(5, 2);
  list.addTo(7, 9);
  list.clear();

  for (g::VertexIdentifier i{0}; i < 100; ++i) {
    for (g::VertexIdentifier j{0}; j < 100; ++j) {
      REQUIRE_FALSE(list.isDirectlyReachable(i, j));
    }

    REQUIRE_UNARY(list.directlyReachables(i).empty());
  }
}

TEST_CASE("(AdjacencyList, ShouldBeAbleToSwap)")
{
  g::AdjacencyList list1{}, list2{};

  list1.addTo(1, 2);
  list1.addTo(2, 3);

  list2.addTo(2, 1);
  list2.addTo(5, 6);
  list2.addTo(5, 7);
  list2.addTo(5, 8);
  list2.addTo(6, 5);

  list1.swap(list2);

  CHECK_UNARY(list1.isDirectlyReachable(2, 1));
  CHECK_UNARY(list1.isDirectlyReachable(5, 6));
  CHECK_UNARY(list1.isDirectlyReachable(5, 7));
  CHECK_UNARY(list1.isDirectlyReachable(5, 8));
  CHECK_UNARY(list1.isDirectlyReachable(6, 5));

  CHECK_UNARY(list2.isDirectlyReachable(1, 2));
  CHECK_UNARY(list2.isDirectlyReachable(2, 3));
}
