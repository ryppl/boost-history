//-----------------------------------------------------------------------------
// boost mpl/back.hpp header file
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

#ifndef BOOST_MPL_BACK_HPP
#define BOOST_MPL_BACK_HPP

#include "boost/mpl/begin_end.hpp"
#include "boost/mpl/sequence_traits.hpp"
#include "boost/mpl/while_true.hpp"
#include "boost/mpl/utility/next.hpp"
#include "boost/mpl/compose/f_gx.hpp"
#include "boost/mpl/same_as.hpp"
#include "boost/type_traits/same_traits.hpp"
#include "boost/config.hpp"

namespace boost {
namespace mpl {

namespace detail {

// workaround of MSVC's early template instantiation bug
struct back_algorithm_traits_base
{
    template<typename Sequence> struct algorithm
    {
     private:
        typedef typename mpl::begin<Sequence>::iterator first_;
        typedef typename mpl::end<Sequence>::iterator last_;
        typedef typename mpl::while_true<
              mpl::compose_f_gx< mpl::not_same_as<last_>,mpl::next<mpl::_1> >
            , first_
            , mpl::next<mpl::_1>
            > loop_;
     
     public:                           
        typedef typename loop_::executed::state type;
    };
};
    
} // namespace detail 

template<typename SequenceTag>
struct back_algorithm_traits
    : mpl::detail::back_algorithm_traits_base
{
};

template<typename Sequence>
struct back
    : back_algorithm_traits< 
          typename mpl::sequence_traits<Sequence>::sequence_category
          >::template algorithm<Sequence>::type
{
};

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_BACK_HPP
