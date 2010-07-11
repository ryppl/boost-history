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
    typedef typename get_create_parameter<\
        BitfieldTuple,N>::is_valid_index fixer_type ## N;

/** Defines typedefs which prevent the MSVC9 compiler from failing where it 
 *  shouldn't
 *  Creates the following pattern
 *      typedef typename get_create_parameter<
 *          BitfieldTuple,0>::is_valid_index    fixer_type0;
 *        ...
 *      typedef typename get_create_parameter<
 *          BitfieldTuple,N>::is_valid_index    fixer_typeN;
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


/** Used to help generate psudo variadic template paramerer apperance.
 *  This make the typename T##N = mpl::void_ pattern.
 */
#define BOOST_MAKE_BFT_PARAM(z, n, data ) \
    typename T ## n = mpl::void_*

/** Makes the T0 through TN parameter list for the 
 *  Generates pattern.
 *  typename T0 = mpl::void_*, ... typename TN = mpl::void_*
 */
#define BOOST_MAKE_BFT_TEMPLATE_PARAMETERS() \
    BOOST_PP_ENUM(  BOOST_BFT_PARAM_COUNT,\
                    BOOST_MAKE_BFT_PARAM, \
                    BOOST_BFT_NOTHING )


/** Used for generating a single funciton parameter for the make_bitfield_tuple
 *  function template.
 */
#define BOOST_MAKE_BFT_FUNCTION_PARAM(z, n, data ) \
    T ## n parameter ## n = 0

/** used for generating the parameters for a the make_bitfield_tuple
 *  funciton.
 *  Generates pattern
 *  T0 = 0, ... TN = 0 
 */
#define BOOST_MAKE_BFT_FUNCTION_PARAMETERS() \
    BOOST_PP_ENUM(  BOOST_BFT_PARAM_COUNT,\
                    BOOST_MAKE_BFT_FUNCTION_PARAM, \
                    BOOST_BFT_NOTHING )

#define BOOST_MAKE_BFT_ASSIGN_FUNCTION_CALL(z,n,data)\
    detail::assign_parameter_to_bft<n>(bft, parameter##n);

#define BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS() \
    BOOST_PP_REPEAT_FROM_TO( \
        0,\
        BOOST_BFT_PARAM_COUNT, \
        BOOST_MAKE_BFT_ASSIGN_FUNCTION_CALL,\
        BOOST_BFT_NOTHING )


template <typename BitfieldTuple, BOOST_MAKE_BFT_TEMPLATE_PARAMETERS()>
BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS()) {
    BitfieldTuple bft;
    BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS();
    return bft;
}


}}} // end boost::detail::msvc_fixes
#endif
