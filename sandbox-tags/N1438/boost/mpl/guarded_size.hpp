//-----------------------------------------------------------------------------
// boost mpl/guarded_size.hpp header file
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

#ifndef BOOST_MPL_GUARDED_SIZE_HPP_INCLUDED
#define BOOST_MPL_GUARDED_SIZE_HPP_INCLUDED

#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/guarded_distance.hpp"
#include "boost/mpl/integral_c.hpp"
#include "boost/mpl/sequence_tag.hpp"
#include "boost/mpl/aux_/void_spec.hpp"
#include "boost/mpl/aux_/lambda_support.hpp"

namespace boost {
namespace mpl {

template < typename Tag >
struct guarded_size_algorithm_traits
{
    template < typename Sequence, typename Size > struct algorithm
    {
        typedef typename guarded_distance<
			  typename begin<Sequence>::type
			, typename end<Sequence>::type
			, Size
			>::type type;
    };
};

template <typename Sequence, unsigned long Size>
struct guarded_size_c
{
private:
    typedef typename sequence_tag<Sequence>::type tag_;
    typedef integral_c<unsigned long, Size> size_;

public:
    typedef typename guarded_size_algorithm_traits<tag_>
        ::template algorithm< Sequence,size_ >::type type;
};

template <
      typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Sequence)
    , typename BOOST_MPL_AUX_VOID_SPEC_PARAM(Size)
    >
struct guarded_size
{
private:
    typedef typename sequence_tag<Sequence>::type tag_;

public:
    typedef typename guarded_size_algorithm_traits<tag_>
        ::template algorithm< Sequence,Size >::type type;

    BOOST_MPL_AUX_LAMBDA_SUPPORT(2,guarded_size,(Sequence,Size))
};

BOOST_MPL_AUX_VOID_SPEC(2, guarded_size)

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_GUARDED_SIZE_HPP_INCLUDED
