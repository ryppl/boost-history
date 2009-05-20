// Pinhole category_iterator.hpp file
//
// Copyright Jared McIntyre 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CATEGORY_ITERATOR
#define BOOST_CATEGORY_ITERATOR

#include <exception>
#include <stdexcept>

#include "types.hpp"
#include "property_group.hpp"

#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning(disable: 4561 4996)
#endif
#include <boost/iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/static_assert.hpp>
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

namespace boost { namespace pinhole
{
    /** @todo: comment */
    template <class Iterator>
    class category_iterator
        : public boost::iterator_adaptor<
            category_iterator<Iterator>
            , Iterator
        >
    {
    private:

        /** Required by boost::iterator_adaptor. */
        friend class boost::iterator_core_access;

        /** Required by boost::iterator_adaptor. */
        struct enabler {}; // a private type avoids misuse

        /** Required by boost::iterator_adaptor. */
        category_iterator() :
            category_iterator::iterator_adaptor_() { }
        
    public:
        
        explicit category_iterator( const std::string& category, Iterator x, Iterator end ) :
            category_iterator::iterator_adaptor_(x), search_category(category), current_end(end) { MoveIteratorToValidPosition(); }

        /** Required by boost::iterator_adaptor. Allows for proper conversion between otherwise incompatible iterator types. */
        template <class other_iterator>
        category_iterator(
               category_iterator<other_iterator> const& other
             , typename boost::enable_if<
                   boost::is_convertible<other_iterator*, Iterator*>
                 , enabler
               >::type = enabler()
             )
        : category_iterator::iterator_adaptor_(other.base()) {}

    private:

        void MoveIteratorToValidPosition()
        {
            while( current_end != this->base_reference() )
            {
                const boost::pinhole::category_collection& categories = (*this->base_reference())->get_category_collection();
                if( categories.end() != std::find(categories.begin(), categories.end(), search_category) )
                {
                    break;
                }
                ++(this->base_reference());
            }
        }

        void increment()
        {
            ++(this->base_reference());

            MoveIteratorToValidPosition();
        }

        const std::string search_category;
        Iterator current_end;
    };

    template <class Iterator>
    category_iterator<Iterator> make_category_iterator(const std::string& category, Iterator x, Iterator end)
    {
        return category_iterator<Iterator>(category, x, end);
    }
}}

#endif // include guard