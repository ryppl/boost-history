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

#ifndef BOOST_ASSIGN_FIXED_SIZE_TUPLE_ASSIGNER_HPP
#define BOOST_ASSIGN_FIXED_SIZE_TUPLE_ASSIGNER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
    #pragma once
#endif

#include <boost/config.hpp>

#ifdef BOOST_NO_EXCEPTIONS
    #include <cassert>
#else
    #include <boost/assign/exception.hpp>
#endif

#include <boost/tuple/tuple.hpp>

namespace boost
{
namespace assignment
{
    template< typename InputIterator > 
    class fixed_size_tuple_assigner
    {
    public:
        typedef InputIterator  iterator;

        fixed_size_tuple_assigner( const iterator& begin, const iterator& end ) 
        : iter_( begin ), end_( end ), begin_( begin ) 
        {}



        ~fixed_size_tuple_assigner()
        {
            if( has_assignment_commenced() )
#ifdef BOOST_NO_EXCEPTIONS
                if( iter_ != end_ )
                    assert( false && "Container assigned with too few elements" );
#else
                if( iter_ != end_ && !std::uncaught_exception() )
                    throw assignment_exception( " Exception: container"
                                                " assigned with too"
                                                " few elements! " );
#endif
        }

        template< typename T >
        fixed_size_tuple_assigner& operator()( const T& v )
        {
            check_bounds<1>();
            make_tuple_insertion( iter_, v );
            return *this;
        }



        template< typename T, typename T2 >
        fixed_size_tuple_assigner& operator()( const T& t, const T2& t2 )
        {
            check_bounds<2>();
            make_tuple_insertion( iter_, t, t2 );
            return *this;
        }



        template< typename T, typename T2, typename T3 >
        fixed_size_tuple_assigner& operator()( const T& t, const T2& t2, const T3& t3 )
        {
            check_bounds<3>();
            make_tuple_insertion( iter_, t, t2, t3 );
            return *this;
        }



        template< typename T, typename T2, typename T3, typename T4 >
        fixed_size_tuple_assigner& operator()( const T& t, const T2& t2, const T3& t3,
                                               const T4& t4 )
        {
            check_bounds<4>();
            make_tuple_insertion( iter_, t, t2, t3, t4 );
            return *this;
        }



        template< typename T, typename T2, typename T3, typename T4,
        typename T5 >
        fixed_size_tuple_assigner& operator()( const T& t, const T2& t2, const T3& t3,
                                               const T4& t4, const T5& t5 )
        {
            check_bounds<5>();
            make_tuple_insertion( iter_, t, t2, t3, t4, t5 );
            return *this;
        }



        template< typename T, typename T2, typename T3, typename T4,
        typename T5, typename T6 >
        fixed_size_tuple_assigner& operator()( const T& t, const T2& t2, const T3& t3,
                                               const T4& t4, const T5& t5, const T6& t6 )
        {
            check_bounds<6>();
            make_tuple_insertion( iter_, t, t2, t3, t4, t5, t6 );
            return *this;
        }

    private:
        fixed_size_tuple_assigner& operator=( const fixed_size_tuple_assigner& );

        
        template< int delta >
        void check_bounds()
        {
            if( iter_ + delta > end_ )
            {
#ifdef BOOST_NO_EXCEPTIONS
                assert( false && "Trying to write past the end of the container" );
#else
                throw assignment_exception( " Exception: trying to write"
                                            " past the end of the container! " );
#endif
            }
        }

        
        
        bool has_assignment_commenced()
        {
            return iter_ != begin_;
        }

        iterator        iter_;
        const iterator  end_;
        const iterator  begin_;

    };

} // namespace 'assignment'
} // namespace 'boost'

#endif
