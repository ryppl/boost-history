// MscProperty system map_key_value_iterators.h file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MAP_KEY_VALUE_ITERATORS
#define BOOST_MAP_KEY_VALUE_ITERATORS

#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning(disable: 4561 4996)
#endif
#include <boost/iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_categories.hpp>

#include <boost/type_traits/is_pointer.hpp>
#include <boost/static_assert.hpp>
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

namespace boost { namespace pinhole
{
    template <class Reference, class Iterator>
    class map_key_iterator;

    namespace map_key_iterator_detail
    {
        template <class Reference, class Iterator>
        struct iterator_base
        {
            typedef boost::iterator_adaptor<
                map_key_iterator<Reference, Iterator>
              , Iterator                        // Base
              , boost::use_default              // Value
              , boost::use_default//boost::forward_traversal_tag    // CategoryOrTraversal
              , Reference
            > type;
        };
    }

    /** Iterates over all the keys in a map. */
    template <class Reference, class Iterator>
    class map_key_iterator
        : public map_key_iterator_detail::iterator_base<Reference, Iterator>::type
    {
     private:
        struct enabler {};  // a private type avoids misuse
        
        typedef typename map_key_iterator_detail::iterator_base<
              Reference, Iterator
          >::type super_t;

        friend class iterator_core_access;

     public:
        map_key_iterator() { }

        map_key_iterator( Iterator x, Iterator end = Iterator()) : super_t(x) { }

        Reference dereference() const
        {
            return (*this->base()).first;
        }
    };

    /** Factory for creating map_key_iterator iterators. */
    template <class Reference, class Iterator>
    map_key_iterator<Reference, Iterator>
        make_map_key_iterator(Iterator x, Iterator end = Iterator())
    {
        return map_key_iterator<Reference,Iterator>(x,end);
    }

    template <class Reference, class Iterator>
    class map_value_iterator;

    namespace map_value_iterator_detail
    {
        template <class Reference, class Iterator>
        struct iterator_base
        {
            typedef boost::iterator_adaptor<
                map_value_iterator<Reference, Iterator>
              , Iterator                        // Base
              , boost::use_default              // Value
              , boost::use_default//boost::forward_traversal_tag    // CategoryOrTraversal
              , Reference
            > type;
        };
    }

    /** Iterates over all the keys in a map. */
    template <class Reference, class Iterator>
    class map_value_iterator
        : public map_value_iterator_detail::iterator_base<Reference, Iterator>::type
    {
     private:
        struct enabler {};  // a private type avoids misuse
        
        typedef typename map_value_iterator_detail::iterator_base<
              Reference, Iterator
          >::type super_t;

        friend class iterator_core_access;

     public:
        map_value_iterator() { }

        map_value_iterator( Iterator x, Iterator end = Iterator()) : super_t(x) { }

        Reference dereference() const
        {
            return (*this->base()).second;
        }
    };

    /** Factory for creating map_value_iterator iterators. */
    template <class Reference, class Iterator>
    map_value_iterator<Reference, Iterator>
        make_map_value_iterator(Iterator x, Iterator end = Iterator())
    {
        return map_value_iterator<Reference,Iterator>(x,end);
    }
}}

#endif // include guard