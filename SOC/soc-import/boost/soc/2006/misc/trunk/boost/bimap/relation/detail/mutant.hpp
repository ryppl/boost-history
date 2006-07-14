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

/// \brief A way to specify allowed views using derivation.
/**

A class can derived from this template to specify the views it supports.
Basically it is a safe way to use reinterpret_cast. If you are careful
with the allowed views the user of your mutant class wont be able to
break anything.
See also mutant, mutate.
\ingroup mutant_group
                                                                        **/

template< class ViewsContainer >
class can_mutate_in
{
    public: typedef ViewsContainer mutant_views;
};

/// \brief A wrapper to a default constructible type
///        that allows to specify views of the data.
/**

This template class allows to view an object from different perspectives.
The mutant type is that of the first template parameter. The second
parameter is an mpl container that specifies the views that will be
supported. Basically it is a safe wrapper around reinterpret_cast. If you
are careful with the allowed views the user of your mutant class wont be
able to break anything.
See also can_mutate_in, mutate.
\ingroup mutant_group
                                                                         **/

template< class Type, class ViewsContainer >
class mutant : public Type
{
    BOOST_CLASS_REQUIRE(Type, boost, DefaultConstructibleConcept);
    public: typedef ViewsContainer mutant_views;
};

//@{
/// \brief Converts a mutant class to a view with zero overhead.
/**

This function is a safe wrapper around reinterpret_cast. It checks at
compile time that the desired view is supported by the mutant class.
See also mutant, can_mutate_in.
\ingroup mutant_group
                                                                            **/
template< class View, class Type, class ViewsContainer >
View& mutate( mutant<Type,ViewsContainer> & m )
{
    BOOST_MPL_ASSERT(( boost::mpl::contains<ViewsContainer,View> ));
    BOOST_STATIC_ASSERT( sizeof(m) >= sizeof( View ) );
    return *reinterpret_cast< View* >(addressof(m));
}

template< class View, class Type, class ViewsContainer >
const View& mutate( const mutant<Type,ViewsContainer> & m )
{
    BOOST_MPL_ASSERT(( boost::mpl::contains<ViewsContainer,View> ));
    BOOST_STATIC_ASSERT( sizeof(m) >= sizeof( View ) );
    return *reinterpret_cast< const View* >(addressof(m));
}

template< class View, class T >
View& mutate( T & m )
{
    BOOST_MPL_ASSERT(( boost::mpl::contains<typename T::mutant_views,View> ));
    BOOST_STATIC_ASSERT( sizeof(m) >= sizeof( View ) );
    return *reinterpret_cast< View* >(addressof(m));
}

template< class View, class T >
const View& mutate( const T & m )
{
    BOOST_MPL_ASSERT(( boost::mpl::contains<typename T::mutant_views,View> ));
    BOOST_STATIC_ASSERT( sizeof(m) >= sizeof( View ) );
    return *reinterpret_cast< const View* >(addressof(m));
}

//@}

} // namespace detail
} // namespace relation
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_MUTANT_MUTATE_HPP

