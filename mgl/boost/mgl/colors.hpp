// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_COLORS_HPP
#define BOOST_MGL_COLORS_HPP

#include <boost/mpl/erase_key.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/mpl/at.hpp>

namespace boost
{

namespace mgl
{

struct white {};
struct gray  {};
struct black {};

template<typename Vertex, typename Color, typename Map>
struct set_color
{
	typedef typename ::boost::mpl::erase_key<
		Map,
		Vertex
	>::type map;

	typedef typename ::boost::mpl::insert<
		map,
		::boost::mpl::pair<Vertex, Color>
	>::type type;
};

template<typename Vertex, typename Map>
struct get_color
{
	typedef typename ::boost::mpl::if_<
		typename ::boost::mpl::has_key<Map, Vertex>::type,
		typename ::boost::mpl::at<Map, Vertex>::type,
		white
	>::type type;
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_COLORS_HPP
