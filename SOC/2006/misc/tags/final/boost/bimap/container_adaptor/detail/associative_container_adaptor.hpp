// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// This code may be used under either of the following two licences:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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

    typedef weak_associative_container_adaptor
    <
        Base, Iterator, ConstIterator, KeyType,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter, KeyToBaseConverter,
        FunctorsFromDerivedClasses

    > base_;

    // ACCESS -----------------------------------------------------------------

    public:

    explicit associative_container_adaptor(Base & c)
        : base_(c) {}

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

    typename base_::size_type
        erase(const typename base_::key_type& k)
    {
        typedef typename base_::key_to_base key_to_base;
        return this->base().erase
        (
            this->template functor<key_to_base>()(k)
        );
    }

    // As we redefine erase, the other overloads need to be manually routed

    typename base_::iterator erase(
        typename base_::iterator pos)
    {
        return base_::container_adaptor_::erase(pos);
    }

    typename base_::iterator erase(
        typename base_::iterator first,
        typename base_::iterator last)
    {
        return base_::container_adaptor_::erase(first,last);
    }

    typename base_::size_type
        count(const typename base_::key_type& k)
    {
        return this->base().count(
            this->template functor<typename base_::key_to_base>()(k)
        );
    }

    typename base_::iterator
        find(const typename base_::key_type& k)
    {
        typedef typename base_::key_to_base key_to_base;
        return this->template functor<typename base_::iterator_from_base>()
        (
            this->base().find(
                this->template functor<key_to_base>()(k)
            )
        );
    }

    typename base_::const_iterator
        find(const typename base_::key_type& k) const
    {
        typedef typename base_::key_to_base key_to_base;
        return this->template functor<
            typename base_::iterator_from_base>()
        (
            this->base().find(
                this->template functor<key_to_base>()(k)
            )
        );
    }

    std::pair
    <
        typename base_::iterator,
        typename base_::iterator
    >
        equal_range(const typename base_::key_type& k)
    {
        std::pair<

            typename Base::iterator,
            typename Base::iterator

        > r( this->base().equal_range(
                this->template functor<typename base_::key_to_base>()(k)
            )
        );

        return std::pair
        <
            typename base_::iterator,
            typename base_::iterator
        >(
            this->template functor<
                typename base_::iterator_from_base
            >()                                         ( r.first ),
            this->template functor<
                typename base_::iterator_from_base
            >()                                         ( r.second )
        );
    }

    std::pair
    <
        typename base_::const_iterator,
        typename base_::const_iterator
    >
        equal_range(const typename base_::key_type& k) const
    {
        std::pair<

            typename Base::const_iterator,
            typename Base::const_iterator

        > r( this->base().equal_range(
                this->template functor<typename base_::key_to_base>()(k)
            )
        );

        return std::pair
        <
            typename base_::const_iterator,
            typename base_::const_iterator
        >(
            this->template functor<
                typename base_::iterator_from_base
            >()                                         ( r.first ),
            this->template functor<
                typename base_::iterator_from_base
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



