//-----------------------------------------------------------------------------
// boost mpl/contains.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_CONTAINS_HPP
#define BOOST_MPL_CONTAINS_HPP

#include "boost/mpl/find.hpp"
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/sequence_traits.hpp"
#include "boost/type_traits/same_traits.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<typename SequenceTag>
struct contains_algorithm_traits
{
    template<typename Sequence, typename T> struct algorithm
    {
        BOOST_STATIC_CONSTANT(bool, value = (!::boost::is_same<
              typename mpl::find<Sequence, T>::iterator
            , typename mpl::end<Sequence>::iterator
            >::value)
        );
    };
};

template<typename Sequence, typename T>
struct contains
    : contains_algorithm_traits< 
          typename mpl::sequence_traits<Sequence>::sequence_category
          >::template algorithm<Sequence, T>
{
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_CONTAINS_HPP
