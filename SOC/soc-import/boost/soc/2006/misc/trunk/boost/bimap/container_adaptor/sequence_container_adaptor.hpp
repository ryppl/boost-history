// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file container_adaptor/sequence_container_adaptor.hpp
/// \brief Container adaptor to build a type that is compliant to the concept of a weak associative container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_SEQUENCE_CONTAINER_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_SEQUENCE_CONTAINER_ADAPTOR_HPP

#include <utility>

#include <boost/mpl/if.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/bimap/container_adaptor/detail/identity_converters.hpp>
#include <boost/bimap/container_adaptor/container_adaptor.hpp>
#include <boost/call_traits.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template
<
    class Base, class Iterator, class ConstIterator,
    class ReverseIterator, class ConstReverseIterator,
    class IteratorToBaseConverter, class IteratorFromBaseConverter,
    class ReverseIteratorFromBaseConverter,
    class ValueToBaseConverter, class ValueFromBaseConverter,
    class FunctorsFromDerivedClasses
>
struct sequence_container_adaptor_base
{
    typedef container_adaptor
    <
        Base, Iterator, ConstIterator,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,

        typename mpl::push_front<

            FunctorsFromDerivedClasses,

            typename mpl::if_< ::boost::mpl::is_na<ReverseIteratorFromBaseConverter>,
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

    > type;
};

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

/// \brief Container adaptor to build a type that is compliant to the concept of a sequence container.

template
<
    class Base,

    class Iterator,
    class ConstIterator,

    class ReverseIterator,
    class ConstReverseIterator,

    class IteratorToBaseConverter           = ::boost::mpl::na,
    class IteratorFromBaseConverter         = ::boost::mpl::na,
    class ReverseIteratorFromBaseConverter  = ::boost::mpl::na,
    class ValueToBaseConverter              = ::boost::mpl::na,
    class ValueFromBaseConverter            = ::boost::mpl::na,

    class FunctorsFromDerivedClasses = mpl::list<>
>
class sequence_container_adaptor :

    public sequence_container_adaptor_base
    <
        Base, Iterator, ConstIterator,
        ReverseIterator, ConstReverseIterator,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        FunctorsFromDerivedClasses

    >::type,

    ::boost::totally_ordered
    <
        sequence_container_adaptor
        <
            Base, Iterator, ConstIterator, ReverseIterator, ConstReverseIterator,
            IteratorToBaseConverter, IteratorFromBaseConverter,
            ReverseIteratorFromBaseConverter, ValueToBaseConverter, ValueFromBaseConverter,
            FunctorsFromDerivedClasses
        >
    >
{
    typedef typename sequence_container_adaptor_base
    <
        Base, Iterator, ConstIterator,
        ReverseIterator, ConstReverseIterator,
        IteratorToBaseConverter, IteratorFromBaseConverter,
        ReverseIteratorFromBaseConverter,
        ValueToBaseConverter, ValueFromBaseConverter,
        FunctorsFromDerivedClasses

    >::type base_;

    // MetaData -------------------------------------------------------------

    public:

    typedef ReverseIterator      reverse_iterator;
    typedef ConstReverseIterator const_reverse_iterator;

    protected:

    typedef typename mpl::if_< ::boost::mpl::is_na<ReverseIteratorFromBaseConverter>,
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
        : base_(c) {}

    protected:


    typedef sequence_container_adaptor sequence_container_adaptor_;

    // Interface --------------------------------------------------------------

    public:

    reverse_iterator rbegin()
    {
        return this->template functor<
            reverse_iterator_from_base
        >()                            ( this->base().rbegin() );

    }

    reverse_iterator rend()
    {
        return this->template functor<
            reverse_iterator_from_base
        >()                            ( this->base().rend() );
    }

    const_reverse_iterator rbegin() const
    {
        return this->template functor<
            reverse_iterator_from_base
        >()                            ( this->base().rbegin() );
    }

    const_reverse_iterator rend() const
    {
        return this->template functor<
            reverse_iterator_from_base
        >()                            ( this->base().rend() );
    }

    void resize(typename base_::size_type n,
                typename ::boost::call_traits< typename base_::value_type >::param_type x =
                    typename base_::value_type())
    {
        this->base().resize(n,
            this->template functor<typename base_::value_to_base>()(x)
        );
    }

    typename base_::reference front()
    {
        return this->template functor<
            typename base_::value_from_base>()
        (
            this->base().front()
        );
    }

    typename base_::reference back()
    {
        return this->template functor<
            typename base_::value_from_base>()
        (
            this->base().back()
        );
    }

    typename base_::const_reference front() const
    {
        return this->template functor<
            typename base_::value_from_base>()
        (
            this->base().front()
        );
    }

    typename base_::const_reference back() const
    {
        return this->template functor<
            typename base_::value_from_base>()
        (
            this->base().back()
        );
    }

    void push_front(typename ::boost::call_traits< typename base_::value_type >::param_type x)
    {
        this->base().push_front(
            this->template functor<typename base_::value_to_base>()(x));
    }

    void pop_front()
    {
        this->base().pop_front();
    }

    void push_back(typename ::boost::call_traits< typename base_::value_type >::param_type x)
    {
        this->base().push_back(
            this->template functor<typename base_::value_to_base>()(x));
    }

    void pop_back()
    {
        this->base().pop_back();
    }

    std::pair<typename base_::iterator,bool>
    insert(typename base_::iterator position,
           typename ::boost::call_traits< typename base_::value_type >::param_type x)
    {
        std::pair< typename Base::iterator, bool > r(
            this->base().insert(
                this->template functor<typename base_::iterator_to_base>()(position),
                this->template functor<typename base_::value_to_base   >()(x)
            )
        );

        return std::pair<typename base_::iterator, bool>(
            this->template functor<typename base_::iterator_from_base>()(r.first),
            r.second
        );
    }

    void insert(typename base_::iterator position,
                typename base_::size_type m,
                typename ::boost::call_traits< typename base_::value_type >::param_type x)
    {
        this->base().insert(
            this->template functor<typename base_::iterator_to_base>()(position),
            m,
            this->template functor<typename base_::value_to_base   >()(x)
        );
    }

    template<typename InputIterator>
    void insert(typename base_::iterator position,
                InputIterator first, InputIterator last)
    {
        // This is the same problem found in the insert function of container_adaptor
        // For now, do the simple thing. This can be optimized

        for( ; first != last ; ++first )
        {
            this->base().insert(
                this->template functor<typename base_::iterator_to_base>()( position ),
                this->template functor<typename base_::value_to_base   >()( typename base_::value_type(*first) )
            );
        }
    }

    // Totally ordered implementation

    bool operator==(const sequence_container_adaptor & c) const
    {
        return ( this->base() == c.base() );
    }

    bool operator<(const sequence_container_adaptor & c) const
    {
        return ( this->base() < c.base() );
    }
};

} // namespace container_adaptor
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_SEQUENCE_CONTAINER_ADAPTOR_HPP
