//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_MEMBER_IMPL_HPP
#define BOOST_BITFIELD_MEMBER_IMPL_HPP
#include "bitfield_vector_base.hpp"
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <iostream>
#include <iomanip>
#include <cstring>

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
        :_ptr(x._ptr), _offset(x._offset)
    { }

    /** pointer, offset constructor. */
    proxy_reference_type(storage_type* ptr, offset_type offset)
        :_ptr(ptr), _offset(offset)
    { }
    //@}

    /** Copy assignment. */
    _self& operator=(_self const& x) {
        _ptr = x._ptr;
        _offset = x._offset;
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
    offset_type     _offset;
};


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
        :_ptr(x._ptr), _offset(x._offset)
    { }

    /** pointer, offset constructor. */
    proxy_reference_type(storage_type* ptr, offset_type offset)
        :_ptr(ptr), _offset(offset)
    { }
    //@}

    /** Copy assignment. */
    _self& operator=(_self const& x) {
        _ptr = x._ptr;
        _offset = x._offset;
        return *this;
    }

    /** Implicit Conversion Operator*/
    operator value_type() const {
        value_type ret = 0;
        storage_type* byte_ptr = _ptr;
        // std::size_t remaining_bits = width;
        storage_type mask = 0;

        // constructing mask inside of char array.
        // TODO: Make this correctly deduced! as of right it is the largest
        // possible size.
        storage_type mask_array[9];
        std::memset(mask_array, 0,9);


        // this is so that the loop will work correctly the first time through.
        storage_type* mask_ptr = mask_array;
        --mask_ptr;
        mask = 0x80;
        mask >>= _offset;
        std::size_t bit_copy_ammount = _offset + width;
        std::size_t trailing_zeros = 8 - _offset;
        std::size_t mask_byte_count = 0;
        for(std::size_t bit_index = _offset;bit_index <= bit_copy_ammount; ++bit_index){
            if( (bit_index%8) == 0) {
                ++mask_byte_count;
                trailing_zeros = 8;
                mask >>= 1;
                *mask_ptr |= mask;
                ++mask_ptr;
                mask = 0x80;
                continue;
            }
            --trailing_zeros;
            *mask_ptr |= mask;
            mask >>= 1;
        }

        // mask_ptr = mask_array;
        // storage_type* mask_array_end = (mask_array) + 9;
        
        for( std::size_t mask_index = 0;
             mask_index < mask_byte_count;
             ++mask_index)
        {
            ret <<= 8;
            ret += *byte_ptr & mask_array[mask_index];
            ++byte_ptr;
        }       
        return ret >> trailing_zeros;
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
    offset_type     _offset;
};

}} // end boost::detail


#endif
