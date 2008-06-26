/**
 * \file boost/mirror/meta_path/ancestors_and_self.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_PATH_ANCESTORS_AND_SELF_HPP
#define BOOST_MIRROR_META_PATH_ANCESTORS_AND_SELF_HPP

#include <boost/mirror/meta_path/ancestors.hpp>

namespace boost {
namespace mirror {
namespace meta_path {


/** A nodeset containing ancestor and self nodes of the
 *  given node context
 */
template <class NodeContext>
struct ancestors_and_self : node_set<
	detail::ancestors_base<NodeContext, mpl::true_> 
>
{
	typedef ancestors_and_self<NodeContext> type;
};

} // namespace meta_path
} // namespace mirror
} // namespace boost

#endif //include guard

