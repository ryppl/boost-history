// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MGL_NEXT_PRIOR_HPP
#define BOOST_MGL_NEXT_PRIOR_HPP

#include <boost/mgl/aux_/next_prior_impl.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_void.hpp>

namespace boost
{

namespace mgl
{

/// \brief Move iterator to the next vertex
/// \param Iterator Iterator that should be moved
template<class Iterator, class Enable = void>
struct next
{
	typedef typename aux::next_impl<Iterator>::template apply<Iterator>::type type;
};

template<class Iterator>
struct next<Iterator, typename ::boost::enable_if< ::boost::is_same<Iterator, ::boost::mpl::void_> >::type>
{
	typedef ::boost::mpl::void_ type;
};

/// \brief Move iterator to the previous vertex
/// \param Iterator Iterator that should be moved
template<class Iterator>
struct prior
{
	typedef typename aux::prior_impl<Iterator>::template apply<Iterator>::type type;
};

template<class Iterator>
struct edge_next
{
	typedef typename aux::edge_next_impl<Iterator>::template apply<Iterator>::type type;
};

} // namespace mgl

} // namespace boost

#endif // BOOST_MGL_NEXT_PRIOR_HPP
