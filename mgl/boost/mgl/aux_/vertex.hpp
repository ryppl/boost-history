// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_AUX_VERTEX_HPP
#define BOOST_MGL_AUX_VERTEX_HPP

//#include <boost/mpl/int.hpp>

#include <boost/mgl/aux_/utility.hpp>

namespace boost
{

namespace mgl
{

namespace aux
{

template<class Type, class Neighbors/*, int Id*/>
struct vertex
{
	typedef Type type;
	typedef Neighbors neighbors;
	//typedef ::boost::mpl::int_<Id> id;
};

template<class Row>
struct row2vertex
{
	typedef vertex<
		typename aux::row_start_vertex<Row>::type,
		typename aux::row_adjacency_vertices<Row>::type
	> type;
};

} // namespace aux

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_AUX_VERTEX_HPP
