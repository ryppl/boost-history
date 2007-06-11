// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file relation/support/get_pair_functor.hpp
/// \brief get_pair_functor definition

#ifndef BOOST_BIMAP_RELATION_SUPPORT_GET_PAIR_FUNCTOR_HPP
#define BOOST_BIMAP_RELATION_SUPPORT_GET_PAIR_FUNCTOR_HPP

#include <boost/bimap/relation/support/pair_by.hpp>

namespace boost {
namespace bimap {
namespace relation {
namespace support {

/// \brief A Functor that takes a relation as a parameter an return the desired view.
/**

This functor is included to help users of the relation class when using
stl algorithms.

See also member_at, pair_by().
\ingroup relation_group

                                                                                  **/

template< class Tag, class Relation >
struct get_pair_functor
{
    typename result_of::pair_by<Tag,Relation>::type
    operator()( Relation & r ) const
    {
        return pair_by<Tag>(r);
    }

    typename result_of::pair_by<Tag,const Relation>::type
    operator()( const Relation & r ) const
    {
        return pair_by<Tag>(r);
    }
};


/// \brief A Functor that takes a relation as a parameter an return the above view.
/**

\ingroup relation_group
                                                                                  **/

template< class Relation >
struct get_above_view_functor
{
    typename Relation::above_view_reference
    operator()( Relation & r ) const
    {
        return r.get_view();
    }

    typename Relation::const_above_view_reference
    operator()( const Relation & r ) const
    {
        return r.get_view();
    }
};

} // namespace support
} // namespace relation
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_RELATION_SUPPORT_GET_PAIR_FUNCTOR_HPP

