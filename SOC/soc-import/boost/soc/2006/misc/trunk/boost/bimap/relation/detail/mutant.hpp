// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file relation/detail/mutant.hpp
/// \brief Mutate functions to extract views of mutant classes.

#ifndef BOOST_BIMAP_RELATION_DETAIL_MUTANT_HPP
#define BOOST_BIMAP_RELATION_DETAIL_MUTANT_HPP

#include <boost/concept_check.hpp>
#include <boost/bimap/detail/debug/static_error.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/utility.hpp>
#include <boost/static_assert.hpp>

namespace boost {
namespace bimap {
namespace relation {

/// \brief Relation details, mutant idiom and symmetrical metafunctions builders.

namespace detail {

//@{
/// \brief Converts a mutant class to a view with zero overhead.
/**

This function is a safe wrapper around reinterpret_cast. It checks at
compile time that the desired view is supported by the mutant class.
See also mutant, can_mutate_in.
\ingroup mutant_group
                                                                            **/
template< class View, class Type >
View& mutate( Type & m )
{
    BOOST_MPL_ASSERT(( boost::mpl::contains<typename Type::mutant_views,View> ));
    BOOST_STATIC_ASSERT( sizeof(Type) == sizeof(View) );
    return *reinterpret_cast< View* >(addressof(m));
}

template< class View, class Type >
const View& mutate( const Type & m )
{
    BOOST_MPL_ASSERT(( boost::mpl::contains<typename Type::mutant_views,View> ));
    BOOST_STATIC_ASSERT( sizeof(Type) == sizeof(View) );
    return *reinterpret_cast< const View* >(addressof(m));
}

//@}

} // namespace detail
} // namespace relation
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_RELATION_DETAIL_MUTANT_HPP

