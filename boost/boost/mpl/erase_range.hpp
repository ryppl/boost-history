//-----------------------------------------------------------------------------
// boost mpl/erase_range.hpp header file
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

#ifndef BOOST_MPL_ERASE_RANGE_HPP
#define BOOST_MPL_ERASE_RANGE_HPP

#include "boost/mpl/clear.hpp"
#include "boost/mpl/copy.hpp"
#include "boost/mpl/clear.hpp"
#include "boost/mpl/iterator_range.hpp"
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/sequence_traits.hpp"

namespace boost {
namespace mpl {

template<typename SequenceTag>
struct erase_range_algorithm_traits
{
    template<
          typename Sequence
        , typename FirstIterator
        , typename LastIterator
        >
    struct algorithm
    {
     private:
        typedef typename mpl::begin<Sequence>::iterator first_;
        typedef typename mpl::end<Sequence>::iterator last_;
        
        typedef typename mpl::copy<
              mpl::iterator_range<first_,FirstIterator>
            , typename mpl::clear<Sequence>::sequence
            >::sequence part1_;
                                  
     public:
        typedef typename mpl::copy<
              mpl::iterator_range<LastIterator,last_>
            , part1_
            >::sequence sequence;
    };
};


template<
      typename Sequence
    , typename FirstIterator
    , typename LastIterator
    >
struct erase_range
    : erase_range_algorithm_traits<
          typename mpl::sequence_traits<Sequence>::sequence_category
          >::template algorithm<Sequence, FirstIterator, LastIterator>
{
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_ERASE_RANGE_HPP
