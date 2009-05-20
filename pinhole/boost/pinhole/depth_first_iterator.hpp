// Pinhole depth_first_iterator.hpp file
//
// Copyright Jared McIntyre 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DEPTH_FIRST_ITERATOR
#define BOOST_DEPTH_FIRST_ITERATOR

#include <string>
#include "detail/depth_first_iterator_base.hpp"

namespace boost { namespace pinhole
{
    template<class Iterator>
    struct no_filter : public detail::depth_first_iterator_base<Iterator, no_filter<Iterator> >
    {
        typedef detail::depth_first_iterator_base< Iterator, no_filter<Iterator> > base_type;

        explicit no_filter( Iterator end ) :
            base_type(end, end)
        {
        }
        
        explicit no_filter( Iterator x, Iterator end ) :
            base_type(x, end)
        {
            // required call by depth_first_iterator_base
            base_type::MoveIteratorToValidPosition();
        }

        inline bool visit_children(property_group* property)
        {
            return true;
        }

        inline bool is_valid(property_group* property)
        {
            return true;
        }

        inline void move_up()
        {
            
        }

    private:

        no_filter() :
            base_type() { }
    };

    /** Iterates over all the keys in a map. */
    typedef no_filter<children_collection::iterator> depth_first_iterator;
    typedef no_filter<children_collection::const_iterator> const_depth_first_iterator;
}}

#endif // include guard