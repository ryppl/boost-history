// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_MATRIX_TRAITS_UBLAS_HPP
#define BOOST_MATRIX_TRAITS_UBLAS_HPP

#include <boost/assert.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/size_type.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace boost
{
    namespace matrix_traits
    {
        namespace u = boost::numeric::ublas;
        
        ///////////////////////////////////////////////////////////////////////
        // typedefs
        
        template< class T, class F, class A >
        struct column_iterator_of< u::matrix<T,F,A> > 
        {
            typedef typename u::matrix_column< u::matrix<T,F,A> >::iterator type;
        };
        
        template< class T, class F, class A >
        struct const_column_iterator_of< u::matrix<T,F,A> > 
        {
            typedef typename u::matrix_column< u::matrix<T,F,A> >::const_iterator type;
        };

        template< class T, class F, class A >
        struct row_iterator_of< u::matrix<T,F,A> > 
        {
            typedef typename u::matrix_row< u::matrix<T,F,A> >::iterator type;
        };
        
        template< class T, class F, class A >
        struct const_row_iterator_of< u::matrix<T,F,A> > 
        {
            typedef typename u::matrix_row< u::matrix<T,F,A> >::const_iterator type;
        };

        ///////////////////////////////////////////////////////////////////////
        // constants
        
        template< class T, class F, class A >
        struct is_column_major< u::matrix<T,F,A> >
        {
            enum { value = false }; 
        };
        
        ///////////////////////////////////////////////////////////////////////
        // accessors, run-time
        //
        
        template< class T, class F, class A >
        typename u::matrix<T,F,A>::size_type 
        rows( const u::matrix<T,F,A>& m )
        {
            return m.size1();
        }

        template< class T, class A >
        typename u::vector<T,A>::size_type
        rows( const u::vector<T,A>& v )
        {
            return v.size();
        }
        
        template< class T, class F, class A >
        typename u::matrix<T,F,A>::size_type 
        columns( const u::matrix<T,F,A>& m )
        {
            return m.size2();
        }

        template< class T, class F, class A >
        typename row_iterator_of< u::matrix<T,F,A> >::type
        begin_row_n( u::matrix<T,F,A>& m, typename u::matrix<T,F,A>::size_type n )
        {
            BOOST_ASSERT( n < m.size1() );
            u::matrix_row< u::matrix<T,F,A> > r( m, n );
            return r.begin();
        }
        
        template< class T, class F, class A >
        typename const_row_iterator_of< const u::matrix<T,F,A> >::type
        begin_row_n( const u::matrix<T,F,A>& m, typename u::matrix<T,F,A>::size_type n )
        {
            BOOST_ASSERT( n < m.size1() );
            u::matrix_row< const u::matrix<T,F,A> > r( m, n );
            return r.begin();
        }

        template< class T, class F, class A >
        typename column_iterator_of< u::matrix<T,F,A> >::type
        begin_column_n( u::matrix<T,F,A>& m, typename u::matrix<T,F,A>::size_type n )
        {
            BOOST_ASSERT( n < m.size2() );
            u::matrix_column< u::matrix<T,F,A> > c( m, n );
            return c.begin();
        }
        
        template< class T, class F, class A >
        typename const_column_iterator_of< const u::matrix<T,F,A> >::type
        begin_column_n( const u::matrix<T,F,A>& m, typename u::matrix<T,F,A>::size_type n )
        {
            BOOST_ASSERT( n < m.size2() );
            u::matrix_column< const u::matrix<T,F,A> > c( m, n );
            return c.begin();
        }

        template< class T, class F, class A >
        typename row_iterator_of< u::matrix<T,F,A> >::type
        end_row_n( u::matrix<T,F,A>& m, typename u::matrix<T,F,A>::size_type n )
        {
            BOOST_ASSERT( n < m.size1() );
            u::matrix_row< u::matrix<T,F,A> > r( m, n );
            return r.end();
        }
        
        template< class T, class F, class A >
        typename const_row_iterator_of< const u::matrix<T,F,A> >::type
        end_row_n( const u::matrix<T,F,A>& m, typename u::matrix<T,F,A>::size_type n )
        {
            BOOST_ASSERT( n < m.size1() );
            u::matrix_row< const u::matrix<T,F,A> > r( m, n );
            return r.end();
        }
        
        template< class T, class F, class A >
        typename column_iterator_of< u::matrix<T,F,A> >::type
        end_column_n( u::matrix<T,F,A>& m, typename u::matrix<T,F,A>::size_type n )
        {
            BOOST_ASSERT( n < m.size2() );
            u::matrix_column< u::matrix<T,F,A> > c( m, n );
            return c.end();
        }
        
        template< class T, class F, class A >
        typename const_column_iterator_of< const u::matrix<T,F,A> >::type
        end_column_n( const u::matrix<T,F,A>& m, typename u::matrix<T,F,A>::size_type n )
        {
            BOOST_ASSERT( n < m.size2() );
            u::matrix_column< const u::matrix<T,F,A> > c( m, n );
            return c.end();
        }
        
        template< class T, class A >
        typename u::vector<T,A>::value_type&     
        row_value( u::vector<T,A>& v, typename u::vector<T,A>::size_type row )
        {
            BOOST_ASSERT( row < v.size() );
            return v( row );
        }
             
        template< class T, class A >
        const typename u::vector<T,A>::value_type&     
        row_value( const u::vector<T,A>& v, typename u::vector<T,A>::size_type row )
        {
            BOOST_ASSERT( row < v.size() );
            return v( row );
        }

    } // namespace 'matrix_traits'
    
} // namespace 'boost'

#endif
