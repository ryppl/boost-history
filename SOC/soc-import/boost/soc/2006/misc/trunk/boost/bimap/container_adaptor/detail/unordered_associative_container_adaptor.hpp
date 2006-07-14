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
#include <boost/mpl/copy.hpp>
#include <boost/mpl/front_inserter.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {
namespace detail {

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

    class FunctorsFromDerivedClasses = mpl::list<>

>
class unordered_associative_container_adaptor :

    public associative_container_adaptor
    <
        Base,

        Iterator, ConstIterator,

        KeyType,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ValueToBaseConverter   , ValueFromBaseConverter,

        typename mpl::copy<

            mpl::list
            <
                typename mpl::if_< is_same< LocalIteratorFromBaseConverter, use_default >,
                // {
                        detail::iterator_from_base_identity
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
            >,
            mpl::front_inserter< FunctorsFromDerivedClasses >

        >::type
    >
{

    // Metadata ---------------------------------------------------------------

    private:

    typedef unordered_associative_container_adaptor this_type;

    public:

    typedef typename Base::key_equal key_equal;
    typedef typename Base::hasher hasher;

    typedef LocalIterator      local_iterator;
    typedef ConstLocalIterator const_local_iterator;

    typedef typename mpl::if_< is_same< LocalIteratorFromBaseConverter, use_default >,
        // {
                detail::iterator_from_base_identity
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
        : this_type::associative_container_adaptor_(c) {}

    protected:


    typedef unordered_associative_container_adaptor
    <
        Base,

        Iterator, ConstIterator, LocalIterator, ConstLocalIterator,

        KeyType,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        LocalIteratorFromBaseConverter,
        ValueToBaseConverter   , ValueFromBaseConverter,

        FunctorsFromDerivedClasses

    > unordered_associative_container_adaptor_;

    // Interface --------------------------------------------------------------

    public:

    // bucket interface:

    typename this_type::size_type bucket_count() const
    {
        return this_type::base().bucket_count();
    }

    typename this_type::size_type max_bucket_count() const
    {
        return this_type::base().max_bucket_count();
    }

    typename this_type::size_type bucket_size(typename this_type::size_type n) const
    {
        return typename this_type::base().bucket_size(n);
    }

    typename this_type::size_type bucket(const typename this_type::key_type& k) const
    {
        return this_type::base().bucket(k);
    }

    local_iterator       begin(typename this_type::size_type n)
    {
        return this_type::template functor<
            local_iterator_from_base
        >()                          ( this_type::base().begin(n) );
    }

    const_local_iterator begin(typename this_type::size_type n) const
    {
        return this_type::template functor<
            local_iterator_from_base
        >()                          ( this_type::base().begin(n) );
    }

    local_iterator       end(typename this_type::size_type n)
    {
        return this_type::dwfb.template functor<
            local_iterator_from_base
        >()                          ( this_type::base().end(n) );
    }

    const_local_iterator end(typename this_type::size_type n) const
    {
        return this_type::template functor<
            local_iterator_from_base
        >()                          ( this_type::base().end(n) );
    }

    // hash policy

    float load_factor() const
    {
        return this_type::base().load_factor();
    }

    float max_load_factor() const
    {
        return this_type::base().max_load_factor();
    }

    void max_load_factor(float z)
    {
        return this_type::base().max_load_factor(z);
    }

    void rehash(typename this_type::size_type n)
    {
        return this_type::base().rehash(n);
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
