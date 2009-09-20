//
// iterator_range.hpp - container-like streaming for iterator_range
//
// Copyright (C) 2007, Jeffrey Faust
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/explore for library home page.

#ifndef BOOST_EXPLORE_ITERATOR_RANGE_H
#define BOOST_EXPLORE_ITERATOR_RANGE_H

#include "is_assoc_iter.hpp"
#include "stream_value.hpp"

#include <boost/range/iterator_range.hpp>
#include <boost/mpl/if.hpp>

namespace boost { namespace explore
{
    // To work around some problems with overriding for operator<< for
    // boost::iterator_rangethere already exists such an operator that
    // does not do what we want.
    template<typename T>
    struct iterator_range_wrapper
    {
        iterator_range_wrapper(const boost::iterator_range<T>& ir)
            : t(ir) {}

        boost::iterator_range<T> t;
        typedef typename boost::mpl::if_<
            typename is_assoc_iter<T>::type,
            stream_associative_value,
            stream_normal_value>::type stream_type;
    };

    template<typename T>
    iterator_range_wrapper<T> as_container(const boost::iterator_range<T>& ir)
    {
        return iterator_range_wrapper<T>(ir);
    }

    template< typename IteratorT >
    inline iterator_range_wrapper<IteratorT>
    make_iterator_range(IteratorT Begin, IteratorT End)
    {
        return iterator_range_wrapper<IteratorT>(
            boost::make_iterator_range(Begin, End));
    }

    template< class ForwardRange >
    iterator_range_wrapper<
        BOOST_DEDUCED_TYPENAME boost::range_const_iterator<ForwardRange>::type>
        make_iterator_range(const ForwardRange& r)
    {
        return as_container(boost::make_iterator_range(r));
    }

    // stream boost::iterator_range
    template<typename Elem, typename Tr, typename T>
    std::basic_ostream<Elem, Tr>& operator<<(
        std::basic_ostream<Elem, Tr>& ostr,
        const iterator_range_wrapper<T>& r)
    {
        return stream_container(ostr, r.t.begin(), r.t.end(),
            typename iterator_range_wrapper<T>::stream_type());
    }
}}

#endif
