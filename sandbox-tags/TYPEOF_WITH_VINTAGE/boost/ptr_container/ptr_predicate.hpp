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

#ifndef BOOST_PTR_CONTAINER_PTR_PREDICATE_HPP
#define BOOST_PTR_CONTAINER_PTR_PREDICATE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
    #pragma once
#endif

#include <boost/config.hpp>
#include <functional>

namespace boost
{

    template< typename T >
    struct ptr_equal_to : public std::binary_function<T*,T*,bool>
    {
        bool operator()( const T* l, const T* r ) const
        {
            return *l == *r;
        }
    };
    
    template< typename T >
    struct ptr_not_equal_to : public std::binary_function<T*,T*,bool>
    {
        bool operator()( const T* l, const T* r ) const
        {
            return *l != *r;
        }
    };
    
    template< typename T >
    struct ptr_greater : public std::binary_function<T*,T*,bool>
    {
        bool operator()( const T* l, const T* r ) const
        {
            return *l > *r;
        }
    };
    
    template< typename T >
    struct ptr_less : public std::binary_function<T*,T*,bool>
    {
        bool operator()( const T* l, const T* r ) const
        {
            return *l < *r;
        }
    };
    
    template< typename T >
    struct ptr_greater_equal : public std::binary_function<T*,T*,bool>
    {
        bool operator()( const T* l, const T* r ) const
        {
            return *l >= *r;
        }
    };

    template< typename T >
    struct ptr_less_equal : public std::binary_function<T*,T*,bool>
    {
        bool operator()( const T* l, const T* r ) const
        {
            return *l <= *r;
        }
    };

    template< typename Predicate, typename A >
    class indirected1
    {
        Predicate pred_;
    public:
        
        typedef A*   argument_type;
        typedef bool result_type;
        
    public:
        indirected1( Predicate pred ) : pred_( pred )
        { }
        
        template< typename T >
        bool operator()( const T* x ) const
        {
            return pred_( *x );
        }        
    };
    
    template< typename BinaryPredicate >
    class indirected2
    {
        BinaryPredicate pred_;
    public:
        BOOST_DEDUCED_TYPENAME BinaryPredicate::first_argument_type*   first_argument_type;
        BOOST_DEDUCED_TYPENAME BinaryPredicate::second_argument_type*  second_argument_type;
        typedef bool                                                   result_type;
        
    public:
        indirected2( BinaryPredicate pred ) : pred_( pred )
        { }
        
        template< typename T >
        bool operator()( const T* l, const T* r )
        {
            return pred_( *l, *r );
        }
    };

    
    namespace ptr_container
    {
        namespace detail
        {
            template< typename T >
            class equal
            {
                const T& value_;
                
            public:
                typedef T*    argument_type;
                typedef bool  result_type;
                typedef T*    first_argument_type;
                typedef T*    second_argument_type;
                
            public:
                
                equal( const T& v ) : value_( v )
                { }
                
                bool operator()( const T& x ) const
                {
                    return x == value_;
                }
                
                bool operator()( const T& l, const T& r ) const
                {
                    return l == r;
                }
                
                bool operator()( const T* x ) const
                {
                    return *x == value_;
                }
                
                bool operator()( const T* l, const T* r ) const
                {
                    return *l == *r;
                }
            };
            
            template< typename T >
            class less_than
            {
            public:
                bool operator()( const T& l, const T& r ) const
                {
                    return l < r;
                }
                
                bool operator()( const T* l, const T* r ) const 
                {
                    return *l < *r;
                }
            };

        } // namespace 'detail'
            
    } // namespace 'ptr_container'
} // namespace 'boost'

#endif
