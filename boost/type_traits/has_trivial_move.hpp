
// (C) Copyright Eric Friedman 2002.
// Permission to copy, use, modify, sell and distribute this software is 
// granted provided this copyright notice appears in all copies. This software 
// is provided "as is" without express or implied warranty, and with no claim 
// as to its suitability for any purpose.
//
// See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TT_HAS_TRIVIAL_MOVE_HPP_INCLUDED
#define BOOST_TT_HAS_TRIVIAL_MOVE_HPP_INCLUDED

#include "boost/type_traits/has_trivial_copy.hpp"
#include "boost/type_traits/has_trivial_assign.hpp"

#include "boost/mpl/logical/and.hpp"
#include "boost/mpl/logical/or.hpp"

// should be the last #include
#include "boost/type_traits/detail/bool_trait_def.hpp"

namespace boost {

// TRAIT: has_trivial_move

BOOST_TT_AUX_BOOL_TRAIT_DEF1(
      has_trivial_move
    , T
    , (::boost::mpl::logical_and<
          has_trivial_copy<T>
        , has_trivial_assign<T>
        >::type::value
      )
    )


// TRAIT: has_trivial_move_constructor

BOOST_TT_AUX_BOOL_TRAIT_DEF1(
      has_trivial_move_constructor
    , T
    , (::boost::mpl::logical_or<
          has_trivial_move<T>
        , has_trivial_copy<T>
        >::type::value
      )
    )


// TRAIT: has_trivial_move_assign

BOOST_TT_AUX_BOOL_TRAIT_DEF1(
      has_trivial_move_assign
    , T
    , (::boost::mpl::logical_or<
          has_trivial_move<T>
        , has_trivial_assign<T>
        >::type::value
      )
    )

} // namespace boost

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif // BOOST_TT_HAS_TRIVIAL_MOVE_HPP_INCLUDED
