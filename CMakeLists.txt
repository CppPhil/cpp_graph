cmake_minimum_required(VERSION 3.15)

enable_testing()

project(graph_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    string(APPEND CMAKE_CXX_FLAGS "/W4")
  endif()

  string(
    APPEND
    CMAKE_CXX_FLAGS
    " /DNOMINMAX /D_UNICODE /DUNICODE /DCPP20"
  )

  string(APPEND CMAKE_CXX_FLAGS_DEBUG " /Od /DDEBUG_MODE")
  string(APPEND CMAKE_CXX_FLAGS_RELEASE " /O2 /DNDEBUG /DRELEASE_MODE /D_ITERATOR_DEBUG_LEVEL=1")
else()
  string(
    APPEND
    CMAKE_CXX_FLAGS
    " -Werror -Wall -Wextra -Wstrict-aliasing -Wcast-align -Wmissing-include-dirs -Wpointer-arith -Wunreachable-code -Wunused -Wunused-parameter"
  )

  string(APPEND CMAKE_CXX_FLAGS_DEBUG " -g -O0 -DDEBUG_MODE")
  string(APPEND CMAKE_CXX_FLAGS_RELEASE " -g -O3 -DNDEBUG -DRELEASE_MODE")
endif()

set(APP_NAME graph_app)

set(
  HEADERS
  include/algo/a_star/algorithm.hpp
  include/algo/a_star/expand.hpp
  include/algo/a_star/generate_new_paths.hpp
  include/algo/a_star/insert.hpp
  include/algo/a_star/path.hpp
  include/algo/a_star/vertex_with_cost.hpp
  include/algo/dijkstra/insert.hpp
  include/algo/dijkstra/vertex_with_priority.hpp
  include/algo/fleury/is_bridge.hpp
  include/algo/fleury/reachable_vertices_from.hpp
  include/algo/ford_fulkerson/algorithm.hpp
  include/algo/ford_fulkerson/breadth_first_search.hpp
  include/algo/bellman_ford.hpp
  include/algo/shortest_paths.hpp
  include/algo/hierholzer.hpp
  include/nm/create_graph.hpp
  include/nm/heuristic.hpp
  include/romania/city.hpp
  include/romania/create_graph.hpp
  include/romania/heuristic.hpp
  include/adjacency_list.hpp
  include/adjacency_matrix.hpp
  include/assert.hpp
  include/assertion_violation_exception.hpp
  include/build_error_message.hpp
  include/directionality.hpp
  include/edge.hpp
  include/graph_exception.hpp
  include/graph.hpp
  include/pretty_function.hpp
  include/string_format.hpp
  include/vertex_identifier.hpp
)

set(
  SOURCES
  src/algo/a_star/generate_new_paths.cpp
  src/algo/a_star/path.cpp
  src/algo/a_star/vertex_with_cost.cpp
  src/algo/dijkstra/insert.cpp
  src/algo/dijkstra/vertex_with_priority.cpp
  src/algo/ford_fulkerson/algorithm.cpp
  src/algo/ford_fulkerson/breadth_first_search.cpp
  src/algo/hierholzer.cpp
  src/algo/shortest_paths.cpp
  src/nm/heuristic.cpp
  src/romania/city.cpp
  src/romania/heuristic.cpp
  src/test/algo/fleury/fleury_test.cpp
  src/test/algo/fleury/is_bridge_test.cpp
  src/test/algo/fleury/reachable_vertices_from_test.cpp
  src/test/algo/a_star_test.cpp
  src/test/algo/bellman_ford_test.cpp
  src/test/algo/dijkstra_test.cpp
  src/test/algo/ford_fulkerson_test.cpp
  src/test/algo/hierholzer_test.cpp
  src/test/adjacency_list_test.cpp
  src/test/adjacency_matrix_test.cpp
  src/test/directionality_test.cpp
  src/test/edge_test.cpp
  src/test/graph_test.cpp
  src/adjacency_list.cpp
  src/adjacency_matrix.cpp
  src/assertion_violation_exception.cpp
  src/build_error_message.cpp
  src/directionality.cpp
  src/edge.cpp
  src/graph_exception.cpp
  src/main.cpp
  src/string_format.cpp
)

add_executable(${APP_NAME} ${HEADERS} ${SOURCES})

target_compile_definitions(${APP_NAME} PRIVATE TESTING)

target_include_directories(
  ${APP_NAME} 
  PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/external/doctest/doctest)
