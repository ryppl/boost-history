//-----------------------------------------------------------------------------
// boost mpl/begin_end.hpp header file
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

#ifndef BOOST_MPL_BEGIN_END_HPP
#define BOOST_MPL_BEGIN_END_HPP

#include "boost/mpl/sequence_traits.hpp"

namespace boost {
namespace mpl {

template<typename SequenceTag>
struct begin_algorithm_traits
{
    template<typename Sequence> struct algorithm
    {
        typedef typename Sequence::begin iterator;
    };
};

template<typename Sequence>
struct begin
    : begin_algorithm_traits< 
          typename mpl::sequence_traits<Sequence>::sequence_category
          >::template algorithm<Sequence> 
{
};

template<typename SequenceTag>
struct end_algorithm_traits
{
    template<typename Sequence> struct algorithm
    {
        typedef typename Sequence::end iterator;
    };
};

template<typename Sequence>
struct end
    : end_algorithm_traits< 
          typename mpl::sequence_traits<Sequence>::sequence_category
          >::template algorithm<Sequence> 
{
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_BEGIN_END_HPP
