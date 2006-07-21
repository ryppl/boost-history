// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file container_adaptor/detail/associative_container_adaptor.hpp
/// \brief Container adaptor to build a type that is compliant to the concept of an associative container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP

#include <utility>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/bimap/container_adaptor/detail/identity_converters.hpp>
#include <boost/bimap/container_adaptor/use_default.hpp>
#include <boost/bimap/container_adaptor/detail/weak_associative_container_adaptor.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {
namespace detail {

/// \brief Container adaptor to build a type that is compliant to the concept of an associative container.

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
class associative_container_adaptor :

    public weak_associative_container_adaptor
    <
        Base,

        Iterator, ConstIterator,

        KeyType,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ValueToBaseConverter   , ValueFromBaseConverter,

        KeyToBaseConverter,

        FunctorsFromDerivedClasses
    >
{

    typedef associative_container_adaptor this_type;

    // ACCESS -----------------------------------------------------------------

    public:

    explicit associative_container_adaptor(Base & c)
        : associative_container_adaptor::weak_associative_container_adaptor_(c) {}

    protected:


    typedef associative_container_adaptor
    <
        Base,

        Iterator, ConstIterator,

        KeyType,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ValueToBaseConverter   , ValueFromBaseConverter,

        KeyToBaseConverter,

        FunctorsFromDerivedClasses

    > associative_container_adaptor_;

    // Interface --------------------------------------------------------------

    public:

    typename this_type::size_type
        erase(const typename this_type::key_type& k)
    {
        return associative_container_adaptor::base().erase
        (
            this_type::template functor<typename this_type::key_to_base>()(k)
        );
    }

    // As we redefine erase, the other overloads need to be manually routed

    typename associative_container_adaptor::iterator erase(
        typename associative_container_adaptor::iterator pos)
    {
        return associative_container_adaptor::container_adaptor_::erase(pos);
    }

    typename associative_container_adaptor::iterator erase(
        typename associative_container_adaptor::iterator first,
        typename associative_container_adaptor::iterator last)
    {
        return associative_container_adaptor::container_adaptor_::erase(first,last);
    }

    typename this_type::size_type
        count(const typename this_type::key_type& k)
    {
        return this_type::base().count(
            this_type::template functor<typename this_type::key_to_base>()(k)
        );
    }

    typename this_type::iterator
        find(const typename this_type::key_type& k)
    {
        return this_type::template functor<
            typename this_type::iterator_from_base>()
        (
            this_type::base().find(
                this_type::template functor<typename this_type::key_to_base>()(k)
            )
        );
    }

    typename this_type::const_iterator
        find(const typename this_type::key_type& k) const
    {
        return this_type::template functor<
            typename this_type::iterator_from_base>()
        (
            this_type::base().find(
                this_type::template functor<typename this_type::key_to_base>()(k)
            )
        );
    }

    std::pair
    <
        typename this_type::iterator,
        typename this_type::iterator
    >
        equal_range(const typename this_type::key_type& k)
    {
        std::pair<

            typename Base::iterator,
            typename Base::iterator

        > r( this_type::base().equal_range(
                this_type::template functor<typename this_type::key_to_base>()(k)
            )
        );

        return std::pair
        <
            typename this_type::iterator,
            typename this_type::iterator
        >(
            this_type::template functor<
                typename this_type::iterator_from_base
            >()                                         ( r.first ),
            this_type::template functor<
                typename this_type::iterator_from_base
            >()                                         ( r.second )
        );
    }

    std::pair
    <
        typename this_type::const_iterator,
        typename this_type::const_iterator
    >
        equal_range(const typename this_type::key_type& k) const
    {
        std::pair<

            typename Base::const_iterator,
            typename Base::const_iterator

        > r( this_type::base().equal_range(
                this_type::template functor<typename this_type::key_to_base>()(k)
            )
        );

        return std::pair
        <
            typename this_type::const_iterator,
            typename this_type::const_iterator
        >(
            this_type::template functor<
                typename this_type::iterator_from_base
            >()                                         ( r.first ),
            this_type::template functor<
                typename this_type::iterator_from_base
            >()                                         ( r.second )
        );
    }

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


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP



