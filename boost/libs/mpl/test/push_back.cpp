
// + file: libs/mpl/test/push_back.cpp
// + last modified: 10/jun/03

// Copyright (c) 2002-03
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.
//
// See http://www.boost.org/libs/mpl for documentation.

#include "boost/mpl/push_back.hpp"
#include "boost/mpl/push_front.hpp"
#include "boost/type_traits/is_same.hpp"
#include "boost/static_assert.hpp"

using namespace boost;
using namespace mpl;

struct seq1 { struct tag; };
struct seq2 { struct tag; };

namespace boost { namespace mpl {
template<> struct push_back_impl<seq1::tag>
{
    template< typename Seq, typename T > struct apply
    {
        typedef Seq type;
    };
};

template<> struct push_front_impl<seq2::tag>
{
    template< typename Seq, typename T > struct apply
    {
        typedef Seq type;
    };
};
}}

int main()
{
    BOOST_STATIC_ASSERT(( is_same< push_back<seq1,int>::type, seq1 >::value ));
    BOOST_STATIC_ASSERT(( is_same< push_front<seq2,int>::type, seq2 >::value ));
    BOOST_STATIC_ASSERT(( has_push_back<seq1>::value ));
    BOOST_STATIC_ASSERT(( !has_push_back<seq2>::value ));
    BOOST_STATIC_ASSERT(( !has_push_front<seq1>::value ));
    BOOST_STATIC_ASSERT(( has_push_front<seq2>::value ));

    // should work on non-sequence types as well
    BOOST_STATIC_ASSERT(( !has_push_back<int>::value )); 
    BOOST_STATIC_ASSERT(( !has_push_front<int>::value )); 
    
    return 0;
}
