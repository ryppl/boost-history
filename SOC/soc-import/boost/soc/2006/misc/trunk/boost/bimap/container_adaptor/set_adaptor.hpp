// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file container_adaptor/set_adaptor.hpp
/// \brief Container adaptor to easily build a std::set_adaptor signature compatible container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_SET_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_SET_ADAPTOR_HPP

#include <boost/bimap/container_adaptor/detail/ordered_associative_container_adaptor.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {

/// \brief Container adaptor to easily build a std::set_adaptor signature compatible container.

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
    class KeyToBaseConverter               = use_default
>
class set_adaptor :

    public detail::ordered_associative_container_adaptor
    <
        Base,

        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,

        typename Iterator::value_type,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter
    >
{

    // Access -----------------------------------------------------------------

    public:

    set_adaptor() {}

    explicit set_adaptor(Base & c) :
        set_adaptor::ordered_associative_container_adaptor_(c) {}

    protected:

    typedef set_adaptor
    <
        Base,

        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter

    > set_adaptor_;

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


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_SET_ADAPTOR_HPP


