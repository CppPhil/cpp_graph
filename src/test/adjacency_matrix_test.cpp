#include <doctest.h>

#include "adjacency_matrix.hpp"

TEST_CASE("(AdjacencyMatrix, ShouldConstructEmptymatrix)")
{
  g::AdjacencyMatrix matrix{};

  for (g::VertexIdentifier i{0}; i < 100; ++i) {
    for (g::VertexIdentifier j{0}; j < 100; ++j) {
      REQUIRE_FALSE(matrix.isDirectlyReachable(i, j));
    }

    REQUIRE_UNARY(matrix.directlyReachables(i).empty());
  }
}

TEST_CASE("(AdjacencyMatrix, ShouldNotBeAbleToReachAnythingWithAnEmptymatrix)")
{
  g::AdjacencyMatrix matrix{};
  CHECK_FALSE(matrix.isDirectlyReachable(1, 2));
}

TEST_CASE("(AdjacencyMatrix, ShouldBeAbleToReachAnInsertedPair)")
{
  g::AdjacencyMatrix matrix{};
  matrix.addTo(/* source */ 1, /* target */ 2);
  CHECK_UNARY(matrix.isDirectlyReachable(/* source */ 1, /* target */ 2));
  CHECK_FALSE(matrix.isDirectlyReachable(/* source */ 2, /* target */ 1));

  const std::vector<g::VertexIdentifier>& reachables{
    matrix.directlyReachables(/* source */ 1)};
  REQUIRE_EQ(1, reachables.size());
  CHECK_EQ(2, reachables.front());

  CHECK_UNARY(matrix.directlyReachables(2).empty());
}

TEST_CASE("(AdjacencyMatrix, ShouldBeAbleToAddAnAdjacent)")
{
  g::AdjacencyMatrix matrix{};
  REQUIRE_FALSE(matrix.isDirectlyReachable(1, 2));
  matrix.addTo(1, 2);
  REQUIRE_UNARY(matrix.isDirectlyReachable(1, 2));
}

TEST_CASE(
  "(AdjacencyMatrix, ShouldDoNothingIfAnAdjacentThatIsAlreadyInThereIsAdded)")
{
  g::AdjacencyMatrix matrix{};
  REQUIRE_FALSE(matrix.isDirectlyReachable(1, 2));
  matrix.addTo(1, 2);
  REQUIRE_UNARY(matrix.isDirectlyReachable(1, 2));
  matrix.addTo(1, 2);
  CHECK_UNARY(matrix.isDirectlyReachable(1, 2));
}

TEST_CASE("(AdjacencyMatrix, ShouldBeAbleToAddReverseAdjacency)")
{
  g::AdjacencyMatrix matrix{};
  REQUIRE_FALSE(matrix.isDirectlyReachable(1, 2));
  REQUIRE_FALSE(matrix.isDirectlyReachable(2, 1));

  matrix.addTo(1, 2);

  REQUIRE_UNARY(matrix.isDirectlyReachable(1, 2));
  REQUIRE_FALSE(matrix.isDirectlyReachable(2, 1));

  matrix.addTo(2, 1);

  REQUIRE_UNARY(matrix.isDirectlyReachable(1, 2));
  REQUIRE_UNARY(matrix.isDirectlyReachable(2, 1));
}

TEST_CASE("(AdjacencyMatrix, ShouldNotGetAnyReachablesForAnEmptymatrix)")
{
  g::AdjacencyMatrix matrix{};
  REQUIRE_UNARY(matrix.directlyReachables(1).empty());
}

TEST_CASE("(AdjacencyMatrix, ShouldBeAbleToGetSingleAdjacentValue)")
{
  g::AdjacencyMatrix matrix{};
  matrix.addTo(1, 2);
  const std::vector<g::VertexIdentifier>& vec{matrix.directlyReachables(1)};
  REQUIRE_EQ(1, vec.size());
  CHECK_EQ(2, vec.front());
}

TEST_CASE("(AdjacencyMatrix, ShouldBeAbleToGetMultipleAdjacents)")
{
  g::AdjacencyMatrix matrix{};
  for (g::VertexIdentifier i{1}; i <= 100; ++i) { matrix.addTo(0, i); }
  const std::vector<g::VertexIdentifier>& v{matrix.directlyReachables(0)};
  REQUIRE_EQ(100, v.size());

  for (g::VertexIdentifier i{1}; i <= 100; ++i) { REQUIRE_EQ(i, v[i - 1]); }
}

TEST_CASE(
  "(AdjacencyMatrix, ShouldNotBeAbleToGetReverseAdjacentIfItIsNotInThere)")
{
  g::AdjacencyMatrix matrix{};
  matrix.addTo(1, 2);
  CHECK_UNARY(matrix.directlyReachables(2).empty());
}

TEST_CASE("(AdjacencyMatrix, ShouldBeAbleToGetReverseAdjacentIfItWasAdded)")
{
  g::AdjacencyMatrix matrix{};
  matrix.addTo(1, 2);
  matrix.addTo(2, 1);

  const std::vector<g::VertexIdentifier> fromOne{matrix.directlyReachables(1)};
  const std::vector<g::VertexIdentifier> fromTwo{matrix.directlyReachables(2)};

  REQUIRE_EQ(1, fromOne.size());
  REQUIRE_EQ(1, fromTwo.size());

  CHECK_EQ(2, fromOne.front());
  CHECK_EQ(1, fromTwo.front());
}

