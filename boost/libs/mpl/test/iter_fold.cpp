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
#include <cassert>
#include <algorithm>
#include <cstddef>

#if defined(BOOST_INTEL_CXX_VERSION) && BOOST_INTEL_CXX_VERSION < 600
# pragma warning (disable: 985) // identifier was truncated in debug information
#endif 
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
        
        static void execute(std::size_t** x)
        {
            *(*x)++ = sizeof(X);
            *(*x)++ = sizeof(Y);
        }
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

        static void execute(std::size_t** x)
        {
            *(*x)++ = sizeof(X);
            *(*x)++ = sizeof(Y);
        }
    };
};


int main()
{
    typedef mpl::type_list<char[2], char[4], char[8], char[10]> types;

    std::size_t sizes1[9] = {0};
    std::size_t const sizes1_expected[9] = {
        1, 2
        , 1, 4
        , 4, 8
        , 4, 10
        , 0
    };

    
    std::size_t sizes2[17] = {0};
    std::size_t const sizes2_expected[17] = {
        1, 2
        , 1, 4
        , 4, 8
        , 4, 10
        
        , 10, 4
        , 8, 4
        , 4, 8
        , 2, 8
        , 0
    };
    
    typedef mpl::iter_fold<types, char, choose1> loop1;
    std::size_t* out = sizes1;
    loop1::execute(&out);
    assert(std::equal(sizes1, sizes1 + 9, sizes1_expected));
    
    typedef mpl::iter_fold<types, char, choose1, choose2> loop2;

    std::size_t* out2 = sizes2;
    loop2::execute(&out2);
    assert(std::equal(sizes2, sizes2 + 17, sizes2_expected));
    
    BOOST_STATIC_ASSERT((boost::is_same<loop1::type,char[4]>::value));
    BOOST_STATIC_ASSERT((boost::is_same<loop2::type,char[8]>::value));
    
    return 0;
}
