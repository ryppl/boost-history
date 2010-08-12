//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_MEMBER_IMPL_HPP
#define BOOST_BITFIELD_MEMBER_IMPL_HPP
#include "bitfield_vector_base.hpp"
#include "mask_creator.hpp"
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/assert.hpp>
#include <cstring>
#include <boost/integer/low_bits_mask.hpp>
#include <boost/integer/bit_width.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <bitset>

namespace boost { namespace detail {

template <typename T>
std::string to_binary_2(T x) {
    std::stringstream ss(std::stringstream::in|std::stringstream::out);
    ss << std::setfill('0') << std::setw(boost::bit_width<T>::value)
        << std::bitset<  boost::bit_width<T>::value >(x).to_string();
    return std::string(ss.str());
}

template <std::size_t Width, bool = bool((Width%8) > 0)>
struct next_allocation_size;


// NOT divible by 8
template <std::size_t Width>
struct next_allocation_size<Width, true> {
    std::size_t compute_8_bit_boundry(std::size_t bits) {
        if((bits%8) == 0) {
            return bits;
        }else{
            return bits + std::size_t(8-(bits%8));
        }
    }

    typedef typename mpl::divides<
        typename mpl::plus<
            mpl::size_t<Width>,
            typename mpl::minus<
                mpl::size_t<8>,
                typename mpl::modulus<
                    mpl::size_t<Width>,
                    mpl::size_t<8>
                >::type
            >::type
        >::type,
        mpl::size_t<8>            
    >::type                 first_alloc_size;
        
        
    std::size_t operator()(std::ptrdiff_t current_size, std::size_t bits_used) {
        if(current_size == 0) {
            return std::size_t(first_alloc_size::value);
        }
        return compute_8_bit_boundry(bits_used*2)/8;
    }
};

// IS divible by 8
template <std::size_t Width>
struct next_allocation_size<Width,false> {
    typedef typename mpl::divides<
        mpl::size_t<Width>,
        mpl::size_t<8>
    >::type             first_return_value;

    std::size_t operator()(std::ptrdiff_t current_size, std::size_t) {
        if(current_size == 0) {
            return first_return_value::value;
        }else{
            return current_size * 2;
        }
    }
};


/** This is the proxy reference type used by the iterator and by the 
 *  bitfield_tuple for accessing the storage and correctly returning
 *  and setting the values within the bitfield_vector.
 */

template <  typename RetType,
            std::size_t Width,
            bool = is_signed<RetType>::type::value >
class proxy_reference_type;

template <typename RetType,std::size_t Width>
class proxy_reference_type<RetType,Width,true> {
    typedef proxy_reference_type<RetType,Width,true> _self;
    proxy_reference_type();
public:
    /** Typedefs and integral static constant values. */
    //@{
    typedef unsigned char       storage_type;
    typedef RetType             value_type;
    typedef std::size_t         offset_type;
    BOOST_STATIC_CONSTANT( std::size_t, width = Width );
    BOOST_STATIC_CONSTANT(value_type, sign_bit =
        (high_bits_mask<value_type,1>::value));
    /** constructors and destructor for the proxy_reference_type type. */
    //@{

    /** Copy Constructor. */
    explicit proxy_reference_type(_self const& x)
        :_ptr(x._ptr), _mask(x._mask )
    { }

    /** pointer, offset constructor. */
    explicit proxy_reference_type(storage_type* ptr, offset_type offset)
        :_ptr(ptr), _mask(get_mask_detail<Width>(offset) )
    { }
    //@}

    /** Copy assignment. */
    _self& operator=(_self const& x) {
        _ptr = x._ptr;
        _mask = x._mask;
        return *this;
    }
    /** Implicit conversion operator.*/
    operator value_type() const {
    }

    /** value_type storage assignement operator.*/
    _self& operator=(value_type x) {

    }

    bool operator==(_self const& rhs);
    bool operator!=(_self const& rhs);
    bool operator<(_self const& rhs);


    // private:
    /** Member variables. */
    storage_type*    _ptr;
    mask_detail     _mask;
};

/** Proxy reference type for unsigned types. */
template <typename RetType, std::size_t Width>
class proxy_reference_type<RetType,Width,false> {
    typedef proxy_reference_type<RetType,Width,false> _self;
    proxy_reference_type();
public:
    /** Typedefs and integral static constant values. */
    //@{
    typedef unsigned char       storage_type;
    typedef RetType             value_type;
    typedef std::size_t         offset_type;
    BOOST_STATIC_CONSTANT( std::size_t, width = Width );
    //@}
    
    /** constructors and destructor for the proxy_reference_type type. */
    //@{

    /** Copy Constructor. */
    explicit proxy_reference_type(_self const& x)
        :_ptr(x._ptr), _mask(x._mask)
    { }

