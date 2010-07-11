//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BFT_MAKE_BFT_HPP
#define BOOST_BFT_MAKE_BFT_HPP
#include <boost/config.hpp>
#include <cstddef>


namespace boost { namespace detail {
#ifdef BOOST_MSVC
    
template <typename T>
struct bft_create_param_fix_return_type {
    typedef T type;
};

template <typename ReturnType, typename NameType, typename Offset, typename Width>
struct bft_create_param_fix_return_type< bitfield_element<ReturnType,NameType,Offset,Width> >{
    typedef ReturnType type;
};



template <typename BitfieldTuple, std::size_t Index>
struct get_create_parameter {
    // check for valid Index.
    typedef typename mpl::less<
        mpl::size_t<Index>,
        typename mpl::size<
            typename BitfieldTuple::members
        >::type
    >::type                 is_valid_index;
    
    typedef typename mpl::if_<
        is_valid_index,
        typename mpl::at_c<
            typename BitfieldTuple::members,
            Index
        >::type,
        mpl::void_*
    >::type                 element_type;

    typedef typename bft_create_param_fix_return_type<element_type>::type type;
};

typedef ::boost::bitfield_tuple< bitfield::storage<int> > fixer_tuple;

/** Used for fixing an inadequacy in MSVC9 which causes my make_bitfield_tuple
 *  function template parameters not to be instantiated unless they are
 *  typedef'ed peior to the function declaration.
 */
#define BOOST_MAKE_BFT_HELPER_TYPEDEF(Z, N, DATA) \
    typedef get_create_parameter<fixer_tuple,N>::is_valid_index fixer_type ## N;

/** Defines typedefs which prevent the MSVC9 compiler from failing where it 
 *  shouldn't
 *  Creates the following pattern
 *      typedef 
 */
#define BOOST_MAKE_BFT_COMPENSATE_FOR_MSVC() \
        BOOST_PP_REPEAT_FROM_TO( \
            0,\
            BOOST_BFT_PARAM_COUNT, \
            BOOST_MAKE_BFT_HELPER_TYPEDEF,\
            BOOST_BFT_NOTHING )

BOOST_MAKE_BFT_COMPENSATE_FOR_MSVC()

// get_create_parameter<test_tuple,0>::is_valid_index
/*
BOOST_MPL_ASSERT(( get_create_parameter<test_tuple,1>::is_valid_index ));
BOOST_MPL_ASSERT(( get_create_parameter<test_tuple,2>::is_valid_index ));
BOOST_MPL_ASSERT(( get_create_parameter<test_tuple,3>::is_valid_index ));
BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,4>::is_valid_index ));
BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,5>::is_valid_index ));
BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,6>::is_valid_index ));
BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,7>::is_valid_index ));
BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,8>::is_valid_index ));
BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,9>::is_valid_index ));
BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,10>::is_valid_index ));
BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,11>::is_valid_index ));
BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,12>::is_valid_index ));
BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,13>::is_valid_index ));
BOOST_MPL_ASSERT_NOT(( get_create_parameter<test_tuple,14>::is_valid_index ));
*/

#else

/** Used to generate a function parameter for the create function used
 *  with make_bitfield_tuple
 */
template <typename T>
struct bft_create_param_fix_return_type {
    typedef typename T::return_type type;
};

template <>
struct bft_create_param_fix_return_type< mpl::void_ > {
    typedef mpl::void_*  type;
};


template <typename BitfieldTuple, std::size_t Index>
struct get_create_parameter {
    typedef typename mpl::if_<
        typename mpl::less<
            mpl::size_t<Index>,
            typename mpl::size<
                typename BitfieldTuple::members
            >::type
        >::type,
        typename bft_create_param_fix_return_type<
            typename mpl::at_c<
                typename BitfieldTuple::members,
                Index
            >::type
        >::type,
        mpl::void_*
    >::type                     type;
};

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

} // end detail

/** I really don't like having to do this but it really is the Only way
 *  its possible (by using macros thats is)
 *  This is used to create parameters for the static call function
 *  inside of make_bitfield_tuple.
 */
#define BOOST_MAKE_BITFIELD_TUPLE_SINGLE_PARAMETER(z, n, data ) \
    typename detail::get_create_parameter<BitfieldTuple, n>::type \
    parameter ## n = 0


/** This macro is used for creating the list of parameters inside of the
 *  make_bitfield_tuple's create function.
 *  Creates the following pattern
 *  From 0 to n
 *      typename detail::get_create_parameter<BitfieldTuple, 0>::type \
 *    parameter0 = 0, ... 
 *      typename detail::get_create_parameter<BitfieldTuple, n>::type \
 *    parametern = 0 
 */
#define BOOST_MAKE_BITFIELD_TUPLE_CREATE_FUNCTION_PARAMETERS() \
    BOOST_PP_ENUM(  BOOST_BFT_PARAM_COUNT,\
                    BOOST_MAKE_BITFIELD_TUPLE_SINGLE_PARAMETER, \
                    BOOST_BFT_NOTHING )


#define BOOST_MAKE_BITFIELD_TUPLE_ASSIGN_PARAMETER_TO_BFT_CALL(z,n,data)\
    detail::assign_parameter_to_bft<n>(bft, parameter##n);

#define BOOST_MAKE_BITFIELD_TUPLE_CREATE_FUNCTION_PARSE_ARGUMENTS() \
    BOOST_PP_REPEAT_FROM_TO( \
        0,\
        BOOST_BFT_PARAM_COUNT, \
        BOOST_MAKE_BITFIELD_TUPLE_ASSIGN_PARAMETER_TO_BFT_CALL,\
        BOOST_BFT_NOTHING )


/** This can be used so that I can deduce the correct types for the arguments of
 *  the function at compile time and make the function callible at run time
 *  and make that function type safe.
 *      This will need a macro in order to create the appearance of variadic
 *  behavior, although some of that will be taken care of by default values.
 */
/** the reason the name of the macro is so large is because the number of 
 *  parameters this function could possibly hold is as many as fifty. So
 *  I felt that the name should reflect that.
 */
template <typename BitfieldTuple>
BitfieldTuple make_bitfield_tuple(
    BOOST_MAKE_BITFIELD_TUPLE_CREATE_FUNCTION_PARAMETERS())
{
    BitfieldTuple bft;
    BOOST_MAKE_BITFIELD_TUPLE_CREATE_FUNCTION_PARSE_ARGUMENTS();
    return bft;
}

} // end boost

#endif
