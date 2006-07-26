// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file container_adaptor/detail/unordered_associative_container_adaptor.hpp
/// \brief Container adaptor to build a type that is compliant to the concept of an unordered associative container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_UNORDERED_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_UNORDERED_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP

#include <boost/bimap/container_adaptor/detail/associative_container_adaptor.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/push_front.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {
namespace detail {

template
<
    class Base, class Iterator, class ConstIterator,
    class LocalIterator, class ConstLocalIterator,
    class KeyType,
    class IteratorToBaseConverter, class IteratorFromBaseConverter,
    class LocalIteratorFromBaseConverter,
    class ValueToBaseConverter, class ValueFromBaseConverter,
    class KeyToBaseConverter,
    class FunctorsFromDerivedClasses
>
struct unordered_associative_container_adaptor_base
{

    typedef associative_container_adaptor
    <
        Base, Iterator, ConstIterator, KeyType,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ValueToBaseConverter   , ValueFromBaseConverter,
        KeyToBaseConverter,

        typename mpl::push_front<

            FunctorsFromDerivedClasses,

            typename mpl::if_< is_same< LocalIteratorFromBaseConverter, use_default >,
            // {
                    iterator_from_base_identity
                    <
                        typename Base::local_iterator                , LocalIterator,
                        typename Base::const_local_iterator          , ConstLocalIterator
                    >,
            // }
            // else
            // {
                    LocalIteratorFromBaseConverter
            // }

            >::type

        >::type

    > type;
};


/// \brief Container adaptor to build a type that is compliant to the concept of an unordered associative container.

template
<
    class Base,

    class Iterator,
    class ConstIterator,

    class LocalIterator,
    class ConstLocalIterator,

    class KeyType,

    class IteratorToBaseConverter        = use_default,
    class IteratorFromBaseConverter      = use_default,
    class LocalIteratorFromBaseConverter = use_default,
    class ValueToBaseConverter           = use_default,
    class ValueFromBaseConverter         = use_default,
    class KeyToBaseConverter             = use_default,

    class FunctorsFromDerivedClasses     = mpl::list<>

>
class unordered_associative_container_adaptor :

    public unordered_associative_container_adaptor_base
    <
        Base, Iterator, ConstIterator,
        LocalIterator, ConstLocalIterator,
        KeyType,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        LocalIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,
        FunctorsFromDerivedClasses

    >::type
{
    typedef typename unordered_associative_container_adaptor_base
    <
        Base, Iterator, ConstIterator,
        LocalIterator, ConstLocalIterator,
        KeyType,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        LocalIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        KeyToBaseConverter,
        FunctorsFromDerivedClasses

    >::type base_;

    // Metadata ---------------------------------------------------------------

    public:

    typedef typename Base::key_equal key_equal;
    typedef typename Base::hasher hasher;

    typedef LocalIterator      local_iterator;
    typedef ConstLocalIterator const_local_iterator;

    protected:

    typedef typename mpl::if_< is_same< LocalIteratorFromBaseConverter, use_default >,
        // {
                iterator_from_base_identity
                <
                    typename Base::local_iterator                , local_iterator,
                    typename Base::const_local_iterator          , const_local_iterator
                >,
        // }
        // else
        // {
                LocalIteratorFromBaseConverter
        // }

        >::type local_iterator_from_base;

    // Access -----------------------------------------------------------------

    public:

    explicit unordered_associative_container_adaptor(Base & c)
        : base_(c) {}

    protected:


    typedef unordered_associative_container_adaptor
    <
        Base,

        Iterator, ConstIterator, LocalIterator, ConstLocalIterator,

        KeyType,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        LocalIteratorFromBaseConverter,
        ValueToBaseConverter   , ValueFromBaseConverter,

        KeyToBaseConverter,

        FunctorsFromDerivedClasses

    > unordered_associative_container_adaptor_;

    // Interface --------------------------------------------------------------

    public:

    // bucket interface:

    typename base_::size_type bucket_count() const
    {
        return this->base().bucket_count();
    }

    typename base_::size_type max_bucket_count() const
    {
        return this->base().max_bucket_count();
    }

    typename base_::size_type bucket_size(typename base_::size_type n) const
    {
        return this->base().bucket_size(n);
    }

    typename base_::size_type bucket(const typename base_::key_type& k) const
    {
        typedef typename base_::key_to_base key_to_base;
        return this->base().bucket(
            this->template functor<key_to_base>()(k)
        );
    }

    local_iterator       begin(typename base_::size_type n)
    {
        return this->template functor<
            local_iterator_from_base
        >()                          ( this->base().begin(n) );
    }

    const_local_iterator begin(typename base_::size_type n) const
    {
        return this->template functor<
            local_iterator_from_base
        >()                          ( this->base().begin(n) );
    }

    local_iterator       end(typename base_::size_type n)
    {
        return this->template functor<
            local_iterator_from_base
        >()                          ( this->base().end(n) );
    }

    const_local_iterator end(typename base_::size_type n) const
    {
        return this->template functor<
            local_iterator_from_base
        >()                          ( this->base().end(n) );
    }

    // hash policy

    float load_factor() const
    {
        return this->base().load_factor();
    }

    float max_load_factor() const
    {
        return this->base().max_load_factor();
    }

    void max_load_factor(float z)
    {
        return this->base().max_load_factor(z);
    }

    void rehash(typename base_::size_type n)
    {
        return this->base().rehash(n);
    }

    // We have redefined end and begin so we have to manually route the old ones

    typename base_::iterator begin()
    {
        return base_::container_adaptor_::begin();
    }

    typename base_::iterator end()
    {
        return base_::container_adaptor_::end();
    }

    typename base_::const_iterator begin() const
    {
        return base_::container_adaptor_::begin();
    }

    typename base_::const_iterator end() const
    {
        return base_::container_adaptor_::end();
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


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_UNORDERED_ASSOCIATIVE_CONTAINER_ADAPTOR_HPP
