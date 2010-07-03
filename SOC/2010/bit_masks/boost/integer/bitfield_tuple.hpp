//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_HPP
#define BOOST_BITFIELD_TUPLE_HPP
#include <boost/integer/details/bft/template_expansion_macros.hpp>
#include <boost/integer/details/bitfield_tuple_impl.hpp>
#include <boost/bitfield/bitfield.hpp>
#include <boost/integer/details/bft/name_lookup.hpp>
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/size.hpp>
#include <boost/integer/details/bft/reference_builder.hpp>
#include <boost/integer/details/bft/ext/bitfield_tuple_fusion_includes.hpp>
#include <boost/integer/details/fusion_ext_includes.hpp>
#include <boost/integer/details/bft/make_bitfield_tuple.hpp>



namespace boost {


template <  BOOST_MAKE_BFT_TEMPLATE_PARAMS() >
struct bitfield_tuple
    : protected details::bitfield_tuple_base<
        BOOST_BFT_PARAMETER_LIST()
    >
{
private:
    typedef details::bitfield_tuple_base<BOOST_BFT_PARAMETER_LIST()> _base;
    typedef bitfield_tuple<BOOST_BFT_PARAMETER_LIST()>  _self;

public:
    typedef typename _base::field_vector            members;
    typedef typename _base::storage_type            storage_type;
    typedef typename _base::offset                  bits_used;


    /** Second revision of the proxy reference type this one deal better 
     *  with const qualifiers and reduce the number of reference types from
     *  two to one.
     */
    template <typename BitfieldElement>
    struct bitfield_reference {
    private:
        typedef bitfield_reference<BitfieldElement>     _self;
    public:
        typedef typename BitfieldElement::return_type   return_type;
        typedef typename mpl::if_<
            is_const< BitfieldElement >,
            const storage_type,
            storage_type
        >::type                                         storage_t;
            
        // typedef typename Bitfield 
        typedef integer::bitfield<
            storage_t,
            BitfieldElement::offset::value,
            BitfieldElement::offset::value
              +
            BitfieldElement::field_width::value - 1,
            return_type
        >                                               field_type;

        /** Reference constructor. */
        explicit bitfield_reference(storage_t& field)
            :field_( field )
        { }


        /** copy constructor.
         *  This is because references are copy constructible.
         */
        bitfield_reference( _self const& x)
           :field_( x.field_ )
        { }
        
        /** Implicit conversion operator 
         *  Returns the value retrieved from the mask.
         */
        inline operator return_type() const {
            return field_.get();
        }

        /** Assignment Of return_type into reference.
         *  This allows values to be assigned to the get function, as part of 
         *  the tuple like interface.
         */
        _self const& operator=(return_type const& rhs) {
            field_.set( rhs );
            return *this;
        }

    private:
        field_type field_;
    };

    /** Value constructor.
     *  This sets the initial value of the internal data to x.
     *  Also functions as the default constructor.
     */
    explicit bitfield_tuple(storage_type x = 0)
        :_data(x)
    { }

    /** Copy constructor. */
    bitfield_tuple(_self const& x)
        :_data( x.data() )
    { }

    /** Assignment from an integer
     *  Allows for the user to assign a type which they wish for this type to
     *  access.
     */
    _self const& operator=( storage_type const& new_value) {
        _data = new_value;
    }

    /** Copy Assignment Operator.
     *  Provides assignment from this type to another.
     */
    _self const& operator=(_self const& x) {
        _data = x._data;
    }



    /** Internal storage accessors
     *  Returns a reference of the internally stored type. This provides the
     *  ability for the fusion sequence extension to work correctly.
     */
    //@{
    storage_type data( ) const {
         return _data;
    }
    //@}

    /** Get function interfaces.
     *  These provide access into the tuple via "reference".
     *  If an invalid index or name is provided then then the user will cause
     *  look up failure.
     */
    //@{
    template <typename Name>
    inline typename details::disable_if_reference_type_by_name<_self,Name>::type
    get() {
        typedef typename details::disable_if_reference_type_by_name<
            _self,
            Name
        >::type reference_info;
        return reference_info( _data );
    }


    template <typename Name>
    inline typename details::disable_if_reference_type_by_name<
        const _self,
        Name
    >::type
    get() const {
        typedef typename details::disable_if_reference_type_by_name<
            const _self,
            Name
        >::type         reference_info;
        return reference_info( _data );
    }

    template <std::size_t Index>
    inline typename details::enable_if_reference_type_by_index<
        _self,
        Index
    >::type
    get() {
        typedef typename details::enable_if_reference_type_by_index<
            _self,
            Index
        >::type             reference_info;
        return reference_info(_data);
    }

    template <std::size_t Index>
    inline typename details::enable_if_reference_type_by_index<
        const _self,
        Index
    >::type
    get() const {
        typedef typename details::enable_if_reference_type_by_index<
            const _self,
            Index
        >::type                  reference_info;
        return reference_info( _data );
    }
    //@}

private:
    storage_type _data;
};

} // end boost



#endif
