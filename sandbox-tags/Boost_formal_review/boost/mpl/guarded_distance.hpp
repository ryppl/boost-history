//-----------------------------------------------------------------------------
// boost mpl/guarded_distance.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002
// Eric Friedman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_GUARDED_DISTANCE_HPP_INCLUDED
#define BOOST_MPL_GUARDED_DISTANCE_HPP_INCLUDED

#include "boost/type_traits/is_same.hpp"

#include "boost/mpl/apply.hpp"
#include "boost/mpl/apply_if.hpp"
#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/distance.hpp"
#include "boost/mpl/identity.hpp"
#include "boost/mpl/if.hpp"
#include "boost/mpl/integral_c.hpp"
#include "boost/mpl/next.hpp"
#include "boost/mpl/arithmetic/plus.hpp"
#include "boost/mpl/iterator_category.hpp"
#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/mpl/aux_/lambda_support.hpp"

namespace boost {
namespace mpl {

namespace aux {

template <unsigned long N>
struct guarded_distance_impl
{
    template <typename Iterator, typename LastIterator>
    struct apply
    {
    private:
        typedef is_same<Iterator, LastIterator>
            is_last;
        typedef typename apply_if<
              is_last
            , identity<Iterator>
            , next<Iterator>
            >::type next_iterator;

        typedef typename apply_if<
              is_last
            , identity< integral_c<unsigned long, 0> >
            , apply2<
                  guarded_distance_impl<N-1>
                , next_iterator
                , LastIterator
                >
            >::type remaining_distance;

    public:
        typedef typename plus<
              remaining_distance
            , typename if_<
                  is_last
                , integral_c<unsigned long, 0>
                , integral_c<unsigned long, 1>
                >::type
            >::type type;
    };
};

template <>
struct guarded_distance_impl<0>
{
    template <typename Iterator, typename LastIterator>
    struct apply
    {
        typedef integral_c<unsigned long, 0>
            type;
    };
};

} // namespace aux

template < typename Category >
struct guarded_distance_algorithm_traits
{
    template < typename First, typename Last, typename Distance > struct algorithm
    {
        typedef typename apply2<
              aux::guarded_distance_impl<Distance::value>
            , First
            , Last
            >::type type;
    };
};

template <typename First, typename Last, unsigned long Distance>
struct guarded_distance_c
{
private:
    typedef typename iterator_category<First>::type category_;
    typedef integral_c<unsigned long, Distance> distance_;

public:
    typedef typename guarded_distance_algorithm_traits<category_>
        ::template algorithm< First,Last,distance_ >::type type;
};

template <
	  typename BOOST_MPL_AUX_VOID_SPEC_PARAM(First)
	, typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Last)
	, typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Distance)
	>
struct guarded_distance
{
private:
    typedef typename iterator_category<First>::type category_;

public:
    typedef typename guarded_distance_algorithm_traits<category_>
        ::template algorithm< First,Last,Distance >::type type;

    BOOST_MPL_AUX_LAMBDA_SUPPORT(3,guarded_distance,(First,Last,Distance))
};

BOOST_MPL_AUX_VOID_SPEC(3, guarded_distance)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_GUARDED_DISTANCE_HPP_INCLUDED
