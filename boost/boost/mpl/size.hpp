//-----------------------------------------------------------------------------
// boost mpl/size.hpp header file
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

#ifndef BOOST_MPL_SIZE_HPP
#define BOOST_MPL_SIZE_HPP

#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/distance.hpp"
#include "boost/mpl/sequence_traits.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

template<typename SequenceTag>
struct size_algorithm_traits
{
    template<typename Sequence> struct algorithm
    {
     private:
        typedef typename mpl::begin<Sequence>::iterator first_;
        typedef typename mpl::end<Sequence>::iterator last_;

     public:
        BOOST_STATIC_CONSTANT(long, value = 
            (mpl::distance<first_,last_>::value)
            );
    };
};

template<typename Sequence>
struct size 
    : size_algorithm_traits< 
          typename mpl::sequence_traits<Sequence>::sequence_category
          >::template algorithm<Sequence> 
{
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_SIZE_HPP
