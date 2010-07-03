//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>


#ifndef BOOST_BITFIELD_TUPLE_TEMPLATE_EXPANSION_MACROS_HPP
#define BOOST_BITFIELD_TUPLE_TEMPLATE_EXPANSION_MACROS_HPP

// need to handle all cases where the user could/can specify any combonation
// of BOOST_MPL_LIMIT_VECTOR_SIZE prior to including this file.

#ifndef BOOST_BFT_PARAM_COUNT
#   ifndef BOOST_MPL_LIMIT_VECTOR_SIZE
#       define BOOST_BFT_PARAM_COUNT 10
#   else
#       if BOOST_MPL_LIMIT_VECTOR_SIZE < 10
#           error "BOOST_MPL_LIMIT_VECTOR_SIZE must be set to higher then 10 if "
#           error "or BOOST_BFT_PARAM_COUNT must be defined to be equal to "
#           error "BOOST_MPL_LIMIT_VECTOR_SIZE if you wish to use bitfield_tuple."
#       else
#           define BOOST_BFT_PARAM_COUNT 10
#       endif
#   endif
#else
#   ifndef BOOST_MPL_LIMIT_VECTOR_SIZE
#       if BOOST_BFT_PARAM_COUNT <= 10
#           define BOOST_MPL_LIMIT_VECTOR_SIZE 10
#       elif BOOST_BFT_PARAM_COUNT <= 20
#           define BOOST_MPL_LIMIT_VECTOR_SIZE 20
#       elif BOOST_BFT_PARAM_COUNT <= 30
#           define BOOST_MPL_LIMIT_VECTOR_SIZE 30
#       elif BOOST_BFT_PARAM_COUNT <= 40
#           define BOOST_MPL_LIMIT_VECTOR_SIZE 40
#       elif BOOST_BFT_PARAM_COUNT <= 50
#           define BOOST_MPL_LIMIT_VECTOR_SIZE 50
#       else
#           error "BOOST_BFT_PARAM_COUNT of value greater then 50 not supported."
#       endif
#   else
#       if BOOST_BFT_PARAM_COUNT <= BOOST_MPL_LIMIT_VECTOR_SIZE
#       else
#           error "BOOST_MPL_LIMIT_VECTOR_SIZE must be greater then or equal to"
#           error "BOOST_BFT_PARAM_COUNT."
#       endif
#   endif
#endif




/** Creates variadic tempalte behavior by creating parameters up to
 *  BOOST_BFT_PARAM_COUNT in the fasion of typename TN = mpl::void_, ... 
 *  where N is the Nth parameter in the parameter list.
 *
 *  Creates the following pattern
 *      typename T0 = mpl::void_, ... typename TN = mpl::void_
 */
#define BOOST_MAKE_BFT_TEMPLATE_PARAMS() \
            BOOST_PP_ENUM(  BOOST_BFT_PARAM_COUNT,\
                            BOOST_BFT_MAKE_PARAMS, \
                            BOOST_BFT_NOTHING )

/** Macro for generating template parameters
 *  for a bitfield_tuple.
 */
#define BOOST_BFT_MAKE_PARAMS(Z, N, DATA) \
            typename T ## N = mpl::void_


/** Macro for generating the code for unpacking a the template parameters for 
 *  later processing inside of the bitfield_tuple base.
 */
#define BOOST_BFT_UNPACK_CALL(Z, N, DATA) \
                ::template process<T ## N >::type

/** Defines code for the unpacking and parsing of template parameters.
 *  This is used within the bft_base_impl.
 *  Creates the following pattern
 *      ::template process<T1>::type ... ::template process<TN>::type
 */
#define BOOST_BFT_UNPACK_PARAMS() \
        BOOST_PP_REPEAT_FROM_TO( \
            1,\
            BOOST_BFT_PARAM_COUNT, \
            BOOST_BFT_UNPACK_CALL,\
            BOOST_BFT_NOTHING )



/** Helps produce the sequence T0, ... TN, both with and with out typename 
 * before it.
 */
#define BOOST_BFT_PARAM_LIST(Z,N,DATA) \
    DATA ## N

/** This is used for making a list of all of the template parameters and that 
 *  list of comma seperated parameters is then used to name the template within
 *  itself or to pass all of the template arguments at once.
 *
 *  Creates the following pattern
 *     T0, ... TN
 */
#define BOOST_BFT_PARAMETER_LIST() \
            BOOST_PP_ENUM(  BOOST_BFT_PARAM_COUNT,\
                            BOOST_BFT_PARAM_LIST, \
                            T )

/** Creates the template parameters for the bft base or impl class.
 *  Creates the following pattern
 *      typename T0, ... typename TN
 */
#define BOOST_BFT_BASE_TEMPLATE_PARAMS() \
    BOOST_PP_ENUM(  BOOST_BFT_PARAM_COUNT,\
                    BOOST_BFT_PARAM_LIST, \
                    typename T )

/** Used for creating template parameters for specilization.
 *  Creates the following pattern
 *      typename T0, ... typename TN
 */
#define BOOST_BFT_TEMPLATE_ARGS_NO_DEFAULTS() \
    BOOST_PP_ENUM(  BOOST_BFT_PARAM_COUNT,\
                    BOOST_BFT_PARAM_LIST, \
                    typename T )


/** Used for specilizing over the bitfield_tuple sequence.
 *  Creates the following pattern
 *      T0, ... TN
 */
#define BOOST_BFT_SPECILIZATION_PARAM_LIST() \
                BOOST_BFT_PARAMETER_LIST()



    

#endif
