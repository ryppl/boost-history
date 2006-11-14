// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file container_adaptor/multimap_adaptor.hpp
/// \brief Container adaptor to easily build a std::multimap signature compatible container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_MULTIMAP_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_MULTIMAP_ADAPTOR_HPP

#include <boost/bimap/container_adaptor/ordered_associative_container_adaptor.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/mpl/vector.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {

/// \brief Container adaptor to easily build a std::multimap signature compatible container.

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
    class KeyToBaseConverter               = ::boost::mpl::na,

    class FunctorsFromDerivedClasses = mpl::vector<>
>
class multimap_adaptor :

    public ::boost::bimap::container_adaptor::ordered_associative_container_adaptor
    <
        Base,
        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        typename Iterator::value_type::first_type,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,
        FunctorsFromDerivedClasses
    >
{
    typedef ::boost::bimap::container_adaptor::ordered_associative_container_adaptor
    <
        Base,
        Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
        typename Iterator::value_type::first_type,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,
        FunctorsFromDerivedClasses

    > base_;

    // MetaData -------------------------------------------------------------

    public:

    typedef typename Iterator::value_type::second_type data_type;

    // Access -----------------------------------------------------------------

    public:

    explicit multimap_adaptor(Base & c) :
        base_(c) {}

    protected:

    typedef multimap_adaptor multimap_adaptor_;

    // Change the insert functions

    template <class InputIterator>
    void insert(InputIterator iterBegin, InputIterator iterEnd)
    {
        for( ; iterBegin != iterEnd ; ++iterBegin )
        {
            this->base().insert( this->template functor<typename base_::value_to_base>()(
                typename base_::value_type(*iterBegin)) );
        }
    }

    typename base_::iterator insert(
        typename ::boost::call_traits< typename base_::value_type >::param_type x)
    {
        return this->base().insert( this->template functor<typename base_::value_to_base>()(x) );
    }

    typename base_::iterator insert(typename base_::iterator pos,
                    typename ::boost::call_traits< typename base_::value_type >::param_type x)
    {
        return this->template functor<typename base_::iterator_from_base>()(
            this->base().insert(this->template functor<typename base_::iterator_to_base>()(pos),
            this->template functor<typename base_::value_to_base>()(x))
        );
    }
};


} // namespace container_adaptor
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_MULTIMAP_ADAPTOR_HPP


