// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file relation/support/get_pair_functor.hpp
/// \brief GetPairFunctor definition

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
struct GetPairFunctor
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


} // namespace support
} // namespace relation
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_RELATION_SUPPORT_GET_PAIR_FUNCTOR_HPP

