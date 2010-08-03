
#ifndef BOOST_MPL_JOIN_PACK_HPP_INCLUDED
#define BOOST_MPL_JOIN_PACK_HPP_INCLUDED

// $Id$
// $Date: 2010/02/13 15:31:06 $
// $Revision: 1.1 $

#include <boost/mpl/package.hpp>

namespace boost { namespace mpl {

template<
      typename Sequence1_
    , typename Sequence2_
    >
struct join_pack
;
template<
      typename... Elements1_
    , typename... Elements2_
    >
struct join_pack<
      package<Elements1_...>
    , package<Elements2_...>
    >

{
    typedef package< Elements1_..., Elements2_...> type;
};

BOOST_MPL_AUX_NA_SPEC(2, join_pack)

}}

#endif // BOOST_MPL_JOIN_PACK_HPP_INCLUDED
