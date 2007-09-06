// (C) Copyright Jeremy Siek 1999-2001.
// (C) Copyright Andrew Sutton 2007
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_MAP_ITERATOR_PROPERTY_MAP_HPP
#define BOOST_PROPERTY_MAP_ITERATOR_PROPERTY_MAP_HPP

#include <boost/assert.hpp>
#include <boost/detail/iterator.hpp>
#include <boost/property_map/property_traits.hpp>
#include <boost/property_map/concepts.hpp>
#include <boost/property_map/detail/put_get_helper.hpp>

namespace boost { namespace property_map {

    // NOTE: This class depends on std::iterator_traits so we're going to
    // refer back to boost::detail::iterator_traits for a correct implementation
    // on all platforms (?).

    // The iterator property map povides get() and put() functions for random
    // access containers such that their random access iterators act as the
    // key type for access.
    //
    // Iterator property maps rely on a index map (which is also a property map)
    // that provides, via its get() function, an offset from the iterator supplied
    // during construction of this property map. The index map generally gives
    // a value who's type is convertible to the iterator's difference_type.
    //
    // This gives rise to one of the "stranger" aspects of the class - that its
    // key type is the same as that of the IndexMap over which it is instantiated.
    // The strange implication is that the indices represented by the index map
    // must correspond to the offset from the initial iterator given in the
    // constructor. The net result is that, if get(index, key) == i, then
    // imap[key] returns a reference to the ith object past iter. The implication
    // here is that the indices of objects are generally required to be in the
    // range [0, n).
    template <
        class RandomAccessIterator,
        class IndexMap,
        class Type = typename boost::detail::iterator_traits<RandomAccessIterator>::value_type,
        class Reference = typename boost::detail::iterator_traits<RandomAccessIterator>::reference
    >
    class iterator_property_map
        : public put_get_helper<
                Reference,
                iterator_property_map<
                        RandomAccessIterator,
                        IndexMap,
                        Type,
                        Reference
                    >
                >
    {
    public:
        typedef typename property_traits<IndexMap>::key_type key_type;
        typedef Type value_type;
        typedef Reference reference;
        typedef lvalue_property_map_tag category;

        inline iterator_property_map(RandomAccessIterator cc = RandomAccessIterator(),
                                     const IndexMap& id = IndexMap())
            : iter(cc)
            , index(id)
        { }

        inline reference operator[](const key_type& k) const
        { return *(iter + get(index, k)); }

    protected:
        RandomAccessIterator iter;
        IndexMap index;
    };

    // A generator function that constructs an interator property map given a
    // random access iterator and an index map. The value type and reference
    // type are those of the RandomAccessIterator type.
    //
    // NOTE: The requirement that IndexMap model the ReadablePropertyMap concept
    // is somewhat "recursive" since it both defines and the key type for the
    // property map being constructed and provides its type for the function.
    template <class RandomAccessIterator, class IndexMap>
    inline iterator_property_map<
            RandomAccessIterator,
            IndexMap,
            typename boost::detail::iterator_traits<RandomAccessIterator>::value_type,
            typename boost::detail::iterator_traits<RandomAccessIterator>::reference
        >
    make_iterator_property_map(RandomAccessIterator iter, IndexMap id)
    {
        function_requires< RandomAccessIteratorConcept<RandomAccessIterator> >();
        function_requires< ReadablePropertyMapConcept<IndexMap, typename property_traits<IndexMap>::key_type> >();
        typedef iterator_property_map<
                RandomAccessIterator,
                IndexMap,
                typename boost::detail::iterator_traits<RandomAccessIterator>::value_type,
                typename boost::detail::iterator_traits<RandomAccessIterator>::reference
            > PropertyMap;
        return PropertyMap(iter, id);
    }

    // Nearly identical to the iterator_property_map, this enforces the range
    // constraint (mentioned above) for the values of the IndexMap. Specifically,
    // this asserts at runtime that indices returned by the  index map fall
    // within the range [0, n). I suspect that there's an implicit requirement
    // that index types are unsigned.

    template <
        class RandomAccessIterator,
        class IndexMap,
        class Type = typename boost::detail::iterator_traits<RandomAccessIterator>::value_type,
        class Reference = typename boost::detail::iterator_traits<RandomAccessIterator>::reference
    >
    class safe_iterator_property_map
        : public put_get_helper<
                Reference,
                iterator_property_map<
                        RandomAccessIterator,
                        IndexMap,
                        Type,
                        Reference
                    >
                >
    {
    public:
        typedef typename property_traits<IndexMap>::key_type key_type;
        typedef Type value_type;
        typedef Reference reference;
        typedef lvalue_property_map_tag category;

        inline safe_iterator_property_map(RandomAccessIterator first,
                                          std::size_t n = 0,
                                          const IndexMap& id = IndexMap())
            : iter(first)
            , num(n)
            , index(id)
        { }

        inline safe_iterator_property_map()
        { }

        inline Reference operator[](const key_type& k) const
        {
            BOOST_ASSERT(get(index, k) < num);
            return *(iter + get(index, k)) ;
        }

        inline typename property_traits<IndexMap>::value_type
        size() const
        { return num; }

    protected:
        typedef typename property_traits<IndexMap>::value_type Index;

        RandomAccessIterator iter;
        Index num;
        IndexMap index;
    };

    // A generator function for creating safe iterator property maps.
    template <class RandomAccessIterator, class IndexMap>
    inline safe_iterator_property_map<
            RandomAccessIterator,
            IndexMap,
            typename boost::detail::iterator_traits<RandomAccessIterator>::value_type,
            typename boost::detail::iterator_traits<RandomAccessIterator>::reference
        >
    make_safe_iterator_property_map(RandomAccessIterator iter, std::size_t n, IndexMap id)
    {
        function_requires< RandomAccessIteratorConcept<RandomAccessIterator> >();
        function_requires< ReadablePropertyMapConcept<IndexMap, typename property_traits<IndexMap>::key_type> >();
        typedef safe_iterator_property_map<
                RandomAccessIterator,
                IndexMap,
                typename boost::detail::iterator_traits<RandomAccessIterator>::value_type,
                typename boost::detail::iterator_traits<RandomAccessIterator>::reference
            > PropertyMap;

        return PropertyMap(iter, n, id);
    }
} }

#endif
