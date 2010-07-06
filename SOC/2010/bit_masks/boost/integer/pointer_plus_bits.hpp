//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_PPB_POINTER_PLUS_BITS_HPP
#define BOOST_PPB_POINTER_PLUS_BITS_HPP

// for uint64_t 
#include <boost/cstdint.hpp>
#include <cstddef>
#include <boost/mpl/if.hpp>
#include <boost/integer/bits_mask.hpp>


namespace boost { namespace details { namespace ppb {
template <typename T>
struct get_mask_type {
    typedef typename mpl::if_c<
        bit_width<std::size_t>::value < bit_width<T*>::value,
        uint64_t,
        uint32_t
    >::type                 type;
};

}} // end detials::ppb

template <typename T, std::size_t ExtraBits = 2 >
class pointer_plus_bits {
    typedef pointer_plus_bits<T,ExtraBits> _self;
public:

    /** Typedefs about the container.
     *  value_type - is the type that is pointed at.
     *  pointer - is a pointer to value_type type.
     *  const_pointer - const pointer to the value type.
     *  reference - reference type to that you get from dereferencing the
     *      stuffed pointer.
     *  const_reference - the type you get from dereferencing a const_pointer.
     *  stuffed_bits - the number of bits stuffed into the pointer.
     */
    typedef T                   value_type;
    typedef value_type*         pointer;
    typedef value_type const*   const_pointer;
    typedef value_type&         reference;
    typedef value_type const&   const_reference;
    BOOST_STATIC_CONSTANT(std::size_t, stuffed_bits = ExtraBits);

    // this may need to change depending on the allocator.
    typedef std::ptrdiff_t      difference_type;

    // they integral type the same size as a pointer.
    // for 32 bit and 64 bit only. As of right now.
    typedef typename details::ppb::get_mask_type<
        value_type
    >::type                     mask_type;


    // mask used for getting the pointer out of _data
    typedef high_bits_mask<
        mask_type,
        bit_width<mask_type>::value - stuffed_bits
    >                           ptr_mask;
    typedef low_bits_mask<
        mask_type,
        stuffed_bits
    >                           stuffed_bits_mask;


    /** pointer_plus_bits 
     *  - constructor over a pointer to x.
     */
    explicit pointer_plus_bits(pointer x)
        :_data(x)
    { }

    /** Copy constructor. */
    pointer_plus_bits(_self const& x)
        :_data(x._data)
    { }

    /** Defualt constructor. */
    pointer_plus_bits()
        :_data()
    { }


    /** General utilities which are used for preforming basic operations
     *  on the internally stored pointer type.
     */
    //@{
    pointer get_address() const {
        return ptr_mask::value & _data;
    }

    mask_type get_stuffed_bits() const {
        return stuffed_bits_mask::value & _data;
    }

    void set_pointer(pointer x) {
        _data = (ptr_mask::value & x) | get_stuffed_bits();
    }
    //@}

    

    /** Operators. */

    /** Assignement operators.*/
    
    /** Copy Assignmenet*/
    _self const& operator=(_self const& rhs);

    /** Value Assignment. */
    _self const& operator=(pointer rhs);

    /** pointer like operators. */

    /** Dereference operator. */
    reference operator*() const { return *get_address();}
    
    /** arrow operator*/
    pointer operator->() const { return get_address(); }


    // TODO:make this not work if value_type is not divisible by a dword.
    difference_type operator-(_self rhs) {
        return get_address() - rhs.get_address();
    }

    // TODO:make this not work if value_type is not divisible by a dword.
    pointer operator+(uintmax_t rhs) {
        return get_address() + rhs;
    }

    // TODO:make this not work if value_type is not divisible by a dword.
    _self const& operator+=(intmax_t rhs) {
        _data = (get_address() + rhs) | get_stuffed_bits();
        return *this;
    }

    // TODO:make this not work if value_type is not divisible by a dword.
    _self const& operator-=(intmax_t rhs) {
        _data = (get_address() - rhs) | get_stuffed_bits();
        return *this;
    }


    // preincrement.
    _self& operator++() {
        this->set_pointer(++get_address());
        return *this;
    }

    // post increment
    // TODO:make this not work if value_type is not divisible by a dword.
    _self operator++(int) {
        _self return_value(*this);
        this->set_pointer(++get_address());
        return return_value;
    }

    // pre decrement
    _self& operator--() {
        this->set_pointer(--get_address());
        return *this;
    }

    // post decrement
    // TODO:make this not work if value_type is not divisible by a dword.
    _self operator--(int) {
        _self return_value(*this);
        this->set_pointer(--get_address());
        return return_value;
    }



    // TODO: Revisit this later or send something to the ML about how the
    // equality operator works.
    bool operator==(_self rhs) const {
        return get_address() == rhs.get_address();
    }

    bool operator!=(_self rhs) const {
        return get_address() != rhs.get_address();
    }


    // TODO: MAKE A PROXY REFERENCE TYPE FOR THE GET FUNCTION. for getting
    // single bits from the stuffed bits this will be trivial.
    template <std::size_t Index>
    struct bit_reference {
        explicit bit_reference(pointer& x);
        bit_reference(bit_reference<Index> const& x);
        operator bool() const;
        bit_reference<Index> const& operator=(bool rhs);

    private:
        bit_reference();
        pointer& _ref;
    };

private:
    pointer _data;
};
} // end boost

#endif
