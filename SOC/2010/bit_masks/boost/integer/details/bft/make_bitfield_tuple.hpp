//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BFT_MAKE_BFT_HPP
#define BOOST_BFT_MAKE_BFT_HPP
#include <cstddef>
#include <boost/mpl/bitwise.hpp>

namespace boost { namespace details {

/** maks a bitfield out of the NewValue which is aligned to the 0 bit of the 
 *  return type specified in BitfieldElement.
 */
template <  typename NewValue,
            typename BitfieldElement,
            typename NewValueIsSigned
>
struct bitfield_from_value;

/** Specilization: signed types */
template <typename NewValue,typename BitfieldElement>
struct bitfield_from_value <NewValue, BitfieldElement, mpl::true_> {
    // step one is to get the sign bit and then shift it to the correct
    // position within an integral constant so that I can work
    // with it directly and encode it into a bitfield.
    typedef typename mpl::bitand_<
        NewValue,
        typename high_bits_mask<
            typename BitfieldElement::return_type,
            1
        >::type
    >::type                         sign_bit;
/*
    typedef typename mpl::bitand_<
        typename BitfieldElement::return_type,
        BitfieldElement::field_width::value -1
    >::type                         value_bits;
    
    typedef typename mpl::right_shift<
        sign_bit,
        typename mpl::minus<
            typename 
        >::type
*/
};

/** Specilization: unsigned types */
template <typename NewValue,typename BitfieldElement>
struct bitfield_from_value <NewValue, BitfieldElement, mpl::false_> {
    // this bit wise ands the value of the field out of the value.
    typedef typename mpl::bitand_<
        NewValue,
        typename low_bits_mask<
            typename BitfieldElement::return_type,
            BitfieldElement::field_width::value
        >::type
    >::type                     bitfield_value;

};

/** Complex operation operation for storing data into an integral value that 
 *  as a bitfield on compile time. This meta-function is used to help
 *  store data into a bitfield_data structure and then encodit into the storage
 *  type of a data a bitfield_tuple, which can be used to construct it.
 *
 *  Currently only supporting native endianness.
 */
template <  typename Storage,
            typename NewValue,
            typename BitfieldElem
>
struct set_bitfield_in_value {

    // preform correct encoding of the type, basically what I'm doing is
    // masking off the value for a type and then if its signed i get the 
    // sign byte the store that within storage using bitwise or operation.

    typedef typename is_signed<
        typename NewValue::value_type
    >::type                             value_is_signed;

    typedef typename bit_width<
        typename NewValue::value_type
    >::type                             size_of_return_type_in_bits;


    
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
        typename mpl::at_c<
            typename BitfieldTuple::members,
            Index
        >::type::return_type,
        mpl::void_*
    >::type         type;
};

} // end details

/** There are 2 types of functions used for creating bitfields.
 *  The first is a static implementation where by the initial value of
 *  the data is set at compile time. The second is a runtime create bitfield
 *  where by the values are set at runtime.
 */


/** Static create bitfield.
 *  Compile time initilization of a bitfield tuple. This creates a bitfield
 *  with an initial value of that is specified on compile time.
 *  This will be a TODO at a later time because its complicated and I want to
 *  finish the other one first.
 */
template <typename T> struct static_make_bitfield_tuple;

#define ham
/** Function signature for make_bitfield_tuple.
 *      way lay this for a bit because I have a better ideas
 */
// template <typename BitfieldTuple>
// BitfieldTuple make_bitfield_tuple( );



/** This can be used so that I can deduce the correct types for the arguments of
 *  the function at compile time and make the function callible at run time
 *  and make that function type safe.
 *      This will need a macro in order to create the appearance of variadic
 *  behavior, although some of that will be taken care of by default values.
 */
template <typename BitfieldTuple>
struct make_bitfield_tuple {

    static BitfieldTuple create(  );
};

} // end boost

#endif
