//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BFT_MAKE_BFT_HPP
#define BOOST_BFT_MAKE_BFT_HPP
#include <boost/config.hpp>
#include <cstddef>

namespace boost { namespace bitfields{ namespace detail {

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

/** Helps create functions to assign values into the bitfield tuple.
 *  Generates Pattern
 *  detail::assign_parameter_to_bft<n>(bft, parameter##n);
 */
#define BOOST_MAKE_BFT_ASSIGN_FUNCTION_CALL(z,n,data)\
    detail::assign_parameter_to_bft<n>(bft, parameter##n);


/** Helps create a sequence of functions to assign values into the
 *  bitfield_tuple.
 *  Generates Pattern
 *  detail::assign_parameter_to_bft<0>(bft, parameter0); ...
 *      detail::assign_parameter_to_bft<TO>(bft, parameter## TO);
 */
#define BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS( TO ) \
    BOOST_PP_REPEAT_FROM_TO( \
        0,\
        TO, \
        BOOST_MAKE_BFT_ASSIGN_FUNCTION_CALL,\
        BOOST_BFT_NOTHING )


/** Helps create a make bitfield_tuple function for the bitfield_tuple.
 *  Generates Pattern
 *  template <typename BitfieldTuple, BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(N)>
 *  BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS(N)) {
 *      BitfieldTuple bft;
 *      BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(N);
 *      return bft;
 *   }
 */
#define BOOST_MAKE_BFT_FUNCTION(z, N, data)                                   \
    template <typename BitfieldTuple, BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(N)>  \
    BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS(N)){ \
        BitfieldTuple bft;                                                    \
        BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(N);                          \
        return bft;                                                           \
    }

/** Helps create a make a sequence of make_bitfield_tuple functons for the
 *  bitfield_tuple.
 *  Generates Pattern
 *  template <typename BitfieldTuple, BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(0)>
 *  BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS(0)) {
 *      BitfieldTuple bft;
 *      BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(0);
 *      return bft;
 *   }
 *      ...
 *  template <typename BitfieldTuple, BOOST_MAKE_BFT_TEMPLATE_PARAMETERS(N)>
 *  BitfieldTuple make_bitfield_tuple(BOOST_MAKE_BFT_FUNCTION_PARAMETERS(N)) {
 *      BitfieldTuple bft;
 *      BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS(N);
 *      return bft;
 *   }
 */
#define BOOST_MAKE_BFT_OVERLOADS() \
    BOOST_PP_REPEAT_FROM_TO( \
        1,\
        BOOST_BFT_PARAM_COUNT, \
        BOOST_MAKE_BFT_FUNCTION,\
        BOOST_BFT_NOTHING )



#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4244)
#endif

/** This is a function which is used to assign a value into a bitfield_tuple
 *  as well as remove the actual mpl::void_* from the parameter list through
 *  specialization.
 */
template <std::size_t Index, typename BitfieldTuple, typename ParameterType>
inline void assign_parameter_to_bft(BitfieldTuple& bft, ParameterType value) {
    bft.template get<Index>() = value;
}

template <std::size_t Index, typename BitfieldTuple>
inline void assign_parameter_to_bft(BitfieldTuple&, mpl::void_*) { }

#if BOOST_MSVC
#pragma warning(pop)
#endif

} // end detail

BOOST_MAKE_BFT_OVERLOADS()

}} // end boost::bitfields
#undef BOOST_MAKE_BFT_PARAM
#undef BOOST_MAKE_BFT_TEMPLATE_PARAMETERS
#undef BOOST_MAKE_BFT_FUNCTION_PARAM
#undef BOOST_MAKE_BFT_ASSIGN_FUNCTION_CALL
#undef BOOST_MAKE_BFT_GENERATE_ASSIGNMENT_CALLS
#undef BOOST_MAKE_BFT_FUNCTION
#undef BOOST_MAKE_BFT_OVERLOADS

#endif

