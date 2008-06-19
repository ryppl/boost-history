/**
 * \file boost/mirror/meta_path/at.hpp
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_PATH_AT_HPP
#define BOOST_MIRROR_META_PATH_AT_HPP

#include <boost/mirror/meta_path/node_context.hpp>
#include <boost/mpl/at.hpp>

namespace boost {
namespace mirror {

namespace meta_path {

/** Returns the i-th node in the nodeset
 */
template <class NodeSet, class Position>
struct at : public mpl::at<typename NodeSet::nodes, Position> { };

} // namespace meta_path
} // namespace mirror
} // namespace boost

#endif //include guard

