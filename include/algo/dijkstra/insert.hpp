#ifndef INCG_G_ALGO_DIJKSTRA_INSERT_HPP
#define INCG_G_ALGO_DIJKSTRA_INSERT_HPP
#include <deque>

#include "algo/dijkstra/vertex_with_priority.hpp"

namespace g::algo::dijkstra {
void insert(std::deque<VertexWithPriority>& queue, VertexWithPriority toInsert);
} // namespace g::algo::dijkstra
#endif // INCG_G_ALGO_DIJKSTRA_INSERT_HPP
