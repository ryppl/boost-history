//----------------------------------------------------------------------------
// boost mpl/push_back.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//----------------------------------------------------------------------------
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

#ifndef BOOST_MPL_PUSH_BACK_HPP
#define BOOST_MPL_PUSH_BACK_HPP

#include "boost/mpl/sequence_traits.hpp"
#include "boost/mpl/et.hpp"

namespace boost {
namespace mpl {

template<typename SequenceTag>
struct push_back_algorithm_traits
{
    template<typename Sequence, typename T> struct algorithm;
};

template<typename Sequence, typename T>
struct push_back
{
 private:
    typedef typename push_back_algorithm_traits<
          typename mpl::sequence_traits<Sequence>::sequence_category
          >::template algorithm<Sequence, T> algorithm_;
 public:
    typedef typename algorithm_::sequence sequence;
    typedef typename algorithm_::sequence type;
};

// limited forms of ET, just for now
template<>
struct push_back<mpl::_1,mpl::_2>
{
    template<typename U1, typename U2> struct apply
        : push_back<U1,U2>
    {
    };
};

template<>
struct push_back<mpl::_2,mpl::_1>
{
    template<typename U1, typename U2> struct apply
        : push_back<U2,U1>
    {
    };
};

template<>
struct push_back<mpl::placeholder,mpl::placeholder>
{
    template<typename U1, typename U2> struct apply
        : push_back<U1,U2>
    {
    };
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_PUSH_BACK_HPP
