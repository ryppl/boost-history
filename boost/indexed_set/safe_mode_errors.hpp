/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_SAFE_MODE_ERRORS_HPP
#define BOOST_INDEXED_SET_SAFE_MODE_ERRORS_HPP

namespace boost{

namespace indexed_sets{

namespace safe_mode{

/* Error codes for Boost.IndexedSet safe mode. These go in a separate
 * header so that the user can include it when redefining
 * BOOST_INDEXED_SET_SAFE_MODE_ASSERT prior to the inclusion of
 * any other header of Boost.IndexedSet.
 */

enum error_code
{
  invalid_iterator=0,
  not_dereferenceable_iterator,
  not_incrementable_iterator,
  not_decrementable_iterator,
  not_owner,
  not_same_owner,
  invalid_range,
  inside_range,
  same_container
};

} /* namespace indexed_sets::safe_mode */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
