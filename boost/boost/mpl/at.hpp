//-----------------------------------------------------------------------------
// boost mpl/at.hpp header file
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

#ifndef BOOST_MPL_AT_HPP
#define BOOST_MPL_AT_HPP

#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/sequence_traits.hpp"
#include "boost/mpl/for_loop.hpp"
#include "boost/mpl/comparison/less.hpp"
#include "boost/mpl/identity/project1st.hpp"
#include "boost/mpl/compose/f_gxy.hpp"
#include "boost/mpl/utility/next.hpp"
#include "boost/mpl/int_t.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

#if defined(BOOST_MSVC) && (BOOST_MSVC > 1200) 
namespace aux {
// VC7.0 requires special implementation
template< long N >
struct at_impl
{
    template< typename Iterator > struct result_
    {
		typedef typename Iterator::next next_;
        typedef typename at_impl<N-1>
            ::template result_<next_>::type type;
    };
    
};

template<>
struct at_impl<0>
{
    template< typename Iterator > struct result_
    {
        typedef typename Iterator::type type;
    };
};

} // namespace aux 

template<typename SequenceTag>
struct at_algorithm_traits
{
    template<long N, typename Sequence> struct algorithm
    {
        typedef typename begin<Sequence>::iterator first_;
        typedef typename aux::at_impl<N>
            ::template result_<first_>::type type;
    };
};

template<long N, typename Sequence>
struct at
{
    typedef typename at_algorithm_traits< 
          typename mpl::sequence_traits<Sequence>::sequence_category
          >::template algorithm<N, Sequence>::type type;
};

#else

template<typename SequenceTag>
struct at_algorithm_traits
{
    template<long N, typename Sequence> struct algorithm
    {
     private:
        typedef typename mpl::begin<Sequence>::iterator first_;
        typedef typename mpl::for_loop< 
              mpl::int_t<0>
            , mpl::lt<N>
            , mpl::next<mpl::_1>
            , first_
            , mpl::compose_f_gxy<
                    mpl::next<mpl::placeholder>
                  , mpl::project1st<mpl::_1,mpl::_2>
                  >
            > loop_;

     public:
        typedef typename loop_::executed::state type;
    };
};

template<long N, typename Sequence>
struct at
    : at_algorithm_traits< 
          typename mpl::sequence_traits<Sequence>::sequence_category
          >::template algorithm<N, Sequence>::type
{
};

#endif // #if defined(BOOST_MSVC) && (BOOST_MSVC > 1200) 

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_AT_HPP
