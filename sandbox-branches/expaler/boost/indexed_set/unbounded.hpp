/* Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#ifndef BOOST_INDEXED_SET_UNBOUNDED_HPP
#define BOOST_INDEXED_SET_UNBOUNDED_HPP

namespace boost{

namespace indexed_sets{

/* dummy type and variable for use in index::range() */

namespace detail{

struct unbounded_type{};

} /* namespace indexed_sets::detail */

namespace{

detail::unbounded_type  unbounded_obj=detail::unbounded_type();
detail::unbounded_type& unbounded=unbounded_obj;

} /* unnamed */

} /* namespace indexed_sets */

} /* namespace boost */

#endif
