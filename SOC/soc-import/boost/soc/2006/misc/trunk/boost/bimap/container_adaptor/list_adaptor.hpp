// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file container_adaptor/list_adaptor.hpp
/// \brief Container adaptor to easily build a std::list signature compatible container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_LIST_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_LIST_ADAPTOR_HPP

#include <boost/bimap/container_adaptor/detail/sequence_container_adaptor.hpp>
#include <boost/bimap/container_adaptor/detail/comparison_adaptor.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {

/// \brief Container adaptor to easily build a std::list signature compatible container.

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
class list_adaptor :

    public ::boost::bimap::container_adaptor::detail::sequence_container_adaptor
    <
        Base, Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        FunctorsFromDerivedClasses
    >
{
    typedef ::boost::bimap::container_adaptor::detail::sequence_container_adaptor
    <
        Base, Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        FunctorsFromDerivedClasses

    > base_;

    // Access -----------------------------------------------------------------

    public:

    explicit list_adaptor(Base & c) :
        base_(c) {}

    protected:

    typedef list_adaptor
    <
        Base,

        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,

        FunctorsFromDerivedClasses

    > list_adaptor_;

    // Interface -------------------------------------------------------------

    public:

    // TODO This require some work... later...
    /*

    void splice(iterator position,index class name& x)
    {

    }

    void splice(iterator position,index class name& x,iterator i)
    {

    }

    void splice(
        iterator position,index class name& x,iterator first,iterator last)
    {

    }

    */

    void remove(const typename base_::value_type& value)
    {
        this->base().remove(
            this->template functor<typename base_::value_to_base>()(value)
        );
    }

    template<typename Predicate>
    void remove_if(Predicate pred)
    {
        this->base().remove_if(
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                Predicate,
                typename Base::value_type,
                typename base_::value_to_base

            >( pred, this->template functor<typename base_::value_to_base>() )
        );
    }

    void unique()
    {
        this->base().unique();
    }

    template <class BinaryPredicate>
    void unique(BinaryPredicate binary_pred)
    {
        this->base().unique(
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                BinaryPredicate,
                typename Base::value_type,
                typename base_::value_to_base

            >( binary_pred, this->template functor<typename base_::value_to_base>() )
        );
    }

    // TODO This require some work... later...
    /*

    void merge(index class name& x)
    {

    }

    template <typename Compare>
    void merge(index class name& x,Compare comp)
    {

    }

    */

    void sort()
    {
        this->base().sort();
    }

    template <typename Compare>
    void sort(Compare comp)
    {
        this->base().sort(
            ::boost::bimap::container_adaptor::detail::comparison_adaptor
            <
                Compare,
                typename Base::value_type,
                typename base_::value_to_base

            >( comp, this->template functor<typename base_::value_to_base>() )
        );
    }

    void reverse()
    {
        this->base().reverse();
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




} // namespace container_adaptor
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_SET_ADAPTOR_HPP


