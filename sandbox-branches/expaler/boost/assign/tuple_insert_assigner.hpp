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

#ifndef BOOST_ASSIGN_TUPLE_INSERT_ASSIGNER_HPP
#define BOOST_ASSIGN_TUPLE_INSERT_ASSIGNER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
    #pragma once
#endif

#include <boost/call_traits.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/assign/make_insertion.hpp>

namespace boost
{
namespace assignment
{
    template< typename C > 
    class tuple_insert_assigner
    {
    public:
        explicit tuple_insert_assigner( C& c ) : c_( c )
        {}


        
        template< typename T >
        tuple_insert_assigner& operator()( const T& v )
        {
            make_tuple_insertion( c_, v );
            return *this;
        }

        

        template< typename T, typename T2 >
        tuple_insert_assigner& operator()( const T& t, const T2& t2 )
        {
            make_tuple_insertion( c_, t, t2 );
            return *this;
        }



        template< typename T, typename T2, typename T3 >
        tuple_insert_assigner& operator()( const T& t, const T2& t2, const T3& t3 )
        {
            make_tuple_insertion( c_, t, t2, t3 );
            return *this;
        }



        template< typename T, typename T2, typename T3, typename T4 >
        tuple_insert_assigner& operator()( const T& t, const T2& t2, const T3& t3,
                                           const T4& t4 )
        {
            insert_tuple_insertion( c_, t, t2, t3, t4 );
            return *this;
        }



        template< typename T, typename T2, typename T3, typename T4,
                  typename T5 >
        tuple_insert_assigner& operator()( const T& t, const T2& t2, const T3& t3,
                                           const T4& t4, const T5& t5 )
        {
            make_tuple_insertion( c_, t, t2, t3, t4, t5 );
            return *this;
        }



        template< typename T, typename T2, typename T3, typename T4,
                  typename T5, typename T6 >
        tuple_insert_assigner& operator()( const T& t, const T2& t2, const T3& t3,
                                           const T4& t4, const T5& t5, const T6& t6 )
        {
            make_tuple_insertion( c_, t, t2, t3, t4, t5, t6 );
            return *this;
        }

    private:

        tuple_insert_assigner& operator=( const tuple_insert_assigner& );
        C& c_;
    };

} // namespace 'assignment'
} // namespace 'boost'

#endif