    /** pointer, offset constructor. */
    explicit proxy_reference_type(storage_type* ptr, offset_type offset)
        :_ptr(ptr), _mask(get_mask_detail<Width>(offset))
        
    { }

    //@}

    /** Copy assignment. */
    _self& operator=(_self const& x) {
        _ptr = x._ptr;
        _mask = x._mask;
        return *this;
    }

    /** Implicit Conversion Operator*/
    operator value_type() const {
        if(_mask._size == 1) {
            return (value_type( _mask._first_byte & *_ptr ) >>
                _mask._last_shift);
        }

        value_type ret = 0;
        storage_ptr_t byte_ptr = _ptr;

        if(_mask._size == 2) {
            ++byte_ptr;
            byte_ptr = _ptr;
            // getting first bits.
            ret = value_type(_mask._first_byte & *byte_ptr)
                << ( 8 - _mask._last_shift );
            ++byte_ptr;
            value_type retrieved_value;
            if( _mask._last_byte == 0xFF) {
                ret <<= 8;
                ret += value_type(*byte_ptr);
            }else{
                retrieved_value = (value_type( _mask._last_byte)
                    & value_type(*byte_ptr));
                retrieved_value >>= _mask._last_shift;
                ret += retrieved_value;
            }
            return ret;
        }
        
        const storage_t all_bits = 0xFF;
        // gettting first byte.

        ret = value_type( _mask._first_byte & *byte_ptr);
        ++byte_ptr;
        
        // getting middle bytes
        for(std::size_t index = 0; index < _mask._size - 2; ++index) {
            ret <<= 8;
            ret += *byte_ptr & all_bits;
            ++byte_ptr;
        }
        // shifting bits
        if(_mask._last_byte == 0xFF) {
            ret <<= 8;
            ret += value_type(*byte_ptr & _mask._last_byte);
        }else{
            ret <<= 8 - _mask._last_shift;
            ret += value_type( *byte_ptr & _mask._last_byte ) >> ( _mask._last_shift);
        }
        return ret;
    }

    /** value_type storage assignement operator.*/
    _self& operator=(value_type x) {
        if(_mask._size == 1) {
            storage_t previous_values = *_ptr & ~_mask._first_byte;
            storage_t new_value = low_bits_mask<value_type, width>::value & x;
            new_value <<= _mask._last_shift;
            previous_values |= new_value;
            *_ptr = previous_values;
            return *this;
        }
        
        storage_t       to_be_stored = 0;
        std::size_t     bits_in_mask = 0;
        value_type      mask         = 0;
        storage_ptr_t   byte_ptr     = _ptr;

        if(_mask._size == 2) {
            bits_in_mask = 8 - _mask._offset;
            mask = (~(~value_type(0) << bits_in_mask))
                << (width - bits_in_mask);
            to_be_stored = storage_t((mask&x)>>(width - bits_in_mask));
            *byte_ptr = ((*byte_ptr) & (~_mask._first_byte)) | to_be_stored;
            
            ++byte_ptr;
            bits_in_mask = width - bits_in_mask;
            mask = ~(~value_type(0) << bits_in_mask);
            to_be_stored = storage_t((mask & x) << _mask._last_shift);
            *byte_ptr = (*byte_ptr & ~_mask._last_byte) | to_be_stored;
            return *this;
        }
        // calculate the offset of the first bit within x
        // and creating a mask to extract the fist bits from within x
        bits_in_mask = 8 - _mask._offset;
        mask = _mask._first_byte;
        mask <<= width - bits_in_mask;

        *byte_ptr = (*byte_ptr & ~_mask._first_byte) | ((x & mask ) >> (width - bits_in_mask));
        ++byte_ptr;
        mask = 0xFF;
        mask <<= width - bits_in_mask - 8;
        for(std::size_t index = 0; index < _mask._size - 2;++index) {
            *byte_ptr = (mask & x) >> (width - (bits_in_mask + (8 * index))- 8);
            mask >>= 8;
            ++byte_ptr;
        }
        // now calculating the last bytes information, retrieving it and then
        // storing the data within the array.
        if(_mask._last_byte == 0xFF) {
            *byte_ptr = _mask._last_byte & x;
        }else{
            mask = _mask._last_byte >> _mask._last_shift;
            *byte_ptr = (*byte_ptr & ~_mask._last_byte) |
                    ((mask & x) << (_mask._last_shift));
        }
        return *this;
    }

    bool operator==(_self const& rhs);
    bool operator!=(_self const& rhs);
    bool operator<(_self const& rhs);


// private:
    /** Member variables. */
    storage_type*   _ptr;
    mask_detail     _mask;
};




}} // end boost::detail


#endif
