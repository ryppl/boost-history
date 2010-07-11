//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BFT_MSVC9_MAKE_BFT_FIX_HPP
#define BOOST_BFT_MSVC9_MAKE_BFT_FIX_HPP

namespace boost { namespace detail { namespace msvc_fixes {
/** Used for fixing an inadequacy in MSVC9 which causes my make_bitfield_tuple
 *  function template parameters not to be instantiated unless they are
 *  typedef'ed peior to the function declaration.
 */
#define BOOST_MAKE_BFT_HELPER_TYPEDEF(Z, N, DATA) \
    typedef get_create_parameter<BitfieldTuple,N>::is_valid_index fixer_type ## N;

/** Defines typedefs which prevent the MSVC9 compiler from failing where it 
 *  shouldn't
 *  Creates the following pattern
 *      typedef get_create_parameter<BitfieldTuple,0>::is_valid_index fixer_type0;
 *        ...
 *      typedef get_create_parameter<BitfieldTuple,N>::is_valid_index fixer_typeN;
 */
#define BOOST_MAKE_BFT_COMPENSATE_FOR_MSVC() \
        BOOST_PP_REPEAT_FROM_TO( \
            0,\
            BOOST_BFT_PARAM_COUNT, \
            BOOST_MAKE_BFT_HELPER_TYPEDEF,\
            BOOST_BFT_NOTHING )

template <typename BitfieldTuple>
struct msvc9_make_bft_fix {
    BOOST_MAKE_BFT_COMPENSATE_FOR_MSVC()
};

}}} // end boost::detail::msvc_fixes
#endif
