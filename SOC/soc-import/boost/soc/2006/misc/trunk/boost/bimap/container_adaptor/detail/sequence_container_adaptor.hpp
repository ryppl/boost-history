// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file container_adaptor/detail/weak_associative_container_adaptor.hpp
/// \brief Container adaptor to build a type that is compliant to the concept of a weak associative container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_WEAK_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_WEAK_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP

#include <utility>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/bimap/container_adaptor/detail/identity_converters.hpp>
#include <boost/bimap/container_adaptor/detail/container_adaptor.hpp>
#include <boost/bimap/container_adaptor/use_default.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {
namespace detail {

/// \brief Container adaptor to build a type that is compliant to the concept of a weak associative container.

template
<
    class Base,

    class Iterator,
    class ConstIterator,

    class KeyType,

    class IteratorToBaseConverter   = use_default,
    class IteratorFromBaseConverter = use_default,
    class ValueToBaseConverter      = use_default,
    class ValueFromBaseConverter    = use_default,

    class KeyToBaseConverter        = use_default,

    class FunctorsFromDerivedClasses = mpl::list<>
>
class weak_associative_container_adaptor :

    public container_adaptor
    <
        Base,

        Iterator, ConstIterator,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ValueToBaseConverter   , ValueFromBaseConverter,

        typename mpl::copy<

            mpl::list
            <
                typename mpl::if_< is_same< KeyToBaseConverter, use_default >,
                // {
                        key_to_base_identity
                        <
                            typename Base::key_type, KeyType
                        >,
                // }
                // else
                // {
                        KeyToBaseConverter
                // }

                >::type
            >,
            mpl::front_inserter< FunctorsFromDerivedClasses >

        >::type
    >
{
    // MetaData -------------------------------------------------------------

    public:

    typedef KeyType key_type;

    protected:

    typedef typename mpl::if_< is_same< KeyToBaseConverter, use_default >,
    // {
            key_to_base_identity
            <
                typename Base::key_type, KeyType
            >,
    // }
    // else
    // {
            KeyToBaseConverter
    // }

    >::type key_to_base;


    // ACCESS -----------------------------------------------------------------

    public:

    explicit weak_associative_container_adaptor(Base & c)
        : weak_associative_container_adaptor::container_adaptor_(c) {}

    protected:


    typedef weak_associative_container_adaptor
    <
        Base,

        Iterator, ConstIterator,

        KeyType,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ValueToBaseConverter   , ValueFromBaseConverter,

        KeyToBaseConverter,

        FunctorsFromDerivedClasses

    > weak_associative_container_adaptor_;

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

} // namespace detail
} // namespace container_adaptor
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_WEAK_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP
