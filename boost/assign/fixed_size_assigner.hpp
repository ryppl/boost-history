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

#ifndef BOOST_ASSIGN_FIXED_SIZE_ASSIGNER_HPP
#define BOOST_ASSIGN_FIXED_SIZE_ASSIGNER_HPP

//#include "assign_traits.hpp"
#include "exception.hpp"

namespace boost
{
namespace assignment
{   
    template< typename ValueType, typename InputIterator >
    class fixed_size_assigner
    {
    public:
        typedef ValueType      value_type;
        typedef InputIterator  iterator;

        fixed_size_assigner( const iterator& begin, const iterator& end )
        : iter_( begin ), end_( end ), begin_( begin ) 
        {}



        fixed_size_assigner( const iterator& begin, const iterator& end,
                             const value_type& v ) 
        : iter_( begin ), end_( end ), begin_( begin ) 
        {
            insert_( v );
        }



        fixed_size_assigner( const fixed_size_assigner& r ) 
        : iter_( r.iter_ ), end_( r.end_ ), begin_( r.begin_ )
        {}



        ~fixed_size_assigner()
        {
            if( has_assignment_commenced() )
                if( iter_ != end_ && !std::uncaught_exception() )
                    throw assignment_exception( " Exception: container"
                                                " initialized with too"
                                                " few elements! " );
        }



        fixed_size_assigner& operator,( const value_type& v )
        {
            insert_( v );
            return *this;
        }



        fixed_size_assigner& operator()( const value_type& v )
        {
            insert_( v );
            return *this;
        }



        template< typename T, typename T2 >
        insert_assigner& operator()( const T& t, const T2& t2 )
        {
            insert_( value_type( t, t2 ) );
            return *this;
        }



        template< typename T, typename T2, typename T3 >
        fixed_size_assigner& operator()( const T& t, const T2& t2, const T3& t3 )
        {
            insert_( value_type( t, t2, t3 ) );
            return *this;
        }



        template< typename T, typename T2, typename T3, typename T4 >
        fixed_size_assigner& operator()( const T& t, const T2& t2, const T3& t3,
                                         const T4& t4 )
        {
            insert_( value_type( t, t2, t3, t4 ) );
            return *this;
        }



        template< typename T, typename T2, typename T3, typename T4,
        typename T5 >
        fixed_size_assigner& operator()( const T& t, const T2& t2, const T3& t3,
                                         const T4& t4, const T5& t5 )
        {
            insert_( value_type( t, t2, t3, t4, t5 ) );
            return *this;
        }



        template< typename T, typename T2, typename T3, typename T4,
        typename T5, typename T6 >
        fixed_size_assigner& operator()( const T& t, const T2& t2, const T3& t3,
                                         const T4& t4, const T5& t5, const T6& t6 )
        {
            insert_( value_type( t, t2, t3, t4, t5, t6 ) );
            return *this;
        }


    private:

        fixed_size_assigner& operator=( const fixed_size_assigner& );

        void insert_( const value_type& v )
        {
            if( iter_ == end_ )
                throw assignment_exception( " Exception: trying to write"
                                            " past the end of the container! " );
            *iter_ = v;
            ++iter_;        
        }



        bool has_assignment_commenced()
        {
            return iter_ != begin_;
        }

        iterator        iter_;
        const iterator  end_;
        const iterator  begin_;
    };

    //////////////////////////////////////////////////////////////////////////
    // default templates
    //////////////////////////////////////////////////////////////////////////

    template< typename C >
    inline fixed_size_assigner<typename C::value_type, typename C::iterator>
    assign_all( C& c )
    {
        return fixed_size_assigner<typename C::value_type, typename C::iterator>
        ( c.begin(), c.end() );
    }



    template< typename C >
    inline fixed_size_assigner<typename C::value_type, typename C::iterator>
    operator<<( C& c, typename C::value_type v )
    {
        return fixed_size_assigner<typename C::value_type,typename C::iterator>
        ( c.begin(), c.end(), v ); 
    }

} // namespace 'assignment'
} // namespace 'boost'

#endif
