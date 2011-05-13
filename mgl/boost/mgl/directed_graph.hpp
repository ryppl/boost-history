// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_DIRECTED_GRAPH_HPP
#define BOOST_MGL_DIRECTED_GRAPH_HPP

#include <boost/mgl/graph.hpp>
#include <boost/mgl/graph_policies.hpp>

namespace boost
{

namespace mgl
{

template<class Derived>
class directed_graph : public graph<Derived, VmapDirectedGraph> {};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_DIRECTED_GRAPH_HPP
