#include <algorithm>
#include <functional>

#include "algo/a_star/path.hpp"
#include "assert.hpp"

namespace g::algo::a_star {
Path::Path() : m_totalG{0}, m_vector{} {}

void Path::append(VertexWithCost vertexWithCost)
{
  m_vector.push_back(vertexWithCost);
  m_totalG += vertexWithCost.g();
}

VertexWithCost& Path::back()
{
  G_ASSERT(
    !m_vector.empty(),
    "underlying vector is empty (size: %zu)",
    m_vector.size());
  return m_vector.back();
}

const VertexWithCost& Path::back() const
{
  return const_cast<this_type*>(this)->back();
}

std::int64_t Path::g() const { return m_totalG; }

std::vector<VertexIdentifier> Path::rawPath() const
{
  std::vector<VertexIdentifier> result(m_vector.size());
  std::transform(
    m_vector.begin(),
    m_vector.end(),
    result.begin(),
    std::mem_fn(&VertexWithCost::vertex));
  return result;
}
} // namespace g::algo::a_star
