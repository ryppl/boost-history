/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_INDEX_ITERATOR_FWD_HPP
#define BOOST_INDEXED_SET_INDEX_ITERATOR_FWD_HPP

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

namespace boost{

namespace indexed_sets{

namespace detail{

#if defined(BOOST_INDEXED_SET_ENABLE_SAFE_MODE)
#if BOOST_WORKAROUND(BOOST_MSVC,<1300)
template<typename Node>
class index_iterator;
#else
template<typename Node,typename Container>
class index_iterator;
#endif
#else
template<typename Node>
class index_iterator;
#endif

} /* namespace indexed_sets::detail */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
