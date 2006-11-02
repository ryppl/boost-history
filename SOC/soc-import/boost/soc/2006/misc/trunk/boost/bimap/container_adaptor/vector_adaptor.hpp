// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file container_adaptor/vector_adaptor.hpp
/// \brief Container adaptor to easily build a std::vector signature compatible container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_VECTOR_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_VECTOR_ADAPTOR_HPP

#include <boost/bimap/container_adaptor/sequence_container_adaptor.hpp>
#include <boost/mpl/aux_/na.hpp>

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

    class IteratorToBaseConverter          = ::boost::mpl::na,
    class IteratorFromBaseConverter        = ::boost::mpl::na,
    class ReverseIteratorFromBaseConverter = ::boost::mpl::na,
    class ValueToBaseConverter             = ::boost::mpl::na,
    class ValueFromBaseConverter           = ::boost::mpl::na,

    class FunctorsFromDerivedClasses = mpl::list<>
>
class vector_adaptor :

    public ::boost::bimap::container_adaptor::sequence_container_adaptor
    <
        Base,
        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        FunctorsFromDerivedClasses
    >
{

    typedef ::boost::bimap::container_adaptor::sequence_container_adaptor
    <
        Base,
        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        FunctorsFromDerivedClasses

    > base_;

    // Access -----------------------------------------------------------------

    public:

    vector_adaptor() {}

    explicit vector_adaptor(Base & c) :
        base_(c) {}

    protected:

    typedef vector_adaptor vector_adaptor_;

    // Interface --------------------------------------------------------------

    public:

    typename base_::size_type capacity() const
    {
        return this->base().capacity();
    }

    void reserve(typename base_::size_type m)
    {
        this->base().resize(m);
    }

    void resize(typename base_::size_type n,
                typename ::boost::call_traits< typename base_::value_type >::param_type x =
                    typename base_::value_type())
    {
        this->base().resize(n,
            this->template functor<typename base_::value_to_base>()(x)
        );
    }

    typename base_::const_reference
        operator[](typename base_::size_type n) const
    {
        return this->base().operator[](n);
    }

    typename base_::const_reference
        at(typename base_::size_type n) const
    {
        return this->base().at(n);
    }

    typename base_::reference
        operator[](typename base_::size_type n)
    {
        return this->base().operator[](n);
    }

    typename base_::reference
        at(typename base_::size_type n)
    {
        return this->base().at(n);
    }
};


} // namespace container_adaptor
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_VECTOR_ADAPTOR_HPP


