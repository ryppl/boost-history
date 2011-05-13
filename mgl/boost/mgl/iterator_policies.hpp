// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_ITERATOR_POLICIES_HPP
#define BOOST_MGL_ITERATOR_POLICIES_HPP

#include <boost/mpl/vector.hpp>

#include <boost/typeof/typeof.hpp>

namespace boost
{

namespace mgl
{

// policies for the DFS iterator
struct EndWhenNoVerticesFound {};
struct EndAtEndOfGraph {};

// trace policies
struct NoTrace
{
	typedef ::boost::mpl::void_ container_type;

	template<typename Vertex, typename Container>
    struct apply
    {
		typedef ::boost::mpl::void_ type;
    };
};

template<typename TraceVector>
struct RecordTrace
{
	typedef TraceVector container_type;

	template<typename Vertex, typename Container>
    struct apply
    {
		typedef typename ::boost::mpl::push_back<Container, Vertex>::type type;
    };
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_ITERATOR_POLICIES_HPP
