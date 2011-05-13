// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_ROW_HPP
#define BOOST_MGL_ROW_HPP

namespace boost
{

namespace mgl
{

template<class Vertex, class VertexList>
struct row
{
	typedef Vertex vertex;
	typedef VertexList adjacency_vertices;
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_ROW_HPP
