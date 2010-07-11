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
    typename T ## n

/** Makes the T0 through TN parameter list for the 
 *  Generates pattern.
 *  typename T0, ... typename TN
 */
#define BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(N) \
    BOOST_PP_ENUM(  N,\
                    BOOST_MAKE_BFT_PARAM, \
                    BOOST_BFT_NOTHING )


/** Used for generating a single funciton parameter for the make_bitfield_tuple
 *  function template.
 */
#define BOOST_MAKE_BFT_FUNCTION_PARAM(z, n, data ) \
    T ## n parameter ## n

/** used for generating the parameters for a the make_bitfield_tuple
 *  funciton.
 *  Generates pattern
 *  T0 parameter0 , ... TN parametern 
 */
#define BOOST_MAKE_BFT_FUNCTION_PARAMETERS(N) \
    BOOST_PP_ENUM(  N,\
                    BOOST_MAKE_BFT_FUNCTION_PARAM, \
                    BOOST_BFT_NOTHING )

#define BOOST_MAKE_BFT_ASSIGN_FUNCTION_CALL(z,n,data)\
    detail::assign_parameter_to_bft<n>(bft, parameter##n);

#define BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS( TO ) \
    BOOST_PP_REPEAT_FROM_TO( \
        0,\
        TO, \
        BOOST_MAKE_BFT_ASSIGN_FUNCTION_CALL,\
        BOOST_BFT_NOTHING )


#define BOOST_MAKE_BFT_FUNCTION(Z, N, DATA)                                   \
    template <typename BitfieldTuple, BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(N)>  \
    BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS(N)){ \
        BitfieldTuple bft;                                                    \
        BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(N);                          \
        return bft;                                                           \
    }

#define BOOST_MAKE_BFT_FUNCTION_OVERLOADS() \
    BOOST_PP_REPEAT_FROM_TO( \
        1,\
        BOOST_BFT_PARAM_COUNT, \
        BOOST_MAKE_BFT_ASSIGN_FUNCTION_CALL,\
        BOOST_BFT_NOTHING )





}}

/* template <typename BitfieldTuple,BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(0)>
BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS(0)) {
    BitfieldTuple bft;
    BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(9)
    return bft;
}
*/
template <typename BitfieldTuple,BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(2)>
BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS(2)) {
    BitfieldTuple bft;
    BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(2);
    return bft;
}

template <typename BitfieldTuple,BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(3)>
BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS(3)) {
    BitfieldTuple bft;
    BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(3);
    return bft;
}

template <typename BitfieldTuple,BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(4)>
BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS(4)) {
    BitfieldTuple bft;
    BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(4)
    return bft;
}

template <typename BitfieldTuple,BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(5)>
BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS(5)) {
    BitfieldTuple bft;
    BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(5);
    return bft;
}

template <typename BitfieldTuple,BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(6)>
BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS(6)) {
    BitfieldTuple bft;
    BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(6);
    return bft;
}

template <typename BitfieldTuple,BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(7)>
BitfieldTuple make_bitfield_tuple( BOOST_MAKE_BFT_FUNCTION_PARAMETERS(7) ) {
    BitfieldTuple bft;
    BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(7);
    return bft;
}

template <typename BitfieldTuple,BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(8)>
BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS(8)) {
    BitfieldTuple bft;
    BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(8);
    return bft;
}
template <typename BitfieldTuple,BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(9)>
BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS(9)) {
    BitfieldTuple bft;
    BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(9);
    return bft;
}

template <typename BitfieldTuple,BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(10)>
BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS(10)) {
    BitfieldTuple bft;
    BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(10)
    return bft;
}



} // end boost::detail::msvc_fixes
#endif
