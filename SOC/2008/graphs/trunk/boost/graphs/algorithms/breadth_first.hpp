
#ifndef ALGORITHMS_BREADTH_FIRST_HPP
#define ALGORITHMS_BREADTH_FIRST_HPP

#include <boost/graphs/properties.hpp>
#include <boost/graphs/colors.hpp>

// Possible concept-related notions:
// VertexWalk: A walk whose current state yields vertices.
// EdgeWalk: A walk whose current state yields edges. Edge walks must also
// provide edge-like interfaces (i.e., source/target) functions in order to
// disambiguate the unordering of undirected graphs.

#include <boost/graphs/algorithms/breadth_first/vertex_walk.hpp>
#include <boost/graphs/algorithms/breadth_first/edge_walk.hpp>

#endif
