// Boost.Assign library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/assign/
//

#ifndef BOOST_ASSIGN_LIST_INSERTER_HPP
#define BOOST_ASSIGN_LIST_INSERTER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_void.hpp>

namespace boost
{
namespace assign_detail
{
    template< class T >
    struct repeater
    {
        std::size_t  sz;
        T            val;

        repeater( std::size_t sz, T r ) : sz( sz ), val( r )
        { }
    };
    
    template< class Fun >
    struct fun_repeater
    {
        std::size_t  sz;
        Fun          val;
        fun_repeater( std::size_t sz, Fun r ) : sz( sz ), val( r )
        { }
    };
    
    /*
    template< class T >
    struct from_to_repeater
    {
        T from, to;
        from_to_repeater( T from, T to ) : from( from ), to( to )
        { }
    };
    
    template< class T >
    struct from_to_increment_repeater
    {
        T from, to, incr;
        from_to_increment_repeater( T from, T to, T incr )
         : from( from ), to( to ), incr( incr )
        { }
    };
    */

    template< class C >
    class call_push_back
    {
        C& c_;
    public:
        call_push_back( C& c ) : c_( c )
        { }
        
        template< class T >
        void operator()( T r ) 
        {
            c_.push_back( r );
        }
    };
    
    template< class C >
    class call_push_front
    {
        C& c_;
    public:
        call_push_front( C& c ) : c_( c )
        { }
        
        template< class T >
        void operator()( T r ) 
        {
            c_.push_front( r );
        }
    };
    
    template< class C >
    class call_push
    {
        C& c_;
    public:
        call_push( C& c ) : c_( c )
        { }
    
        template< class T >
        void operator()( T r ) 
        {
            c_.push( r );
        }
    };
    
    template< class C >
    class call_insert
    {
        C& c_;
    public:
        call_insert( C& c ) : c_( c )
        { }
    
        template< class T >
        void operator()( T r ) 
        {
            c_.insert( r );
        }
    };

} // namespace 'assign_detail'

namespace assign
{

    template< class T >
    inline assign_detail::repeater<T>
    repeat( std::size_t sz, T r )
    {
        return assign_detail::repeater<T>( sz, r );
    }
    
    template< class Function >
    inline assign_detail::fun_repeater<Function>
    repeat_fun( std::size_t sz, Function r )
    {
        return assign_detail::fun_repeater<Function>( sz, r );
    }
    
    /*
    template< class T >
    inline detail::from_to_repeater<T>
    repeat_from_to( T from, T to )
    {
        return detail::from_to_repeater<T>( from, to );
    }

    template< class T >
    inline detail::from_to_increment_repeater<T>
    repeat_from_to( T from, T to, T increment )
    {
        return detail::from_to_increment_repeater<T>( from, to, increment );
    }
    */
    
    
    
    template< class Function, class Argument = void > 
    class list_inserter
    {
        struct single_arg_type {};
        struct n_arg_type      {};

        typedef typename mpl::if_c< is_void<Argument>::value,
                                    n_arg_type,
                                    single_arg_type >::type arg_type;  
            
    public:
        
        explicit list_inserter( Function fun ) : insert_( fun )
        {}
        
        template< class Function2, class Arg >
        list_inserter( list_inserter<Function2,Arg> r ) 
        : insert_( r.fun_private() ) 
        {}

        list_inserter& operator()()
        {
            insert_( Argument() );
            return *this;
        }
        
        template< class T >
        list_inserter& operator=( T r )
        {
            insert_( r );
            return *this;
        }
        
        template< template <class> class Repeater, class T >
        list_inserter& operator=( Repeater<T> r )
        {
            return operator,( r );
        }
        
        template< class T >
        list_inserter& operator,( T r )
        {
            insert_( r  );
            return *this;
        }

        template< class T >
        list_inserter& operator,( assign_detail::repeater<T> r )
        {
            return repeat( r.sz, r.val ); 
        }
        
        template< class Nullary_function >
        list_inserter& operator,( assign_detail::fun_repeater<Nullary_function> r )
        {
            return repeat_fun( r.sz, r.val ); 
        }

        /*
        template< class T >
        list_inserter& operator,( detail::from_to_repeater<T> r )
        {
            return repeat_from_to( r.from, r.to ); 
        }

        template< class T >
        list_inserter& operator,( detail::from_to_increment_repeater<T> r )
        {
            return repeat_from_to( r.from, r.to, r.incr ); 
        }
        */

        template< class T >
        list_inserter& repeat( std::size_t sz, T r )
        {
            std::size_t i = 0;
            while( i++ != sz )
                insert_( r );
            return *this;
        }
        
        template< class Nullary_function >
        list_inserter& repeat_fun( std::size_t sz, Nullary_function fun )
        {
            std::size_t i = 0;
            while( i++ != sz )
                insert_( fun() );
            return *this;
        }
        
        /*
        template< class T >
        list_inserter& repeat_from_to( T from, T to )
        {
            while( from < to )
            {
                insert_( from );
                ++from;
            }
            return *this;
        }
        
        template< class T >
        list_inserter& repeat_from_to( T from, T to, T increment )
        {
            while( from < to )
            {
                insert_( from );
                from += increment;
            }
            return *this;
        }
        */

        template< class T >
        list_inserter& operator()( T t )
        {
            insert_( t );
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
        void insert4( T t, T2 t2, T3 t3, T4 t4, single_arg_type )
        {
            insert_( Argument( t, t2, t3, t4 ) );
        }

        template< class T, class T2, class T3, class T4, class T5 >
        void insert5( T t, T2 t2, T3 t3, T4 t4, T5 t5, single_arg_type )
        {
            insert_( Argument( t, t2, t3, t4, t5 ) );
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
        void insert4( T t, T2 t2, T3 t3, T4 t4, n_arg_type )
        {
            insert_( t, t2, t3, t4 );
        }

        template< class T, class T2, class T3, class T4, class T5 >
        void insert5( T t, T2 t2, T3 t3, T4 t4, T5 t5, n_arg_type )
        {
            insert_( t, t2, t3, t4, t5 );
        }
        
        Function fun_private() const
        {
            return insert_;
        }

    private:
        
        list_inserter& operator=( const list_inserter& );
        Function insert_;
    };
    
    template< class Function >
    inline list_inserter< Function >
    make_list_inserter( Function fun )
    {
        return list_inserter< Function >( fun );
    }
    
    template< class C >
    inline list_inserter< assign_detail::call_push_back<C>, 
                          BOOST_DEDUCED_TYPENAME C::value_type >
    push_back( C& c )
    {
        return make_list_inserter( assign_detail::call_push_back<C>( c ) );
    }
    
    template< class C >
    inline list_inserter< assign_detail::call_push_front<C>,
                          BOOST_DEDUCED_TYPENAME C::value_type >
    push_front( C& c )
    {
        return make_list_inserter( assign_detail::call_push_front<C>( c ) );
    }

    template< class C >
    inline list_inserter< assign_detail::call_insert<C>, 
                          BOOST_DEDUCED_TYPENAME C::value_type >
    insert( C& c )
    {
        return make_list_inserter( assign_detail::call_insert<C>( c ) );
    }

    template< class C >
    inline list_inserter< assign_detail::call_push<C>, 
                          BOOST_DEDUCED_TYPENAME C::value_type >
    push( C& c )
    {
        return make_list_inserter( assign_detail::call_push<C>( c ) );
    }
    
} // namespace 'assign'
} // namespace 'boost'

#endif
