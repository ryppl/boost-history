// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file container_adaptor/detail/sequence_container_adaptor.hpp
/// \brief Container adaptor to build a type that is compliant to the concept of a weak associative container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_SEQUENCE_CONTAINER_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_SEQUENCE_CONTAINER_ADAPTOR_HPP

#include <utility>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/bimap/container_adaptor/detail/identity_converters.hpp>
#include <boost/bimap/container_adaptor/detail/container_adaptor.hpp>
#include <boost/bimap/container_adaptor/use_default.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {
namespace detail {

/// \brief Container adaptor to build a type that is compliant to the concept of a sequence container.

template
<
    class Base,

    class Iterator,
    class ConstIterator,

    class ReverseIterator,
    class ConstReverseIterator,

    class IteratorToBaseConverter           = use_default,
    class IteratorFromBaseConverter         = use_default,
    class ReverseIteratorFromBaseConverter  = use_default,
    class ValueToBaseConverter              = use_default,
    class ValueFromBaseConverter            = use_default,

    class FunctorsFromDerivedClasses = mpl::list<>
>
class sequence_container_adaptor :

    public container_adaptor
    <
        Base,

        Iterator, ConstIterator,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ValueToBaseConverter   , ValueFromBaseConverter,

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
    typedef sequence_container_adaptor this_type;

    // MetaData -------------------------------------------------------------

    public:

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

    explicit sequence_container_adaptor(Base & c)
        : sequence_container_adaptor::container_adaptor_(c) {}

    protected:


    typedef sequence_container_adaptor
    <
        Base,

        Iterator, ConstIterator,
        ReverseIterator, ConstReverseIterator,

        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter   , ValueFromBaseConverter,

        FunctorsFromDerivedClasses

    > sequence_container_adaptor_;

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

    void resize(typename this_type::size_type n,
                const typename this_type::value_type& x = typename this_type::value_type())
    {
        this_type::base().resize(n,
            this_type::template functor<typename this_type::value_to_base>()(x)
        );
    }

    typename this_type::reference front()
    {
        return this_type::template functor<
            typename this_type::value_from_base>()( this_type::base().front() );
    }

    typename this_type::reference back()
    {
        return this_type::template functor<
            typename this_type::value_from_base>()( this_type::base().back() );
    }

    typename this_type::const_reference front() const
    {
        return this_type::template functor<
            typename this_type::value_from_base>()( this_type::base().front() );
    }

    typename this_type::const_reference back() const
    {
        return this_type::template functor<
            typename this_type::value_from_base>()( this_type::base().back() );
    }

    std::pair<typename this_type::iterator,bool>
        push_front(const typename this_type::value_type& x)
    {
        std::pair< typename Base::iterator, bool > r(
            this_type::base().push_front(
                this_type::template functor<typename this_type::value_to_base>()(x)
            )
        );

        return std::pair<typename this_type::iterator, bool>(
            this_type::template functor<typename this_type::iterator_from_base>()(r.first),
            r.second
        );
    }

    void pop_front()
    {
        this_type::base().pop_front();
    }

    std::pair<typename this_type::iterator,bool>
        push_back(const typename this_type::value_type& x)
    {
        std::pair< typename Base::iterator, bool > r(
            this_type::base().push_back(
                this_type::template functor<typename this_type::value_to_base>()(x)
            )
        );

        return std::pair<typename this_type::iterator, bool>(
            this_type::template functor<typename this_type::iterator_from_base>()(r.first),
            r.second
        );
    }

    void pop_back()
    {
        this_type::base().pop_back();
    }

    std::pair<typename this_type::iterator,bool>
    insert(typename this_type::iterator position,const typename this_type::value_type& x)
    {
        std::pair< typename Base::iterator, bool > r(
            this_type::base().insert(
                this_type::template functor<typename this_type::iterator_to_base>()(position),
                this_type::template functor<typename this_type::value_to_base   >()(x)
            )
        );

        return std::pair<typename this_type::iterator, bool>(
            this_type::template functor<typename this_type::iterator_from_base>()(r.first),
            r.second
        );
    }

    void insert(typename this_type::iterator position,
                typename this_type::size_type m,
                const typename this_type::value_type& x)
    {
        this_type::base().insert(
            this_type::template functor<typename this_type::iterator_to_base>()(position),
            m,
            this_type::template functor<typename this_type::value_to_base   >()(x)
        );
    }

    template<typename InputIterator>
    void insert(typename this_type::iterator position,
                InputIterator first,InputIterator last)
    {
        // TODO
        // This is the same problem finded in the insert function of container_adaptor
        // For now, do the simple thing

        for( ; first != last ; ++first )
        {
            this_type::base().insert(
                this_type::template functor<typename this_type::iterator_to_base>()(position),
                this_type::template functor<typename this_type::value_to_base   >()(*first)
            );
        }
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


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_SEQUENCE_CONTAINER_ADAPTOR_HPP
