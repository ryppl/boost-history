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

namespace boost { namespace detail {

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
    /** constructors and destructor for the proxy_reference_type type. */
    //@{

    /** Copy Constructor. */
    proxy_reference_type(_self const& x)
        :_ptr(x._ptr), _mask(x._mask )
    { }

    /** pointer, offset constructor. */
    proxy_reference_type(storage_type* ptr, offset_type offset)
        :_ptr(ptr), _mask(get_mask_detail<Width>(offset) )
    { }
    //@}

    /** Copy assignment. */
    _self& operator=(_self const& x) {
        _ptr = x._ptr;
        _mask = x._mask;
        return *this;
    }

    operator value_type() const;
    /** value_type storage assignement operator.*/
    _self& operator=(value_type x);

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
    proxy_reference_type(_self const& x)
        :_ptr(x._ptr), _mask(x._mask)
    { }

    /** pointer, offset constructor. */
    proxy_reference_type(storage_type* ptr, offset_type offset)
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
                (8 - (_mask._offset + width)));
        }

        value_type ret = 0;
        storage_ptr_t byte_ptr = _ptr;

        if(_mask._size == 2) {
            ret = value_type(_mask._first_byte & *byte_ptr) <<
                 _mask._last_shift;

            ++byte_ptr;
            ret += value_type( _mask._last_byte & *byte_ptr) >>
                (8 - _mask._last_shift);
            if( _mask._last_byte != 0xFF) {
                ret >>= _mask._last_shift - 1;
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

        ret <<= 8 - _mask._last_shift;
        ret += value_type( *byte_ptr & _mask._last_byte ) >> ( _mask._last_shift);
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
        
        if(_mask._size == 2) {
            storage_t to_be_stored = 0;
            std::size_t bits_in_first_mask = 8 - _mask._offset;
            std::cout << "bits_in_first_mask: " << bits_in_first_mask << std::endl;
            value_type first_mask = (~(~value_type(0) << bits_in_first_mask))
                << (width - bits_in_first_mask - 1);
            typedef unsigned long long ullt;
            std::cout << "first_mask: " << std::hex << ullt(first_mask) << std::endl;
            to_be_stored = storage_t((first_mask&x)>>(width - bits_in_first_mask - 1));
            std::cout << "to be stored: " << std::hex << ullt(to_be_stored) << std::endl;
            storage_ptr_t byte_ptr = _ptr;
            
            *byte_ptr = (*byte_ptr & ~_mask._first_byte) | to_be_stored;
            ++byte_ptr;
            std::size_t bits_in_mask = width - bits_in_first_mask;
            std::cout << "bits in second mask: " << bits_in_mask << std::endl;
            value_type second_mask = ~(~value_type(0) << bits_in_mask);
            std::cout << "second mask: " << std::hex << ullt(second_mask) << std::endl;
            to_be_stored = storage_t((second_mask & x) << _mask._last_shift);
            std::cout << "to be stored: " << std::hex << ullt(to_be_stored) << std::endl;
            *byte_ptr = (*byte_ptr & ~_mask._last_byte) | to_be_stored;

            return *this;
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
