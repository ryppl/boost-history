/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_INDEX_FWD_HPP
#define BOOST_INDEXED_SET_INDEX_FWD_HPP

#include <boost/indexed_set/index_args.hpp>

namespace boost{

namespace indexed_sets{

namespace detail{

template<
  typename KeyFromValue,typename Compare,
  typename Super,typename TagList,typename Category
>
class index;

} /* namespace indexed_sets::detail */

/* index specifiers */

template<typename Arg1,typename Arg2=detail::null_arg,typename Arg3=detail::null_arg>
struct unique;

template<typename Arg1,typename Arg2=detail::null_arg,typename Arg3=detail::null_arg>
struct non_unique;

} /* namespace indexed_sets */

} /* namespace boost */

#endif
