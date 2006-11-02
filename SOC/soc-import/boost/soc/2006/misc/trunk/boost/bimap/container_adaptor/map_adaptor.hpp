// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file container_adaptor/map_adaptor.hpp
/// \brief Container adaptor to easily build a std::map signature compatible container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_MAP_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_MAP_ADAPTOR_HPP

#include <boost/bimap/container_adaptor/ordered_associative_container_adaptor.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/call_traits.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {

/// \brief Container adaptor to easily build a std::map signature compatible container.

template
<
    class Base,

    class Iterator,
    class ConstIterator,
    class ReverseIterator,
    class ConstReverseIterator,

    class IteratorToBaseConverter          = ::boost::mpl::na,
    class IteratorFromBaseConverter        = ::boost::mpl::na,
    class ReverseIteratorFromBaseConverter = ::boost::mpl::na,
    class ValueToBaseConverter             = ::boost::mpl::na,
    class ValueFromBaseConverter           = ::boost::mpl::na,
    class KeyToBaseConverter               = ::boost::mpl::na,

    class FunctorsFromDerivedClasses = mpl::list<>
>
class map_adaptor :

    public ::boost::bimap::container_adaptor::ordered_associative_container_adaptor
    <
        Base,
        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        typename Iterator::value_type::first_type,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,
        FunctorsFromDerivedClasses
    >
{

    typedef ::boost::bimap::container_adaptor::ordered_associative_container_adaptor
    <
        Base,
        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        typename Iterator::value_type::first_type,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,
        FunctorsFromDerivedClasses

    > base_;

    // MetaData -------------------------------------------------------------

    public:

    typedef typename Iterator::value_type::second_type data_type;

    // Access -----------------------------------------------------------------

    public:

    explicit map_adaptor(Base & c) :
        base_(c) {}

    protected:

    typedef map_adaptor map_adaptor_;

    // Interface --------------------------------------------------------------

    public:

    data_type& operator[](typename ::boost::call_traits< typename base_::key_type >::param_type k)
    {
        return this->base()[k]; 
    }
};


} // namespace container_adaptor
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_MAP_ADAPTOR_HPP

