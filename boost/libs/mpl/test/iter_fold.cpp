//-----------------------------------------------------------------------------
// boost mpl/test/iter_fold.cpp source file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// David Abrahams
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#include <boost/mpl/iter_fold.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/same_traits.hpp>
#include <boost/mpl/aux_/pred.hpp>
#include <boost/mpl/select_type.hpp>
#include <boost/mpl/type_list.hpp>

namespace mpl = boost::mpl;

struct choose1
{
    template <class X, class YIter>
    struct apply
    {
        typedef typename YIter::type Y;
        typedef typename mpl::select_type<
            (3 * sizeof(X) > sizeof(Y))
            , X
            , Y>::type type;
    };
};

struct choose2
{
    template <class XIter, class Y>
    struct apply
    {
        typedef typename XIter::type X;
        typedef typename mpl::select_type<
            (sizeof(X) == 2*sizeof(Y))
            , X
            , Y>::type type;
    };
};


template <class T> struct undefined;

int main()
{

    typedef mpl::type_list<char[2], char[4], char[8], char[10]> types;
    
    typedef mpl::iter_fold<
          types
        , char
        , choose1
        >::type result1;

    typedef mpl::iter_fold<
          types
        , char
        , choose1
        , choose2
        >::type result2;

    BOOST_STATIC_ASSERT((boost::is_same<result1,char[4]>::value));
    BOOST_STATIC_ASSERT((boost::is_same<result2,char[8]>::value));
}
