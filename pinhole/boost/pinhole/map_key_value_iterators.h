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
    /** Iterates over all the keys in a map. */
    template <class Iterator>
    class map_key_iterator
        : public boost::iterator_adaptor<
              map_key_iterator<Iterator>
            , Iterator
            , boost::use_default
            , boost::use_default
            , typename std::iterator_traits<Iterator>::value_type::first_type
        >
    {
    private:
        typedef typename std::iterator_traits<Iterator>::value_type::first_type Reference;

        struct enabler {}; // a private type avoids misuse
        
        friend class boost::iterator_core_access;
        
    public:
        map_key_iterator() :
            map_key_iterator::iterator_adaptor_() { }
        
        explicit map_key_iterator( Iterator x, Iterator end = Iterator()) :
            map_key_iterator::iterator_adaptor_(x) { }
        
        template <class other_iterator>
        map_key_iterator(
               map_key_iterator<other_iterator> const& other
             , typename boost::enable_if<
                   boost::is_convertible<other_iterator*, Iterator*>
                 , enabler
               >::type = enabler()
             )
        : map_key_iterator::iterator_adaptor_(other.base()) {}
        
        Reference dereference() const
        {
            return (*this->base()).first;
        }
    };
    
    /** Factory for creating map_key_iterator iterators. */
    template <class Iterator>
    map_key_iterator<Iterator> make_map_key_iterator(Iterator x, Iterator end = Iterator())
    {
            return map_key_iterator<Iterator>(x,end);
    }

    /** Iterates over all values in a map. */
    template <class Iterator>
    class map_value_iterator
        : public boost::iterator_adaptor<
              map_value_iterator<Iterator>
            , Iterator
            , boost::use_default
            , boost::use_default
            , typename std::iterator_traits<Iterator>::value_type::second_type
        >
    {
    private:
        typedef typename std::iterator_traits<Iterator>::value_type::second_type Reference;
        
        struct enabler {}; // a private type avoids misuse
        
        friend class boost::iterator_core_access;
        
    public:
        map_value_iterator() :
            map_value_iterator::iterator_adaptor_() { }
        
        map_value_iterator( Iterator x, Iterator end = Iterator()) :
            map_value_iterator::iterator_adaptor_(x) { }
        
        template <class other_iterator>
        map_value_iterator(
              map_value_iterator<other_iterator> const& other
            , typename boost::enable_if<
                  boost::is_convertible<other_iterator*, Iterator*>
                , enabler
              >::type = enabler()
            )
        : map_value_iterator::iterator_adaptor_(other.base()) {}
        
        Reference dereference() const
        {
            return (*this->base()).second;
        }
    };
    
    /** Factory for creating map_value_iterator iterators. */
    template <class Iterator>
    map_value_iterator<Iterator> make_map_value_iterator(Iterator x, Iterator end = Iterator())
    {
            return map_value_iterator<Iterator>(x,end);
    }
}}

#endif // include guard