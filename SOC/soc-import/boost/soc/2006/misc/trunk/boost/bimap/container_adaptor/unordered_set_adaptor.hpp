// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file container_adaptor/unordered_set_adaptor.hpp
/// \brief Container adaptor to easily build a std::unordered_set signature compatible container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_UNORDERED_SET_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_UNORDERED_SET_ADAPTOR_HPP

#include <boost/bimap/container_adaptor/detail/unordered_associative_container_adaptor.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {

/// \brief Container adaptor to easily build a std::unordered_set signature compatible container.

template
<
    class Base,

    class Iterator,
    class ConstIterator,
    class LocalIterator,
    class ConstLocalIterator,

    class IteratorToBaseConverter        = use_default,
    class IteratorFromBaseConverter      = use_default,
    class LocalIteratorFromBaseConverter = use_default,
    class ValueToBaseConverter           = use_default,
    class ValueFromBaseConverter         = use_default,
    class KeyToBaseConverter             = use_default
>
class unordered_set_adaptor :

    public detail::unordered_associative_container_adaptor
    <
        Base,

        Iterator, ConstIterator, LocalIterator, ConstLocalIterator,

        typename Iterator::value_type,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        LocalIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter
    >
{

    // Access -----------------------------------------------------------------

    public:

    unordered_set_adaptor() {}

    explicit unordered_set_adaptor(Base & c) :
        unordered_set_adaptor::unordered_associative_container_adaptor_(c) {}

    protected:

    typedef unordered_set_adaptor
    <
        Base,

        Iterator, ConstIterator, LocalIterator, ConstLocalIterator,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        LocalIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter

    > unordered_set_adaptor_;

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


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_UNORDERED_SET_ADAPTOR_HPP

