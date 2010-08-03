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
template <typename ReturnType,std::size_t Width>
class proxy_reference_type {
    typedef proxy_reference_type<ReturnType,Width> _self;
    proxy_reference_type();
public:
    /** Typedefs and integral static constant values. */
    //@{
    typedef unsigned char       storage_type;
    typedef ReturnType          value_type;
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
        value_type ret;
        // std::size_t bit_index = 0;
        storage_type* byte_ptr = _ptr;
        std::size_t remaining_bits = width;

        // creating head mask.
        storage_type mask = ~(~storage_type(0) << (8 - _offset));
        // keep track of how many bits from the width have been extraced.
        remaining_bits -= (8 - _offset);
        // update return type.
        ret = value_type(mask & *byte_ptr) << (remaining_bits);
        // make sure to return if we are finished.
        if(remaining_bits == 0) {
            return ret;
        }
        // next loop while the
        ++byte_ptr;
        mask = ~storage_type(0);
        while((remaining_bits / 8) > 0) {
            ret |= value_type(mask & *byte_ptr) << (remaining_bits);
            
            // increment values so that everything is
            // correctly retrieved.
            remaining_bits -= 8;
            ++byte_ptr;
        }
        // because the field could have ended on a byte boundry then
        // I must then check before exiting.
        if(remaining_bits == 0) {
            return ret;
        }
        // if I reached this point it means that I must now deal with the
        // trailing bits of the field.
        mask = ~(~storage_type(0) >> remaining_bits);
        return ret | (value_type(mask & *byte_ptr)<<(remaining_bits));
    }

    /** value_type storage assignement operator.*/
    _self& operator=(value_type x);

    bool operator==(_self const& rhs);
    bool operator!=(_self const& rhs);
    bool operator<(_self const& rhs);
private:
    /** Member variables. */
    storage_type _ptr;
    offset_type _offset;
};

}} // end boost::detail


#endif
