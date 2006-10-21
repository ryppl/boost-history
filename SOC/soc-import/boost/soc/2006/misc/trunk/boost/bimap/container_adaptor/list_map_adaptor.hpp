// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file container_adaptor/list_map_adaptor.hpp
/// \brief Container adaptor.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_LIST_MAP_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_LIST_MAP_ADAPTOR_HPP

#include <boost/mpl/list.hpp>
#include <boost/mpl/push_front.hpp>

#include <boost/bimap/container_adaptor/list_adaptor.hpp>
#include <boost/bimap/container_adaptor/detail/identity_converters.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {

/// \brief Container adaptor.

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

    class FunctorsFromDerivedClasses = mpl::list<>
>
class list_map_adaptor :

    public list_adaptor
    <
        Base, Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        FunctorsFromDerivedClasses
    >
{
    typedef list_adaptor
    <
        Base, Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        FunctorsFromDerivedClasses

    > base_;

    // MetaData -------------------------------------------------------------

    public:

    typedef typename Iterator::value_type::first_type  key_type;
    typedef typename Iterator::value_type::second_type data_type;


    // Access -----------------------------------------------------------------

    public:

    explicit list_map_adaptor(Base & c) :
        base_(c) {}

    protected:

    typedef list_map_adaptor
    <
        Base,

        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,

        FunctorsFromDerivedClasses

    > list_map_adaptor_;

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


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_LIST_MAP_ADAPTOR_HPP

