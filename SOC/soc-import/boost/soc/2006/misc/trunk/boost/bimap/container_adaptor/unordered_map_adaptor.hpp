// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file container_adaptor/unordered_map_adaptor.hpp
/// \brief Container adaptor to easily build a std::unordered_map signature compatible container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_UNORDERED_MAP_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_UNORDERED_MAP_ADAPTOR_HPP

#include <boost/bimap/container_adaptor/detail/unordered_associative_container_adaptor.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {

/// \brief Container adaptor to easily build a std::unordered_map signature compatible container.

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
    class KeyToBaseConverter             = use_default,

    class FunctorsFromDerivedClasses = mpl::list<>
>
class unordered_map_adaptor :

    public ::boost::bimap::container_adaptor::detail::unordered_associative_container_adaptor
    <
        Base,
        Iterator, ConstIterator, LocalIterator, ConstLocalIterator,
        typename Iterator::value_type::first_type,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        LocalIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,
        FunctorsFromDerivedClasses
    >
{

    typedef ::boost::bimap::container_adaptor::detail::unordered_associative_container_adaptor
    <
        Base,
        Iterator, ConstIterator, LocalIterator, ConstLocalIterator,
        typename Iterator::value_type::first_type,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        LocalIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,
        FunctorsFromDerivedClasses

    > base_;

    // MetaData -------------------------------------------------------------

    public:

    typedef typename Iterator::value_type::second_type data_type;

    // Access -----------------------------------------------------------------

    public:

    explicit unordered_map_adaptor(Base & c) :
        base_(c) {}

    protected:

    typedef unordered_map_adaptor
    <
        Base,

        Iterator, ConstIterator, LocalIterator, ConstLocalIterator,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        LocalIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,

        FunctorsFromDerivedClasses

    > unordered_map_adaptor_;

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


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_UNORDERED_MAP_ADAPTOR_HPP

