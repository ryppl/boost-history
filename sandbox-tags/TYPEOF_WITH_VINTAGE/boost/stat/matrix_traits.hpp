// (C) Copyright Thorsten Ottosen 2004. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_MATRIX_TRAITS_HPP
#define BOOST_MATRIX_TRAITS_HPP

#include <boost/assert.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/size_type.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost
{
    namespace matrix_traits
    {
        ///////////////////////////////////////////////////////////////////////
        // typedefs
        
        template< class M >
        struct column_iterator_of;
        
        template< class M >
        struct const_column_iterator_of;

        template< class M >
        struct row_iterator_of;

        template< class M >
        struct const_row_iterator_of;
        
        template< class M >
        struct inverse_type_of;
        
        template< class M >
        struct transpose_type_of;
        
        template< class M >
        struct column_type_of;

        template< class M >
        struct row_type_of;
        
        template< class M >
        struct diagonal_type_of; 
        
        template< class M >
        struct eigen_value_type_of;
        
        template< class M >
        struct factorization_diagonal_type_of;
        
        template< class M >
        struct factorization_pre_orthogonal_type_of;
        
        template< class M >
        struct factorization_post_orthogonal_type_of;
        
        ///////////////////////////////////////////////////////////////////////
        // constants
        
            
        template< class M >
        struct is_column_major
        {
            // no default
        };
        
        template< class M >
        struct is_row_major
        {
            // no default
        };
        
        template< class M >
        struct is_dense
        {
            // no default
        };
        
        template< class M >
        struct is_sparse
        {
            // no default
        };
       
        template< class M >
        struct is_diagonal
        {
            enum { value = false };
        };
        
        template< class M >
        struct is_symmetric
        {
            enum { value = false };
        };
        
        template< class M >
        struct is_othogonal
        {
            enum { value = false };
        };
        
        template< class M >
        struct is_hermetian
        {
            enum { value = false };
        };

        template< class M, class V >
        struct suggested_inner_product_parallelism // for duff's device
        {
            enum { value = 2 };
        };
        
        template< class M, class V >
        struct suggested_matrix_product_parallelism // for duff's device
        {
            enum { value = 2 };
        };

        ///////////////////////////////////////////////////////////////////////
        // tags
        
        struct is_column_major_tag {};
        struct is_row_major_tag    {};
        struct is_dense_tag        {};
        struct is_sparse_tag       {};
        struct is_diagonal_tag     {};
        struct is_othogonal_tag    {};
        struct is_symmetric_tag    {};
        struct is_hermetian_tag    {};
        
        ///////////////////////////////////////////////////////////////////////
        // accessors, run-time
        //
        
        template< class M >
        typename range_size<M>::type 
        rows( const M& );

        template< class M >
        typename range_size<M>::type 
        columns( const M& );

        template< class M >
        typename row_type_of<M>::type& 
        row( M&, typename range_size<M>::type n );

        template< class M >
        const typename row_type_of<M>::type& 
        row( const M&, typename range_size<M>::type n );

        template< class M >
        typename column_type_of<M>::type& 
        column( M&, typename range_size<M>::type n );
        
        template< class M >
        const typename column_type_of<M>::type& 
        column( const M&, typename range_size<M>::type n );

        template< class M >
        inline typename row_iterator_of<M>::type
        begin_row_n( M& m, typename range_size<M>::type n );

        template< class M >
        inline typename const_row_iterator_of<M>::type
        begin_row_n( const M& m, typename range_size<M>::type n );

        template< class M >
        inline typename column_iterator_of<M>::type
        begin_column_n( M& m, typename range_size<M>::type n );

        template< class M >
        inline typename const_column_iterator_of<M>::type
        begin_column_n( const M& m, typename range_size<M>::type n );

        template< class M >
        inline typename row_iterator_of<M>::type
        end_row_n( M& m, typename range_size<M>::type n );
        
        template< class M >
        inline typename const_row_iterator_of<M>::type
        end_row_n( const M& m, typename range_size<M>::type n );
        
        template< class M >
        inline typename column_iterator_of<M>::type
        end_column_n( M& m, typename range_size<M>::type n );
        
        template< class M >
        inline typename const_column_iterator_of<M>::type
        end_column_n( const M& m, typename range_size<M>::type n );

        template< class M >
        typename range_value<M>::type& 
        row_column_value( M&, typename range_size<M>::type row, typename range_size<M>::type column );
         
        template< class M >
        const typename range_value<M>::type& 
        row_column_value( const M&, typename range_size<M>::type row, typename range_size<M>::type column );

        template< class V >
        typename range_value<V>::type&     
        row_value( V&, typename range_size<V>::type row );    
             
        template< class V >
        const typename range_value<V>::type&     
        row_value( const V&, typename range_size<V>::type row );    

        template< class M >
        typename range_value<M>::type& 
        diaginal_value( M&, typename range_size<M>::type n );
         
        template< class M >
        const typename range_value<M>::type& 
        diagonal_value( const M&, typename range_size<M>::type n );

        template< class M >
        void swap( M&, typename range_size<M>::type row1, typename range_size<M>::type column1, 
                   typename range_size<M>::type row2, typename range_size<M>::type column2 );    
            
        ///////////////////////////////////////////////////////////////////////
        // accessors, compile-time
        //
        
        template< class M >
        struct rows_of
        {
            enum { value = M::rows };
        };

        template< class M >
        struct columns_of
        {
            enum { value = M::rows };
        };

        template< size_t n, class M >
        typename row_iterator_of<M>::type
        begin_row( M& ); // use: begin_row<0>( M );
        
        template< size_t n, class M >
        typename const_row_iterator_of<M>::type
        begin_row( const M& );

        template< size_t n, class M >
        typename column_iterator_of<M>::type
        begin_column( M& );

        template< size_t n, class M >
        typename const_column_iterator_of<M>::type
        begin_column( const M& );

        template< size_t n, class M >
        typename row_iterator_of<M>::type
        end_row( M& ); // use: end_row<0>( M );
        
        template< size_t n, class M >
        typename const_row_iterator_of<M>::type
        end_row( const M& );
        
        template< size_t n, class M >
        typename column_iterator_of<M>::type
        end_column( M& );
        
        template< size_t n, class M >
        typename const_column_iterator_of<M>::type
        end_column( const M& );

        template< size_t r, size_t c, class M >
        typename range_value<M>::type& 
        row_column_value_at( M& ); // use: row_column_value_at<0,1>( M );
         
        template< size_t r, size_t c, class M >
        const typename range_value<M>::type& 
        row_column_value_at( const M& );

        template< size_t n, class M >
        typename range_value<M>::type& 
        diaginal_value_at( M& );
         
        template< size_t n, class M >
        const typename range_value<M>::type& 
        diagonal_value_at( const M& );
        
        template< size_t r1, size_t c1, size_t r2, size_t c2, class M >
        void swap_at( M& ); // use: swap_at<1,2,2,1>( m );   

        ///////////////////////////////////////////////////////////////////////
        // algorithms -- these should be implemented using only the
        //               matrix traits
        //
        
        template< class M >
        size_t rank( const M& );
        
        template< class T, class M >
        T trace( const M& );
        
        template< class T, class M >
        T determinant( const M& m );
        
        template< class M >
        typename transpose_type_of<M>::type transpose( const M& );
        
        template< class M >
        bool has_inverse( const M& ); //?

        template< class M >
        bool is_orthogonal( const M& );
        
        template< class M >
        void inverse( M& );

        template< class M >
        typename inverse_type_of<M>::type inverse( const M& );

        template< class M >
        void transpose( M& );

        template< class M >
        void orthonormal( M& );
        
        template< class M >
        M orthonormal( const M& );
        
        template< class M >
        void sqrt( M& );
        
        template< class M >
        M sqrt( const M& );
        
        template< class M >
        void pow( M&, int x );

        template< class M >
        M pow( const M&, int x );

        template< class M >
        void pow( M&, double x );

        template< class M >
        M pow( const M&, double x );
/*
        template< class M >
        typename eigen_range_value<M>::type eigen_values( const M& );
        
        template< class M >
        void lu_factorization( const M&, 
                               typename factorization_pre_orthogonal_type_of<M>::type&,
                               typename factorization_diagonal_type_of<M>::type&,
                               typename factorization_post_orthogonal_type_of<M>::type& );
        
        template< class M >
        void spectral_factorization( const M&,
                                     typename factorization_pre_orthogonal_type_of<M>::type&,
                                     typename factorization_diagonal_type_of<M>::type&,
                                     typename factorization_post_orthogonal_type_of<M>::type& );
        // mutable versions? Save mem?
*/        
    } // namespace 'matrix_traits'
    
} // namespace 'boost'

#include <boost/stat/matrix_traits_ublas.hpp>

#endif
