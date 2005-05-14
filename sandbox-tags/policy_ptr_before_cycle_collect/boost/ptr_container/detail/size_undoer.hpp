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

#ifndef BOOST_PTR_CONTAINER_DETAIL_SIZE_UNDOER_HPP
#define BOOST_PTR_CONTAINER_DETAIL_SIZE_UNDOER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
    #pragma once
#endif

#include <boost/config.hpp>

namespace boost
{
namespace ptr_container
{
    namespace detail
    {
        template< typename C >
        class size_undoer 
        {
            typedef BOOST_DEDUCED_TYPENAME C::size_type size_type;

            C&               c_;
            const size_type  size_;
            bool             released_;

        public:
            size_undoer( C& c ) : c_( c ), size_( c_.size() ), released_( false ) 
            {}



            ~size_undoer() 
            {
                if( !released_ )
                {
                    assert( c_.size() >= size_ );
                    c_.resize( size_ );
                }
            }



            void release()
            {
                released_ = true;
            }



            size_type old_size() const
            {
                return size_;
            }
        };
    }
}
}

#endif
