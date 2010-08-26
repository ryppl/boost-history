//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_BITFIELD_ITERATOR_BASE_HPP
#define BOOST_BITFIELD_ITERATOR_BASE_HPP
#include "bitfield_vector_member_impl.hpp"
#include <cstddef>
#include <iterator>
#include <boost/assert.hpp>
#include <boost/type_traits/make_signed.hpp>
#include <iostream>
#include <boost/cstdint.hpp>
#include <climits>

namespace boost { namespace detail {

namespace safe_bool_impl {

class safe_bool_base {
protected:
    typedef void (safe_bool_base::* bool_type )() const;
    void this_type_does_not_support_comparisons() const {
        BOOST_ASSERT((false));
    }

    safe_bool_base() { }
    safe_bool_base(const safe_bool_base&) { }
    safe_bool_base& operator=(const safe_bool_base&) { return *this; }
    ~safe_bool_base() {}
};

template <typename T>
struct safe_bool
    :safe_bool_base
{
operator bool_type() const {
    return (static_cast<const T*>(this))->has_value()
        ? &safe_bool_base::this_type_does_not_support_comparisons : 0;
}
protected:
    ~safe_bool() {}
};

template <typename T, typename U> 
void operator==(safe_bool<T> const & lhs,safe_bool<U> const& rhs) {
    lhs.this_type_does_not_support_comparisons();
    return false;
}

template <typename T,typename U> 
void operator!=(safe_bool<T> const& lhs, safe_bool<U> const& rhs) {
    lhs.this_type_does_not_support_comparisons();
    return false;	
}
} // end safe bool impl

/** bitfield_vector_iterator_base
 *  This class is used to abstract all of the baisc operations which are 
 *  preformed on an iterator from a bitfield tuple. This iterator may differ
 *  from what its expected from a normal iterator however that is still TBD.
 *  
 *      Things to consider as "optimizations"/"corrections" to the uasual
 *  vector iterator design. 
 *  
 *  1) Should these iterators be non-invalidaing iterators?
 *      If I cache the index and a pointer to the _impl member of the
 *      vector base class then ever thing works great. If I instead 
 *      keep a copy of the reference type I believe everthing actually get's
 *      larger.
 *  2) cacheing the index.
 *  
 *  
 *  
 *  
 */

template <typename T, std::size_t Width>
struct bitfield_vector_iterator_base
    :safe_bool_impl::safe_bool< bitfield_vector_iterator_base<T,Width> >
{

    /** Typedef's for iterator base class. */
    //@{
    
    typedef bitfield_vector_iterator_base<T,Width>  _self;
    typedef safe_bool_impl::safe_bool< _self >      _base;
    typedef proxy_reference_type<T,Width>           proxy_ref_type;
    typedef const_proxy_reference_type<T,Width>     const_proxy_ref_type;
    typedef typename _base::bool_type               bool_type;

    // I don't believe that this iterator can be a random access iterator
    // until C++0x
    typedef std::bidirectional_iterator_tag         iterator_category;
    typedef T                                       value_type;
    typedef T*                                      pointer;
    typedef std::ptrdiff_t                          difference_type;
    BOOST_STATIC_CONSTANT( std::size_t, width = Width );
    //@}

    /** bitfield_vector_iterator_base
     *  constructors and destructor
     */
    //@{
    // default
    bitfield_vector_iterator_base()
        :_ptr(0),
        _bit_offset(0)
    { }

    // copy
    bitfield_vector_iterator_base( _self const& rhs )
        :_ptr(rhs._ptr),
        _bit_offset(rhs._bit_offset)
    { }

    // over a reference type
    explicit bitfield_vector_iterator_base(proxy_ref_type const& ref)
        : _ptr( ref._ptr),
        _bit_offset(ref._mask._offset)
    { }

    bitfield_vector_iterator_base(storage_ptr_t ptr, std::size_t bit_offset)
        :_ptr(ptr),
        _bit_offset(bit_offset)
    { }
    //@}

    /** iterator operations.
     *  This are the functions which will be used to implement the actual 
     *  iterator functionality.
     */
    //@{
    void advance(intmax_t rhs) {
        typedef typename make_signed<std::size_t>::type signed_size_t;

        signed_size_t previous_offset = signed_size_t(_bit_offset);
        previous_offset += (signed_size_t(Width)*rhs);
        _ptr -= std::size_t((previous_offset%8)<0)+((previous_offset / 8)*-1);

        // Comment for the following line of code.
        // In the case that previous_offset%8 is actually less then 8
        // will construct a size_t with 1 and left shift it 3 making it
        // positive 8. This is done to avoid branching inside of 
        // increment and decrement operations and this only works if 
        // the value is true otherwise the value is zero which is 
        // the behavior I want.
        _bit_offset = (std::size_t((previous_offset%8)<0)<<3) + (previous_offset % 8);
    }
    
    void next() {
        advance(1);
    }

    void previous() {
        advance(-1);
    }

    std::ptrdiff_t distance(_self const& rhs) const {
        std::ptrdiff_t ret = 8 * (_ptr - rhs._ptr);
        ret -= rhs._bit_offset;
        ret += _bit_offset;
        return ret / Width;
    }

    bool is_equal(_self const& rhs) const {
        return _ptr == rhs._ptr && _bit_offset == rhs._bit_offset;
    }

    _self& assign(_self const& rhs) {
        _ptr = rhs._ptr;
        _bit_offset = rhs._bit_offset;
        return *this;
    }

    const_proxy_ref_type const_deref() const {
        return const_proxy_ref_type(_ptr, _bit_offset);
    }

    proxy_ref_type deref() const {
        return proxy_ref_type(_ptr, _bit_offset);
    }

    bool has_value() const {
        return _ptr;
    }

    bool is_less(_self const& rhs) const {
        if(_ptr <= rhs._ptr) {
            if( _bit_offset < rhs._bit_offset) {
                return true;
            }else{
                return false;
            }
        }else{
            return true;
        }
    }

    bool is_greater(_self const& rhs) const {
        if(_ptr >= rhs._ptr) {
            if(_bit_offset > rhs._bit_offset) {
                return true;
            }else{
                return false;
            }
        }else{
            return true;
        }
    }
    //@)
    


    storage_ptr_t   _ptr;
    std::size_t     _bit_offset;
};


}} // end boost::detail


#endif
