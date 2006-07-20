// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file container_adaptor/detail/ordered_associative_container_adaptor.hpp
/// \brief Container adaptor to build a type that is compliant to the concept of an ordered associative container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_ORDERED_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_ORDERED_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP

#include <boost/bimap/container_adaptor/detail/associative_container_adaptor.hpp>
#include <boost/bimap/container_adaptor/detail/value_comparison_adaptor.hpp>
#include <boost/bimap/container_adaptor/detail/comparison_adaptor.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/push_front.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {
namespace detail {

/// \brief Container adaptor to build a type that is compliant to the concept of an ordered associative container.

template
<
    class Base,

    class Iterator,
    class ConstIterator,
    class ReverseIterator,
    class ConstReverseIterator,

    class KeyType,

    class IteratorToBaseConverter          = use_default,
    class IteratorFromBaseConverter        = use_default,
    class ReverseIteratorFromBaseConverter = use_default,
    class ValueToBaseConverter             = use_default,
    class ValueFromBaseConverter           = use_default,
    class KeyToBaseConverter               = use_default,

    class FunctorsFromDerivedClasses = mpl::list<>
>
class ordered_associative_container_adaptor :

    public associative_container_adaptor
    <
        Base,

        Iterator, ConstIterator,

        KeyType,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ValueToBaseConverter   , ValueFromBaseConverter,

        KeyToBaseConverter,

        typename mpl::push_front<

            FunctorsFromDerivedClasses,

            typename mpl::if_< is_same< ReverseIteratorFromBaseConverter, use_default >,
            // {
                    detail::iterator_from_base_identity
                    <
                        typename Base::reverse_iterator                , ReverseIterator,
                        typename Base::const_reverse_iterator          , ConstReverseIterator
                    >,
            // }
            // else
            // {
                    ReverseIteratorFromBaseConverter
            // }

            >::type

        >::type
    >
{
    // MetaData -------------------------------------------------------------

    typedef ordered_associative_container_adaptor this_type;

    public:

    typedef comparison_adaptor
    <
        typename Base::key_compare,
        typename this_type::key_type,
        typename this_type::key_to_base

    > key_compare;

    typedef value_comparison_adaptor
    <
        typename Base::value_compare,
        typename this_type::value_type

    > value_compare;

    typedef ReverseIterator      reverse_iterator;
    typedef ConstReverseIterator const_reverse_iterator;

    protected:

    typedef typename mpl::if_< is_same< ReverseIteratorFromBaseConverter, use_default >,
        // {
                detail::iterator_from_base_identity
                <
                    typename Base::reverse_iterator                , reverse_iterator,
                    typename Base::const_reverse_iterator          , const_reverse_iterator
                >,
        // }
        // else
        // {
                ReverseIteratorFromBaseConverter
        // }

        >::type reverse_iterator_from_base;

    // Access -----------------------------------------------------------------

    public:

    explicit ordered_associative_container_adaptor(Base & c)
        : this_type::associative_container_adaptor_(c) {}

    protected:

    typedef ordered_associative_container_adaptor
    <
        Base,

        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,

        KeyType,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,

        KeyToBaseConverter,

        FunctorsFromDerivedClasses

    > ordered_associative_container_adaptor_;

    // Interface --------------------------------------------------------------

    public:

    reverse_iterator rbegin()
    {
        return this_type::template functor<
            reverse_iterator_from_base
        >()                            ( this_type::base().rbegin() );

    }

    reverse_iterator rend()
    {
        return this_type::template functor<
            reverse_iterator_from_base
        >()                            ( this_type::base().rend() );
    }

    const_reverse_iterator rbegin() const
    {
        return this_type::template functor<
            reverse_iterator_from_base
        >()                            ( this_type::base().rbegin() );
    }

    const_reverse_iterator rend() const
    {
        return this_type::template functor<
            reverse_iterator_from_base
        >()                            ( this_type::base().rend() );
    }

    key_compare key_comp() const
    {
        return key_compare(
            this_type::base().key_comp(),
            this_type::template functor<typename this_type::key_to_base>()
        );
    }

    value_compare value_comp() const
    {
        return value_compare( this_type::base().value_comp() );
    }

    typename this_type::iterator lower_bound(const typename this_type::key_type& k)
    {
       return this_type::template functor<
            typename this_type::iterator_from_base>()(
                this_type::base().lower_bound(
                    this_type::template functor<typename this_type::key_to_base>()(k)
                )
            );
    }

    typename this_type::const_iterator lower_bound(const typename this_type::key_type& k) const
    {
       return this_type::template functor<
            typename this_type::iterator_from_base>()(
                this_type::base().lower_bound(
                    this_type::template functor<typename this_type::key_to_base>()(k)
                )
            );
    }

    typename this_type::iterator upper_bound(const typename this_type::key_type& k)
    {
       return this_type::template functor<
            typename this_type::iterator_from_base>()(
                this_type::base().upper_bound(
                    this_type::template functor<typename this_type::key_to_base>()(k)
                )
            );
    }

    typename this_type::const_iterator upper_bound(const typename this_type::key_type& k) const
    {
        return this_type::template functor<
            typename this_type::iterator_from_base>()(
                this_type::base().upper_bound(
                    this_type::template functor<typename this_type::key_to_base>()(k)
                )
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


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_ORDERED_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP
