/**
 * Non-intrusive Assignment Library
 * (C) Thorsten Ottosen 2002-2003
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/assign for documentation.
 *
 */

#ifndef BOOST_ASSIGN_LIST_OF_HPP
#define BOOST_ASSIGN_LIST_OF_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/assign/exception.hpp>
#include <boost/config.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/mpl/if.hpp>
#include <deque>
#include <cstddef>
#include <utility>

namespace boost
{    
    template< class T, std::size_t sz >
    class array;
    
namespace assignment
{
namespace detail
{
    template< class T, std::size_t sz >
    type_traits::yes_type is_array( array<T,sz>* );
    type_traits::no_type is_array( ... );
    struct array_type_tag {};
    struct default_type_tag {};
    
    template< class T > 
    class generic_list : public std::deque<T> 
    {
    public:
        
        template< class U >
        generic_list& operator()( const U& u )
        {
            this->push_back( u );
            return *this;
        }
        
        /*
        template< std::size_t sz >
        generic_list& operator()( const char (&array)[sz] )
        {
            this->push_back( &array[0] );
            return *this;
        }
        
        template< std::size_t sz >
        generic_list& operator()( const wchar_t (&array)[sz] )
        {
            this->push_back( &array[0] );
            return *this;
        }
        */

        template< class U, class U2 >
        generic_list& operator()( U u, U2 u2 )
        {
            this->push_back( T( u, u2 ) );
            return *this;
        }

        template< class U, class U2, class U3 >
        generic_list& operator()( U u, U2& u2, U3 u3 )
        {
            this->push_back( T( u, u2, u3 ) );
            return *this;
        }

        template< class U, class U2, class U3, class U4 >
        generic_list& operator()( U u, U2& u2, U3 u3, U4 u4 )
        {
            this->push_back( T( u, u2, u3, u4 ) );
            return *this;
        }

        template< class U >
        generic_list& operator,( U u )
        {
            this->push_back( u );
            return *this;
        }
        
        /*
        template< std::size_t sz >
        generic_list& operator,( const char (&array)[sz] )
        {
            this->push_back( &array[0] );
            return *this;
        }
        
        template< std::size_t sz >
        generic_list& operator,( const wchar_t (&array)[sz] )
        {
            this->push_back( &array[0] );
            return *this;
        }
        */
        /*
        template< class U >
        generic_list& operator=( const U& r )
        {
            this->push_back( r );
            return *this;
        }
        
        template< class U >
        void operator+=( const generic_list<U>& r )
        {
            this->insert( this->end(), r.begin(), r.end() );
        }
        */
        
        template< class Container >
        operator Container() const
        {
            static Container* c = 0;
            typedef typename mpl::if_c< sizeof( is_array( c ) ) 
                                        == sizeof( type_traits::yes_type ), 
                                        array_type_tag, 
                                        default_type_tag >::type tag_type;
            return convert( c, tag_type() );
        }
        
        template< class Container >
        Container convert( Container*, default_type_tag ) const
        {
            return Container( this->begin(), this->end() );
        }
        
        template< class Array >
        Array convert( Array*, array_type_tag ) const
        {
            Array array;
            if( array.size() < this->size() )
                throw assignment_exception( "array initialized with too many elements" );
            std::size_t n = 0; 
            typename std::deque<T>::const_iterator i = this->begin(), end = this->end();
            for( ; i != end; ++i, ++n )
                array[n] = *i;
            for( ; n < array.size(); ++n )
                array[n] = typename Array::value_type();
            return array; 
        }

    };
    
    /*
    template< class T, class U >
    inline generic_list<T> operator+( const generic_list<T>& l, const generic_list<U>& r )
    {
        generic_list<T> res( l );
        res += r;
        return res;
    }
    */
} // namespace 'detail'

    template< class T >
    inline detail::generic_list<T> 
    list_of( T t )
    {
        return detail::generic_list<T>()( t );
    }
    
    template< class T, class U, class U2 >
    inline detail::generic_list<T> 
    list_of( U u, U2 u2 )
    {
        return detail::generic_list<T>()( u, u2 );
    }
    
    template< class T, class U, class U2, class U3 >
    inline detail::generic_list<T> 
    list_of( U u, U2 u2, U3 u3 )
    {
        return detail::generic_list<T>()( u, u2, u3 );
    }

    template< class T, class U, class U2, class U3, class U4 >
    inline detail::generic_list<T> 
    list_of( U u, U2 u2, U3 u3, U4 u4 )
    {
        return detail::generic_list<T>()( u, u2, u3, u4 );
    }
  
    template< class Key, class T >
    inline detail::generic_list< std::pair<Key,T> >
    map_list_of( Key k, T t )
    {
        return detail::generic_list< std::pair<Key,T> >()( k, t );
    }


} // namespace 'assignment'
} // namespace 'boost'

#endif
