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

#ifndef BOOST_ASSIGN_INSERT_ASSIGNER_HPP
#define BOOST_ASSIGN_INSERT_ASSIGNER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

namespace boost
{
namespace assignment
{
    template< typename C, typename V = typename C::value_type > 
    class insert_assigner
    {
    public:
        typedef V  value_type;

        explicit insert_assigner( C& c ) : c_( c )
        {}



        template< class U >
        insert_assigner& operator,( U u )
        {
            insert_( u );
            return *this;
        }


        template< class U >
        insert_assigner& operator()( U u )
        {
            insert_( u );
            return *this;
        }

 
        
        template< typename T, typename T2 >
        insert_assigner& operator()( T t, T2 t2 )
        {
            insert_( value_type( t, t2 ) );
            return *this;
        }



        template< typename T, typename T2, typename T3 >
        insert_assigner& operator()( T t, T2 t2, T3 t3 )
        {
            insert_( value_type( t, t2, t3 ) );
            return *this;
        }



        template< typename T, typename T2, typename T3, typename T4 >
        insert_assigner& operator()( T t, T2 t2, T3 t3,
                                     T4 t4 )
        {
            insert_( value_type( t, t2, t3, t4 ) );
            return *this;
        }



        template< typename T, typename T2, typename T3, typename T4,
        typename T5 >
        insert_assigner& operator()( T t, T2 t2, T3 t3,
                                     T4 t4, T5 t5 )
        {
            insert_( value_type( t, t2, t3, t4, t5 ) );
            return *this;
        }



        template< typename T, typename T2, typename T3, typename T4,
        typename T5, typename T6 >
        insert_assigner& operator()( T t, T2 t2, T3 t3,
                                     T4 t4, T5 t5, T6 t6 )
        {
            insert_( value_type( t, t2, t3, t4, t5, t6 ) );
            return *this;
        }

    private:
        
        void insert_( value_type u )
        {
            make_insertion( c_, u );
        }

        insert_assigner& operator=( const insert_assigner& );
        C& c_;
    };
    
} // namespace 'assignment'
} // namespace 'boost'

#endif
