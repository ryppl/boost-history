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

#ifndef BOOST_PTR_CONTAINER_SCOPED_DELETER_HPP
#define BOOST_PTR_CONTAINER_SCOPED_DELETER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/scoped_array.hpp>

namespace boost
{
    namespace ptr_container
    {
        namespace detail
        {
            template< typename T >
            class scoped_deleter
            {
                scoped_array<T*>  ptrs_;
                int               stored_;
                bool              released_;
            public:
                scoped_deleter( int size ) : 
                    ptrs_( new T*[size] ), stored_( 0 ), released_( false )
                {
                    assert( size > 0 );
                }
                
                
                
                ~scoped_deleter()
                {
                    if ( !released_ )
                    {
                        for( int i = 0; i != stored_; ++i )
                            delete ptrs_[i]; 
                    }
                }
                
                
                
                void add( T* t )
                {
                    assert( ptrs_.get() != 0 );
                    ptrs_[stored_] = t;
                    ++stored_;
                }
                
                
                
                void release()
                {
                    released_ = true;
                }
                
                
                
                T** begin()
                {
                    assert( ptrs_.get() != 0 );
                    return &ptrs_[0];
                }
                
                
                
                T** end()
                {
                    assert( ptrs_.get() != 0 );
                    return &ptrs_[stored_];
                }
                
            }; // class 'scoped_deleter'
        }
    }
}

#endif
