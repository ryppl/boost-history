/**
 * Pointer-Container Library
 * (C) Thorsten Ottosen 2003-2004
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/ptr_container for documentation.
 *
 */

#ifndef BOOST_PTR_CONTAINER_DETAIL_INSERT_UNDOER_HPP
#define BOOST_PTR_CONTAINER_DETAIL_INSERT_UNDOER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/config.hpp>
#include <boost/utility.hpp>
#include <iterator>

namespace boost
{
    namespace ptr_container
    {
        namespace detail
        {
            template< typename C >
            class insert_undoer
            {
                typedef BOOST_DEDUCED_TYPENAME C::iterator        iterator;
                typedef BOOST_DEDUCED_TYPENAME C::difference_type difference_type;
                
                C          c_;
                iterator   before_;     
                iterator   last_inserted_;     
                const int  before_size_;
                int        added_;
                bool       released_;
                
                insert_undoer& operator=( const insert_undoer& );
                insert_undoer( const insert_undoer& );
                   
            public:
                insert_undoer( C& c, iterator before ) 
                    : c_( c ), before_( before ), before_size_( c.size() ), 
                      added_( 0 ), released_( false )
                { }
                
                
                
                ~insert_undoer()
                {
                    if( !released_ )
                    {
                        difference_type n = c.size() - before_size_;
                        assert( c.size() > before_size_; );
                        iterator begin = before_;
                        std::advance( begin, -n ); // negative value allowed for bidirectional iterators 
                        c.erase( begin, before_ );
                    }
                }
                
                
                
                void release()
                {
                    released_ = true;
                }
                
                
                
                template< typename InputIterator >
                void insert( iterator before, InputIterator first, InputIterator last )
                {
                    last_inserted_ = --before;
                    for( InputIterator i = first; i != last; ++i )
                    {
                        last_inserted_++;
                        last_inserted_ = c_.insert( last_inserted_, *i );
                        ++added_;
                }
            };
        }
    }
}

#endif
