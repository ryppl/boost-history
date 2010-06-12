//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_HPP
#define BOOST_BITFIELD_TUPLE_HPP
#include <boost/integer/details/bitfield_tuple_impl.hpp>
#include <boost/bitfield/bitfield.hpp>
#include <boost/integer/details/bft/name_lookup.hpp>
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/type_traits.hpp>

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
        typedef bit_ref<MaskInfo>                               _self;
        typedef typename make_unsigned<return_type>::type  unsigned_return_type;
        typedef typename make_unsigned<
            storage_type
        >::type                                           unsigned_storage_type;

        /** Internals bitfield type for extracting individual fields from 
         *  within the storage_type.
         */
        typedef typename integer::bitfield<
            unsigned_storage_type,
            MaskInfo::offset::value,
            MaskInfo::offset::value + MaskInfo::field_width::value - 1
        >                                                       bitfield_type;


        /** Reference constructor.
         *  Because the bit_ref is an abstraction of a reference then it also
         *  must behave like a reference type.
         */
        bit_ref(storage_type& ref)
            :_ref( *reinterpret_cast<unsigned_storage_type*>(&ref) )
        { }

        /** copy constructor.
         *  This is because references are copy construtible.
         */
        // bit_ref( bit_ref<MaskInfo> const& x)
        //    :_ref( x )
        // { }
        
        /** Implicit conversion operator 
         *  this allows for implicit conversion to the return_type.
         */
        operator return_type() const {
            return static_cast< return_type >( _ref.get() );
        }

        /** Assignment Of return_type into reference.
         *  This allows values to be assigned to the get function, as part of 
         *  the tuple like interface.
         */
        _self const& operator=(return_type const& rhs) {
            _ref.set(
                static_cast<typename make_unsigned<storage_type>::type>(rhs));
            return *this;
        }
        
    private:
        // storage reference.
        bitfield_type _ref;

        // not default constructible because this is a reference type
        bit_ref();
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
     *  These provide access into the tuple via "reference".
     *  If an invalid index or name is provided then then the user will cause
     *  look up failure.
     */
    //@{
    template <typename Name>
    inline typename disable_if<
        is_same <
            typename mpl::find_if<
                members,
                details::match_name<
                    mpl::_1,
                    Name
                >
            >::type,
            typename mpl::end<
                members
            >::type
        >,
        bit_ref<
            typename mpl::deref<
                typename mpl::find_if<
                    members,
                    details::match_name<
                        mpl::_1,
                        Name
                    >
                >::type
            >::type
        >
    >::type
    get() {
         typedef bit_ref< 
            typename mpl::deref<
                typename mpl::find_if<
                    members,
                     details::match_name<
                        mpl::_1,
                        Name
                    >
                >::type
            >::type 
        >                                   reference_info;
        return reference_info( this->get_data() );
    }


    template <typename Name>
    inline typename disable_if<
        is_same <
            typename mpl::find_if<
                members,
                details::match_name<
                    mpl::_1,
                    Name
                >
            >::type,
            typename mpl::end<
                members
            >::type
        >,
        bit_ref<
            typename mpl::deref<
                typename mpl::find_if<
                    members,
                    details::match_name<
                        mpl::_1,
                        Name
                    >
                >::type
            >::type
        > const
    >::type 
    get() const {
         typedef bit_ref< 
            typename mpl::deref<
                typename mpl::find_if<
                    members,
                     details::match_name<
                        mpl::_1,
                        Name
                    >
                >::type
            >::type 
        > const                             reference_info;
        return reference_info( this->get_data() );
    }

    //@}
};



} // end boost

#endif
