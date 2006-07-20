// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file container_adaptor/vector_adaptor.hpp
/// \brief Container adaptor to easily build a std::vector signature compatible container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_VECTOR_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_VECTOR_ADAPTOR_HPP

#include <boost/bimap/container_adaptor/detail/sequence_container_adaptor.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {

/// \brief Container adaptor to easily build a std::vector signature compatible container.

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
class vector_adaptor :

    public detail::sequence_container_adaptor
    <
        Base,

        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,

        ValueToBaseConverter, ValueFromBaseConverter,

        FunctorsFromDerivedClasses
    >
{

    typedef vector_adaptor this_type;

    // Access -----------------------------------------------------------------

    public:

    vector_adaptor() {}

    explicit vector_adaptor(Base & c) :
        vector_adaptor::sequence_container_adaptor_(c) {}

    protected:

    typedef vector_adaptor
    <
        Base,

        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,

        FunctorsFromDerivedClasses

    > vector_adaptor_;

    // Interface --------------------------------------------------------------

    public:

    typename this_type::size_type capacity() const
    {
        return this_type::base().capacity();
    }

    void reserve(typename this_type::size_type m)
    {
        this_type::base().resize(m);
    }

    void resize(typename this_type::size_type n,
                const typename this_type::value_type& x =
                    typename this_type::value_type())
    {
        this_type::base().resize(n,
            this_type::template functor<typename this_type::value_to_base>()(x)
        );
    }

    typename this_type::const_reference
        operator[](typename this_type::size_type n) const
    {
        return this_type::base().operator[](n);
    }

    typename this_type::const_reference
        at(typename this_type::size_type n) const
    {
        return this_type::base().at(n);
    }

    typename this_type::reference
        operator[](typename this_type::size_type n)
    {
        return this_type::base().operator[](n);
    }

    typename this_type::reference
        at(typename this_type::size_type n)
    {
        return this_type::base().at(n);
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


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_VECTOR_ADAPTOR_HPP


