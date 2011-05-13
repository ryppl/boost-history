// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_AUX_UTILITY_HPP
#define BOOST_MGL_AUX_UTILITY_HPP

#include <boost/mpl/if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/insert.hpp>

namespace boost
{

namespace mgl
{

namespace aux
{

template<class Row>
struct row_start_vertex
{
	typedef typename Row::vertex type;
};

template<class Row>
struct row_adjacency_vertices
{
	typedef typename Row::adjacency_vertices type;
};

template<class Vertex>
struct get_node_id
{
	typedef typename Vertex::id type;

	enum { value = type::value };
};

template<class T>
struct wrap
{};

template<class Set, class Vec>
struct vec_to_set
{
	typedef typename ::boost::mpl::fold<
		Vec,
		Set,
		::boost::mpl::if_<
			::boost::mpl::has_key< ::boost::mpl::placeholders::_1, ::boost::mpl::placeholders::_2>,
			::boost::mpl::placeholders::_1,
			::boost::mpl::insert< ::boost::mpl::placeholders::_1, ::boost::mpl::placeholders::_2>
		>
	>::type type;
};

} // namespace aux

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_AUX_UTILITY_HPP
