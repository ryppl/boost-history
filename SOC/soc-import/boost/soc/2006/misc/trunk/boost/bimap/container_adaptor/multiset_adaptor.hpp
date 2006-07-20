// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file container_adaptor/multiset_adaptor.hpp
/// \brief Container adaptor to easily build a std::multiset signature compatible container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_MULTISET_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_MULTISET_ADAPTOR_HPP

#include <boost/bimap/container_adaptor/detail/ordered_associative_container_adaptor.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {

/// \brief Container adaptor to easily build a std::multiset signature compatible container.

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
class multiset_adaptor :

    public detail::ordered_associative_container_adaptor
    <
        Base,

        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,

        typename Iterator::value_type,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,

        FunctorsFromDerivedClasses
    >
{

    // Access -----------------------------------------------------------------

    public:

    multiset_adaptor() {}

    explicit multiset_adaptor(Base & c) :
        multiset_adaptor::ordered_associative_container_adaptor_(c) {}

    protected:

    typedef multiset_adaptor
    <
        Base,

        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,

        FunctorsFromDerivedClasses

    > multiset_adaptor_;

};


} // namespace container_adaptor
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_MULTISET_ADAPTOR_HPP

