//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_HPP
#define BOOST_BITFIELD_TUPLE_HPP
#include <boost/integer/details/bitfield_tuple_impl.hpp>

namespace boost {

template <  typename T0,
            typename T1 = mpl::void_,
            typename T2 = mpl::void_,
            typename T3 = mpl::void_,
            typename T4 = mpl::void_,
            typename T5 = mpl::void_,
            typename T6 = mpl::void_,
            typename T7 = mpl::void_,
            typename T8 = mpl::void_,
            typename T9 = mpl::void_
>
struct bitfield_tuple
    : protected details::bitfield_tuple_base<
        T0,T1,T2,T3,T4,T5,T6,T7,T8,T9
    >,
    details::bitfield_tuple_base<
        T0,T1,T2,T3,T4,T5,T6,T7,T8,T9
    >::allocation_base_policy
{
private:
    typedef details::bitfield_tuple_base<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9> _base;
    typedef typename _base::allocation_base_policy  _alloc_base;
    typedef bitfield_tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>  _self;

public:
    typedef typename _base::is_stack_allocated      is_stack_allocated;
    typedef typename _base::is_allocator_allocated  is_allocator_allocated;
    typedef typename _base::processed_args          processed_args;
    typedef typename _base::field_vector            members;
    typedef typename _base::storage_type            storage_type;
    typedef typename _base::offset                  bits_used;

    
    /** Proxy type returned by get functions.
     *  This serves as the go between things within this class.
     */
    template <typename MaskInfo>
    struct bit_ref {

        /** typedefs 
         *  return_type - is the type which is retrieved from
         *      within storage is turned into and returned as.
         *
         *  mask - contains all information needed to iteract with data in the
         *      the stroage.
         */
        typedef typename MaskInfo::return_type                  return_type;
        typedef bits_mask<
            storage_type,
            MaskInfo::offset::value,
            MaskInfo::field_width::value
        >                                                       mask;

        typedef bit_ref<MaskInfo>                               _self;

        /** Reference constructor.
         *  Because the bit_ref is an abstraction of a reference then it also
         *  must behave like a reference type.
         */
        bit_ref(storage_type& ref)
            :_ref(ref)
        { }
        
        operator return_type() const {
            return (_ref & mask()) >> mask::offset;
        }


        _self const& operator=(return_type const& rhs) {
            _ref ^= (_ref & mask());
            _ref |= ((static_cast<storage_type>(rhs) << mask::offset) & mask());
            return *this;
        }
        
    private:
        storage_type& _ref;
        bit_ref(); // not default constructible.
    };


    /** Value constructor.
     *  This sets the initial value of the internal data to x.
     *  Also functions as the default constructor.
     */
    bitfield_tuple(storage_type x = 0)
        :_alloc_base(x)
    { }

    /** Copy constructor. */
    bitfield_tuple(_self const& x)
        :_alloc_base( x.data() )
    { }

    /** "Identical members" copy constructor.
     *  Basically this is used to get the data within a structure whose 
     *  bitfields are the same (in the same order) but the storage is 
     *  specified in a different place within the template arguments.
     *
     *  XXX 
     *      TODO: The signature of this will need to change possibly to
     *      use enable_if or some form of internal dispatching.
     *  XXX
     */
    template <typename T>
    bitfield_tuple(T const& x);

    // TODO: look into the creation of a member wise constructor.
    
    _self const& operator=(_self const& x) {
        this->_data = x._data;
    }

    /**
     *  Retuns a copy of the internally stored type.
     */
    //@{
    storage_type const data( ) const {
         return this->get_data();
    }

    storage_type data( ) {
         return this->get_data();
    }
    //@}

    /** Returns a string with the bits from the internal storage within it. */
    //@{
    std::string to_string() const;
    //@}


    /** Get function interfaces.
     *  These provide access into the tuple.
     */
    //@{
    
    //@}
};



} // end boost

#endif
