#ifndef BOOST_MPL_AT_FWD_HPP_INCLUDED
#define BOOST_MPL_AT_FWD_HPP_INCLUDED
//
// Copyright Aleksey Gurtovoy 2000-2004
//
//ChangeLog:
//  2007-01-29
//    WHAT:
//      1) Added at_impl fwd decl.Copied then modified from boost/mpl/at_fwd.hpp.
//    WHY:
//      1) Needed by mpl/set/aux_/at_impl.hpp
//  2006-09-30
//    WHAT:
//      1) Copied then modified from boost/mpl/at_fwd.hpp.
//      2) Added fwd decl for at_c from boost/mpl/at.hpp.
//    WHY:
//      1) See ../../README.txt
//      2) To allow at_c to be specialized in ./list.hpp
//         while still just including the at_fwd.hpp within
//         ./list.hpp.
//
#include <boost/mpl/aux_/nttp_decl.hpp>
namespace boost { namespace mpl {

template< typename Tag > struct at_impl;
template< typename Sequence, typename N > struct at;

template<
      typename Sequence
    , BOOST_MPL_AUX_NTTP_DECL(long, N)
    >
struct at_c
;
    
}}

#endif // BOOST_MPL_AT_FWD_HPP_INCLUDED
