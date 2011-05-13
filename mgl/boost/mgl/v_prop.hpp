// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_V_PROP_HPP
#define BOOST_MGL_V_PROP_HPP

#include <boost/mpl/empty_base.hpp>

namespace boost
{

namespace mgl
{

template<class T, class Property = ::boost::mpl::empty_base>
struct v_prop
{
	typedef T vertex;
	typedef Property property;
};

template<class T>
struct v_prop_vertex
{
	typedef typename T::vertex type;
};

template<class T>
struct v_prop_property
{
	typedef typename T::property type;
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_V_PROP_HPP
