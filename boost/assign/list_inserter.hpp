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

#ifndef BOOST_ASSIGN_LIST_INSERTER_HPP
#define BOOST_ASSIGN_LIST_INSERTER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>

namespace boost
{
namespace assignment
{
    
    template< class Fun, class Argument = void > 
    class list_inserter
    {
        struct single_arg_type {};
        struct n_arg_type      {};

        typedef typename mpl::if_c< is_void<Argument>::value,
                                    n_arg_type,
                                    single_arg_type >::type arg_type;  
            
    public:
        
        explicit list_inserter( Fun fun ) : insert_( fun )
        {}
        
        template< class Fun2, class Arg >
        list_inserter( list_inserter<Fun2,Arg> r ) 
        : insert_( r.fun_private() ) 
        {}

        template< class T >
        list_inserter& operator,( T t )
        {
            insert1( t, arg_type() );
            return *this;
        }

        template< class T >
        list_inserter& operator()( T t )
        {
            insert1( t, arg_type() );
            return *this;
        }

        template< class T, class T2 >
        list_inserter& operator()( T t, T2 t2 )
        {
            insert2( t, t2, arg_type() );
            return *this;
        }

        template< class T, class T2, class T3 >
        list_inserter& operator()( T t, T2 t2, T3 t3 )
        {
            insert3( t, t2, t3, arg_type() );
            return *this;
        }

        template< class T, class T2, class T3, class T4 >
        list_inserter& operator()( T t, T2 t2, T3 t3, T4 t4 )
        {
            insert4( t, t2, t3, t4, arg_type() );
            return *this;
        }

        template< class T, class T2, class T3, class T4, class T5 >
        list_inserter& operator()( T t, T2 t2, T3 t3, T4 t4, T5 t5 )
        {
            insert5( t, t2, t3, t4, t5, arg_type() );
            return *this;
        }

        template< class T >
        void insert1( T t, single_arg_type )
        {
            insert_( Argument( t ) );
        }

        template< class T, class T2 >
        void insert2( T t, T2 t2, single_arg_type )
        {
            insert_( Argument( t, t2 ) );
        }

        template< class T, class T2, class T3 >
        void insert3( T t, T2 t2, T3 t3, single_arg_type )
        {
            insert_( Argument( t, t2, t3 ) );
        }

        template< class T, class T2, class T3, class T4 >
        void insert4( T t, T2 t2, T3 t3, T3 t4, single_arg_type )
        {
            insert_( Argument( t, t2, t3, t4 ) );
        }

        template< class T, class T2, class T3, class T4, class T5 >
        void insert5( T t, T2 t2, T3 t3, T3 t4, T5 t5, single_arg_type )
        {
            insert_( Argument( t, t2, t3, t4, t5 ) );
        }
        
        template< class T >
        void insert1( T t, n_arg_type )
        {
            insert_( t );
        }
        
        template< class T, class T2 >
        void insert2( T t, T2 t2, n_arg_type )
        {
            insert_( t, t2 );
        }

        template< class T, class T2, class T3 >
        void insert3( T t, T2 t2, T3 t3, n_arg_type )
        {
            insert_( t, t2, t3 );
        }

        template< class T, class T2, class T3, class T4 >
        void insert4( T t, T2 t2, T3 t3, T3 t4, n_arg_type )
        {
            insert_( t, t2, t3, t4 );
        }

        template< class T, class T2, class T3, class T4, class T5 >
        void insert5( T t, T2 t2, T3 t3, T3 t4, T5 t5, n_arg_type )
        {
            insert_( t, t2, t3, t4, t5 );
        }
        
        Fun fun_private() const
        {
            return insert_;
        }

    private:
        
        list_inserter& operator=( const list_inserter& );
        Fun insert_;
    };
    
    
    
    template< class Fun >
    inline list_inserter< Fun >
    make_list_inserter( Fun fun )   
    {
        return list_inserter< Fun >( fun );
    }
    
    /*
    template< class T, class Fun >
    inline list_inserter< Fun, T >
    make_list_inserter_of( Fun fun )
    {
        return list_inserter< Fun, T >( fun );
    }
    */
    
} // namespace 'assignment'
} // namespace 'boost'

#endif
