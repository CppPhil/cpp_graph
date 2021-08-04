#include "algo/ford_fulkerson/algorithm.hpp"

namespace g::algo::ford_fulkerson {
Result::Result(
  std::int32_t                                  maxFlow,
  const std::unordered_map<Edge, std::int32_t>& flow)
  : m_maxFlow{maxFlow}, m_flow{flow}
{
}

std::int32_t Result::maxFlow() const { return m_maxFlow; }

const std::unordered_map<Edge, std::int32_t>& Result::flow() const
{
  return m_flow;
}
} // namespace g::algo::ford_fulkerson
