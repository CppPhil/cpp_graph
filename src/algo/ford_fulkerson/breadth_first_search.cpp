#include "algo/ford_fulkerson/breadth_first_search.hpp"

namespace g::algo::ford_fulkerson {
void enqueue(
  std::queue<VertexIdentifier>&                q,
  VertexIdentifier                             x,
  std::unordered_map<VertexIdentifier, Color>& color)
{
  q.push(x);
  color[x] = Color::Gray;
}

VertexIdentifier dequeue(
  std::queue<VertexIdentifier>&                q,
  std::unordered_map<VertexIdentifier, Color>& color)
{
  const VertexIdentifier x{q.front()};
  q.pop();
  color[x] = Color::Black;
  return x;
}
} // namespace g::algo::ford_fulkerson
