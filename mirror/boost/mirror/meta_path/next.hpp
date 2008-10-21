/**
 * \file boost/mirror/meta_path/next.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_PATH_NEXT_HPP
#define BOOST_MIRROR_META_PATH_NEXT_HPP

#include <boost/mirror/meta_path/node_context.hpp>
#include <boost/mirror/meta_path/prior.hpp>
#include <boost/mirror/iterator/next.hpp>

namespace boost {
namespace mirror {
namespace meta_path {


/** A nodeset containing the next node of the
 *  given node context
 */
template <class NodeContext>
struct next : node_set<
	detail::neighbour_base<NodeContext, mirror::next> 
>
{
	typedef next<NodeContext> type;
};

} // namespace meta_path
} // namespace mirror
} // namespace boost

#endif //include guard