TEST_CASE("(AdjacencyMatrix, ShouldNotBeAbleToRemoveNonExistantAdjacency)")
{
  g::AdjacencyMatrix matrix{};
  REQUIRE_UNARY(matrix.directlyReachables(1).empty());
  matrix.removeAdjacentFrom(1, 2);
  REQUIRE_UNARY(matrix.directlyReachables(1).empty());
}

TEST_CASE("(AdjacencyMatrix, ShouldBeAbleToRemoveAdjacency)")
{
  g::AdjacencyMatrix matrix{};
  matrix.addTo(1, 2);
  const std::vector<g::VertexIdentifier> v{matrix.directlyReachables(1)};
  REQUIRE_EQ(1, v.size());
  CHECK_EQ(2, v.front());
  matrix.removeAdjacentFrom(1, 2);
  const std::vector<g::VertexIdentifier> vNew{matrix.directlyReachables(1)};
  CHECK_UNARY(vNew.empty());
}

TEST_CASE("(AdjacencyMatrix, ShouldNotAutomaticallyRemoveReverseAdjacency)")
{
  g::AdjacencyMatrix matrix{};
  matrix.addTo(1, 2);
  matrix.addTo(2, 1);
  matrix.removeAdjacentFrom(1, 2);
  REQUIRE_UNARY(matrix.directlyReachables(1).empty());
  const std::vector<g::VertexIdentifier>& verts{matrix.directlyReachables(2)};
  REQUIRE_EQ(1, verts.size());
  CHECK_EQ(1, verts.front());
}

TEST_CASE("(AdjacencyMatrix, ShouldDoNothingIfNonExistantVertexIsRemoved)")
{
  g::AdjacencyMatrix matrix{};
  matrix.addTo(1, 2);
  matrix.removeVertex(3);
  const std::vector<g::VertexIdentifier>& v{matrix.directlyReachables(1)};
  REQUIRE_EQ(1, v.size());
  CHECK_EQ(2, v.front());
}

TEST_CASE("(AdjacencyMatrix, ShouldBeAbleToRemoveVertex)")
{
  g::AdjacencyMatrix matrix{};
  matrix.addTo(1, 2);
  matrix.addTo(3, 4);
  matrix.addTo(4, 3);
  matrix.removeVertex(1);
  REQUIRE_UNARY(matrix.directlyReachables(1).empty());

  matrix.removeVertex(3);
  REQUIRE_UNARY(matrix.directlyReachables(3).empty());
  REQUIRE_UNARY(matrix.directlyReachables(4).empty());

  matrix.addTo(4, 5);
  matrix.addTo(4, 6);
  matrix.removeVertex(5);

  const std::vector<g::VertexIdentifier>& v{matrix.directlyReachables(4)};
  REQUIRE_EQ(1, v.size());
  CHECK_EQ(6, v.front());
}

TEST_CASE("(AdjacencyMatrix, ShouldDoNothingWhenClearingEmptymatrix)")
{
  g::AdjacencyMatrix matrix{};
  matrix.clear();

  for (g::VertexIdentifier i{0}; i < 100; ++i) {
    for (g::VertexIdentifier j{0}; j < 100; ++j) {
      REQUIRE_FALSE(matrix.isDirectlyReachable(i, j));
    }

    REQUIRE_UNARY(matrix.directlyReachables(i).empty());
  }
}

TEST_CASE("(AdjacencyMatrix, ShouldBeAbleToClear)")
{
  g::AdjacencyMatrix matrix{};
  matrix.addTo(1, 2);
  matrix.addTo(2, 1);
  matrix.addTo(5, 6);
  matrix.addTo(5, 7);
  matrix.addTo(5, 8);
  matrix.addTo(5, 2);
  matrix.addTo(7, 9);
  matrix.clear();

  for (g::VertexIdentifier i{0}; i < 100; ++i) {
    for (g::VertexIdentifier j{0}; j < 100; ++j) {
      REQUIRE_FALSE(matrix.isDirectlyReachable(i, j));
    }

    REQUIRE_UNARY(matrix.directlyReachables(i).empty());
  }
}

TEST_CASE("(AdjacencyMatrix, ShouldBeAbleToSwap)")
{
  g::AdjacencyMatrix matrix1{}, matrix2{};

  matrix1.addTo(1, 2);
  matrix1.addTo(2, 3);

  matrix2.addTo(2, 1);
  matrix2.addTo(5, 6);
  matrix2.addTo(5, 7);
  matrix2.addTo(5, 8);
  matrix2.addTo(6, 5);

  matrix1.swap(matrix2);

  CHECK_UNARY(matrix1.isDirectlyReachable(2, 1));
  CHECK_UNARY(matrix1.isDirectlyReachable(5, 6));
  CHECK_UNARY(matrix1.isDirectlyReachable(5, 7));
  CHECK_UNARY(matrix1.isDirectlyReachable(5, 8));
  CHECK_UNARY(matrix1.isDirectlyReachable(6, 5));

  CHECK_UNARY(matrix2.isDirectlyReachable(1, 2));
  CHECK_UNARY(matrix2.isDirectlyReachable(2, 3));
}