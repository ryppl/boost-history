// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_MATRIX_TRAITS_VECTOR_HPP
#define BOOST_MATRIX_TRAITS_VECTOR_HPP

#include <boost/stat/matrix_traits.hpp>
#include <vector>

namespace boost
{
    namespace matrix_traits
    {
        ///////////////////////////////////////////////////////////////////////
        // typedefs
        
        template< class T >
        struct column_iterator_of< std::vector< std::vector< T > > > 
        {
            typedef void type;
        };
       
        template< class T >
        struct const_column_iterator_of< std::vector< std::vector< T > > > 
        {
            typedef void type;
        };
                
        
        template< class T >
        struct row_iterator_of< std::vector< std::vector< T > > > 
        {
            typedef typename iterator_of< std::vector<T> >::type type;
        };
        
        
        template< class T >
        struct const_row_iterator_of< std::vector< std::vector< T > > > 
        {
            typedef typename const_iterator_of< std::vector<T> >::type type;
        };

        template< class T >
        struct row_type_of< std::vector< std::vector< T > > >
        {
            typedef typename std::vector<T> type;
        };
        
        template< class T >
        struct column_type_of< std::vector< std::vector< T > > >
        {
            typedef void type;
        };
        
        ///////////////////////////////////////////////////////////////////////
        // constants
        
        template< class T >
        struct is_column_major< std::vector< std::vector< T > > >
        {
            enum { value = false }; 
        };
        
        template< class T >
        struct is_row_major< std::vector< std::vector< T > > >
        {
            enum { value = true }; 
        };
        
        ///////////////////////////////////////////////////////////////////////
        // accessors, run-time
        //
        
        template< class T >
        inline typename std::vector<T>::size_type 
        rows( const std::vector< std::vector< T > >& m )
        {
            return m.size();
        }

        template< class T >
        inline typename std::vector<T>::size_type 
        columns( const std::vector< std::vector< T > >& m )
        {
            return m.at( 0 ).size();
        }

        template< class T >
        inline std::vector<T>& 
        row( std::vector< std::vector< T > >& m, typename std::vector<T>::size_type n )
        {
            return m.at( n );
        }
        
        template< class T >
        inline const std::vector<T>& 
        row( const std::vector< std::vector< T > >& m, typename std::vector<T>::size_type n )
        {
            return m.at( n );
        }

        /////////////////////////////////////////////7
        //
        
        // only defined for row-major
        template< class M >
        inline typename row_iterator_of<M>::type
        begin_row_n( M& m, typename size_type_of<M>::type n )
        {
            return begin( row( m, n ) );
        }
        
        template< class M >
        inline typename const_row_iterator_of<M>::type
        begin_row_n( const M& m, typename size_type_of<M>::type n )
        {
            return begin( row( m, n ) );
        }

        // only defined for column-major
        template< class M >
        inline typename column_iterator_of<M>::type
        begin_column_n( M& m, typename size_type_of<M>::type n )
        {
            return begin( column( m, n ) );
        }

        template< class M >
        inline typename const_column_iterator_of<M>::type
        begin_column_n( const M& m, typename size_type_of<M>::type n )
        {
            return begin( column( m, n ) );
        }

        // only defined for row-major
        template< class M >
        inline typename row_iterator_of<M>::type
        end_row_n( M& m, typename size_type_of<M>::type n )
        {
            return end( row( m, n ) );
        }
        
        template< class M >
        inline typename const_row_iterator_of<M>::type
        end_row_n( const M& m, typename size_type_of<M>::type n )
        {
            return end( row( m, n ) );
        }

        // only defined for column-major
        template< class M >
        inline typename column_iterator_of<M>::type
        end_column_n( M& m, typename size_type_of<M>::type n )
        {
            return end( column( m, n ) );
        }

        template< class M >
        inline typename const_column_iterator_of<M>::type
        end_column_n( const M& m, typename size_type_of<M>::type n )
        {
            return end( column( m, n ) );
        }

    
    } // namespace 'matrix_traits'
    
} // namespace 'boost'

#endif
