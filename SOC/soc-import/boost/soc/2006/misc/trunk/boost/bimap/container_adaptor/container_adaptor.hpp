// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file container_adaptor/container_adaptor.hpp
/// \brief Container adaptor to build a type that is compliant to the concept of a container.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_CONTAINER_ADAPTOR_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_CONTAINER_ADAPTOR_HPP

#include <utility>

#include <boost/mpl/if.hpp>
#include <boost/mpl/aux_/na.hpp>
#include <boost/bimap/container_adaptor/detail/identity_converters.hpp>
#include <boost/iterator/iterator_traits.hpp>

#include <boost/bimap/container_adaptor/detail/functor_bag.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/front_inserter.hpp>
#include <boost/call_traits.hpp>

namespace boost {
namespace bimap {

/// \brief Container Adaptor toolbox, easy way to build new containers from existing ones.

namespace container_adaptor {

/// \brief Container adaptor to build a type that is compliant to the concept of a container.

template
<
    class Base,

    class Iterator,
    class ConstIterator,

    class IteratorToBaseConverter   = ::boost::mpl::na,
    class IteratorFromBaseConverter = ::boost::mpl::na,
    class ValueToBaseConverter      = ::boost::mpl::na,
    class ValueFromBaseConverter    = ::boost::mpl::na,

    class FunctorsFromDerivedClasses = mpl::vector<>
>
class container_adaptor
{
    // MetaData -------------------------------------------------------------

    public:

    typedef Iterator iterator;
    typedef ConstIterator const_iterator;

    typedef typename iterator_value    <       iterator >::type value_type;
    typedef typename iterator_pointer  <       iterator >::type pointer;
    typedef typename iterator_reference<       iterator >::type reference;
    typedef typename iterator_reference< const_iterator >::type const_reference;

    typedef typename Base::size_type size_type;
    typedef typename Base::difference_type difference_type;

    typedef typename mpl::if_< ::boost::mpl::is_na<IteratorToBaseConverter>,
        // {
                ::boost::bimap::container_adaptor::detail::iterator_to_base_identity
                <
                    typename Base::iterator                , iterator,
                    typename Base::const_iterator          , const_iterator
                >,
        // }
        // else
        // {
                IteratorToBaseConverter
        // }

        >::type iterator_to_base;

    typedef typename mpl::if_< ::boost::mpl::is_na<IteratorFromBaseConverter>,
        // {
                ::boost::bimap::container_adaptor::detail::iterator_from_base_identity
                <
                    typename Base::iterator                , iterator,
                    typename Base::const_iterator          , const_iterator
                >,
        // }
        // else
        // {
                IteratorFromBaseConverter
        // }

        >::type iterator_from_base;

    typedef typename mpl::if_< ::boost::mpl::is_na<ValueToBaseConverter>,
        // {
                ::boost::bimap::container_adaptor::detail::value_to_base_identity
                <
                    typename Base::value_type,
                    value_type
                >,
        // }
        // else
        // {
                ValueToBaseConverter
        // }

        >::type value_to_base;

    typedef typename mpl::if_< ::boost::mpl::is_na<ValueFromBaseConverter>,
        // {
                ::boost::bimap::container_adaptor::detail::value_from_base_identity
                <
                    typename Base::value_type,
                    value_type
                >,
        // }
        // else
        // {
                ValueFromBaseConverter
        // }

        >::type value_from_base;

    // ACCESS -----------------------------------------------------------------

    public:

    explicit container_adaptor(Base & c) : dwfb(c) {}

    protected:

    typedef Base base_type;

    typedef container_adaptor container_adaptor_;

    const Base & base() const { return dwfb.data; }
          Base & base()       { return dwfb.data; }

    // Interface --------------------------------------------------------------

    public:

    size_type size() const                    { return base().size();         }
    size_type max_size() const                { return base().max_size();     }
    bool empty() const                        { return base().empty();        }

    iterator begin()
    {
        return functor<iterator_from_base>()( base().begin() );
    }

    iterator end()
    {
        return functor<iterator_from_base>()( base().end() );
    }

    const_iterator begin() const
    {
        return functor<iterator_from_base>()( base().begin() );
    }

    const_iterator end() const
    {
        return functor<iterator_from_base>()( base().end() );
    }


    iterator erase(iterator pos)
    {
        return functor<iterator_from_base>()(
            base().erase(functor<iterator_to_base>()(pos))
        );
    }

    iterator erase(iterator first, iterator last)
    {
        return functor<iterator_from_base>()(
            base().erase(
                functor<iterator_to_base>()(first),
                functor<iterator_to_base>()(last)
            )
        );
    }

    void clear()
    {
        base().clear();
    }

    template <class InputIterator>
    void insert(InputIterator iterBegin, InputIterator iterEnd)
    {

        // TODO
        // This is may be the correct way to do it, but i have to work a little
        // bit more on it to make it work. There are problems with (*)
        /*
            typedef transform_iterator
            <
                value_to_base,
                InputIterator,
                const typename Base::value_type & (*)

            > value_to_base_iterator;

            base().insert(
                value_to_base_iterator(iterBegin, functor<value_to_base>()),
                value_to_base_iterator(iterEnd  , functor<value_to_base>())
            );
        */

        // Go simpler for now, this can be optimized

        for( ; iterBegin != iterEnd ; ++iterBegin )
        {
            base().insert( functor<value_to_base>()( value_type(*iterBegin)) );
        }

    }

    std::pair<iterator, bool> insert(typename ::boost::call_traits< value_type >::param_type x)
    {
        std::pair< typename Base::iterator, bool > r(
            base().insert( functor<value_to_base>()(x) )
        );

        return std::pair<iterator, bool>(
                    functor<iterator_from_base>()(r.first),r.second
               );

    }

    iterator insert(iterator pos, typename ::boost::call_traits< value_type >::param_type x)
    {
        return functor<iterator_from_base>()(
            base().insert(functor<iterator_to_base>()(pos),functor<value_to_base>()(x))
        );
    }

    // Access to functors ----------------------------------------------------

    protected:

    template< class Functor >
    Functor & functor()
    {
        return dwfb.functor<Functor>();
    }

    template< class Functor >
    Functor const & functor() const
    {
        return dwfb.functor<Functor>();
    }

    // Data ------------------------------------------------------------------

    private:

    ::boost::bimap::container_adaptor::detail::data_with_functor_bag
    <
        Base &,

        typename mpl::copy
        <
            mpl::vector
            <
                iterator_to_base,
                iterator_from_base,
                value_to_base,
                value_from_base
            >,

            mpl::front_inserter< FunctorsFromDerivedClasses >

        >::type

    > dwfb;
};


} // namespace container_adaptor
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_CONTAINER_ADAPTOR_HPP
