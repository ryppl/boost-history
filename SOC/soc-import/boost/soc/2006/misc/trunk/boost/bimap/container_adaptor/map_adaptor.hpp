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

#include <boost/bimap/container_adaptor/detail/ordered_associative_container_adaptor.hpp>

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

    class IteratorToBaseConverter          = use_default,
    class IteratorFromBaseConverter        = use_default,
    class ReverseIteratorFromBaseConverter = use_default,
    class ValueToBaseConverter             = use_default,
    class ValueFromBaseConverter           = use_default,
    class KeyToBaseConverter               = use_default,

    class FunctorsFromDerivedClasses = mpl::list<>
>
class map_adaptor :

    public ::boost::bimap::container_adaptor::detail::ordered_associative_container_adaptor
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

    typedef ::boost::bimap::container_adaptor::detail::ordered_associative_container_adaptor
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

    typedef map_adaptor/*
    <
        Base,

        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,

        FunctorsFromDerivedClasses

    > */map_adaptor_;

    // Interface --------------------------------------------------------------

    public:

    data_type& operator[](const typename base_::key_type& k)  { return this->base()[k]; }

};



/* TODO
// Tests two maps for equality.
template<class BimapType, class Tag>
bool operator==(const map_view<BimapType,Tag>&, const map_view<BimapType,Tag>&)
{
}

// Lexicographical comparison.
template<class BimapType, class Tag>
bool operator<(const map_view<BimapType,Tag>&, const map_view<BimapType,Tag>&)
{
}
*/


} // namespace container_adaptor
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_MAP_ADAPTOR_HPP

