//
// short: Test cases for container algortihms.
//
//    

// (C) Copyright Thorsten Ottosen 2002-2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.


#include "boost/sequence_algo/container_algo.hpp"

#include <functional>
#include <cassert>
#include <list>
#include <iostream>
#include <string>
#include <vector>

namespace 
{
    //
    // Data types:
    //
    // - pair
    // - STL container
    // - array    
    //

    //
    // Test for:
    //
    // - ambiguity with std algorithms
    // - same result as std algorithms
    // - ability to handle data types
    //    

    void test_for_each();
    void test_find();
    void test_find_if();
    void test_adjacent_find();
    void test_find_first_of();
    void test_count();
    void test_count_if();
    void test_mismatch();
    void test_equal();
    void test_search();
    void test_search_n();
    void test_find_end();

    void test_copy();
    void test_copy_backward();
    void test_swap_ranges();
    void test_transform();
    void test_replace();
    void test_replace_if();
    void test_replace_copy();
    void test_replace_copy_if();
    void test_fill();
    void test_fill_n();
    void test_generate();
    void test_generate_n();
    void test_remove();
    void test_remove_if();
    void test_remove_copy();
    void test_remove_copy_if();
    void test_unique();
    void test_unique_copy();
    void test_reverse();
    void test_reverse_copy();
    void test_rotate();
    void test_rotate_copy();
    void test_random_shuffle(); 

    void test_sort();
    void test_stable_sort();
    void test_partial_sort();
    void test_partial_sort_copy();
    void test_nth_element();
    void test_lower_bound();
    void test_upper_bound();
    void test_equal_range();
    void test_binary_search();
    void test_merge();
    void test_implace_merge();
    void test_partition();
    void test_stable_partition();

    void test_includes();
    void test_set_union();
    void test_set_intersection();
    void test_set_difference();
    void test_set_symmetric_difference();

    void test_push_heap();
    void test_pop_heap();
    void test_make_heap();
    void test_sort_heap();

    void test_min_eleement();
    void test_max_element();
    void test_lexicographical_compare();

    void test_next_permutation();
    void test_prev_permutation();

    void test_accumulate();
    void test_inner_product();
    void test_partial_sum();
    void test_adjacent_difference();

    void test_iota();
    void test_contains();
    void test_all();
    void test_none();
    void test_any_if();
    void test_is_sorted();

} // namespace ''


int main()
{
    
    test_for_each();
    test_find();
    test_find_if();
    test_adjacent_find();
    test_find_first_of();
    test_count();
    test_count_if();
    test_mismatch();
    test_equal();
    test_search();
    test_search_n();
    test_find_end();

    test_copy();
    test_copy_backward();
    test_swap_ranges();
    test_transform();
    test_replace();
    test_replace_if();
    test_replace_copy();
    test_replace_copy_if();
    test_fill();
    test_fill_n();
    test_generate();
    test_generate_n();
    test_remove();
    test_remove_if();
    test_remove_copy();
    test_remove_copy_if();
    test_unique();
    test_unique_copy();
    test_reverse();
    test_reverse_copy();
    test_rotate();
    test_rotate_copy();
    test_random_shuffle(); 
 
    test_sort();
    test_stable_sort();
    test_partial_sort();
    test_partial_sort_copy();
    test_nth_element();
    test_lower_bound();
    test_upper_bound();
    test_equal_range();
    test_binary_search();
    test_merge();
    test_implace_merge();
    test_partition();
    test_stable_partition();

    test_includes();
    test_set_union();
    test_set_intersection();
    test_set_difference();
    test_set_symmetric_difference();

    test_push_heap();
    test_pop_heap();
    test_make_heap();
    test_sort_heap();

    test_min_eleement();
    test_max_element();
    test_lexicographical_compare();

    test_next_permutation();
    test_prev_permutation();

    test_accumulate();
    test_inner_product();
    test_partial_sum();
    test_adjacent_difference();

    /*
    test_iota();
    test_contains();
    test_all();
    test_none();
    test_any_if();
    test_is_sorted();
    */
    return 0;

} // 'main()'



namespace 
{

    ///////////////////////////////////////////////////////////////////////////
    // Implementation
    ///////////////////////////////////////////////////////////////////////////

    //
    // enable ambiguity conflicts
    //
    using namespace std;
    using namespace boost;

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#define TEST_DATA                                                            \
        int                 a[] = { 1, 2, 4, 2, 6, 7, 9, 3, 2, 5, 5, 3, 3 }; \
        const int           ca[] = { 1, 2, 3, 5, 7, 9, 11 };                 \
        const int           a_size = sizeof( a ) / sizeof( a[0] );           \
        const int           ca_size = sizeof( ca ) / sizeof( ca[0] );        \
        pair<int*, int*>    p( a, a + a_size );                              \
        const pair<const int*, const int*>                                   \
                            cp( ca, ca + ca_size );                          \
        vector<int>         v( a, a + a_size );                              \
        const vector<int>   cv( v );                                         \
        int*                                 aiter;                          \
        const int*                           caiter;                         \
        typedef vector<int>::iterator        iterator;                       \
        typedef vector<int>::const_iterator  const_iterator;                 \
        iterator iter; const_iterator citer;                                 \
        ostream_iterator<int> out( std::cout ); 

#else

#define TEST_DATA                                                             \
        int                 a_[] = { 1, 2, 4, 2, 6, 7, 9, 3, 2, 5, 5, 3, 3 }; \
        const int           ca_[] = { 1, 2, 3, 5, 7, 9, 11 };                 \
        const int           a_size = sizeof( a_ ) / sizeof( a_[0] );          \
        const int           ca_size = sizeof( ca_ ) / sizeof( ca_[0] );       \
        pair<int*, int*>    p( a_, a_ + a_size );                             \
        const pair<const int*, const int*>                                    \
                            cp( ca_, ca_ + ca_size );                        \
        vector<int>         v( a_, a_ + a_size );                            \
        const vector<int>   cv( v );                                         \
        vector<int>         a( v );                                          \
        const vector<int>   ca( v );                                         \
        typedef vector<int>::iterator        iterator;                       \
        typedef vector<int>::const_iterator  const_iterator;                 \
        iterator iter, aiter; const_iterator citer, caiter;                  \
        ostream_iterator<int> out( std::cout ); 


#endif //  BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#define ALL( C ) begin( (C) ), end( (C) )


    struct unary_fun;
    struct binary_fun;
    struct predicate;
    struct bin_predicate;
    struct generator;
    struct print;
    template< typename Container1, typename Container2> 
    void   print2( const Container1&, const Container2& ); 

    struct unary_fun : public unary_function<int,int>
    {
	int n_;

	unary_fun( int n = 100000 ) : n_( n ) 
	{ }

        int operator()( int i )
        {
            return ++i % n_;
        }
    };

    struct binary_fun : public binary_function<int,int,int>
    {
        int operator()( int x, int y )
        {
            return x + y ;
        }
    };

    struct predicate : public unary_function<int,bool>
    {
        bool operator()( int i )
        {
            return i == 5;
        }
    };

    struct bin_predicate : public binary_function<int,int,bool>
    {
        bool operator()( int x, int y )
        {
            return x  > y ;
        }
    };

    struct generator : public unary_function<void, int>
    {
        int operator()()
        {
            return 5;
        }
    };

    struct print : public unary_function<int, void>
    {
        void operator()( int i )
        {
            cout << i <<" ";
        }
    };

    template< typename Container1, typename Container2> 
    void print2( const Container1& c1, const Container2& c2 )
    {
        cout << endl;
        for_each( c1, print() );
        cout << endl;
        for_each( c2, print() );
        cout << endl;
    }

    /////////////////////////////////////////////////////////////////////////
    // Nonmodifying Sequence Operations
    /////////////////////////////////////////////////////////////////////////


    void test_for_each()
    {
        TEST_DATA;
	for_each( p, unary_fun() );  
        for_each( cp, unary_fun() );  
        for_each( a, unary_fun() );
        for_each( ca, unary_fun() );
        for_each( v, unary_fun() );
        for_each( cv, unary_fun() );
    }



    void test_find()
    {
        TEST_DATA;
        const int i = 5;
        aiter  = find( p, i ),  assert( aiter == find( ALL( p ), i ) );
        caiter = find( cp, i ), assert( caiter == find( ALL( cp ), i ) );
        aiter  = find( a, i ),  assert( aiter == find( ALL( a ), i ) );
        caiter = find( ca, i ), assert( caiter == find( ALL( ca ), i ) );
        iter   = find( v, i ),  assert( iter == find( ALL( v ), i ) );
        citer  = find( cv, i ), assert( citer == find( ALL( cv ), i ) );
    }



    void test_find_if()
    {
        TEST_DATA;
        aiter  = find_if( p, predicate() );
        assert( aiter == find_if( ALL( p ), predicate() ) );
        caiter = find_if( cp, predicate() );
        assert( caiter == find_if( ALL( cp ), predicate() ) );
        aiter  = find_if( a, predicate() );
        assert( aiter == find_if( ALL( a ), predicate() ) );
        caiter = find_if( ca, predicate() );
        assert( caiter == find_if( ALL( ca ), predicate() ) );
        iter   = find_if( v, predicate() );
        assert( iter == find_if( ALL( v ), predicate() ) );
        citer  = find_if( cv, predicate() );
        assert( citer == find_if( ALL( cv ), predicate() ) );
    }



    void test_adjacent_find()
    {
        TEST_DATA;
        aiter  = ext::adjacent_find( p );
        assert( aiter == adjacent_find( ALL( p ) ) );
        caiter = ext::adjacent_find( cp );
        assert( caiter == adjacent_find( ALL( cp ) ) );
        aiter  = ext::adjacent_find( a );
        assert( aiter == adjacent_find( ALL( a ) ) );
        caiter = ext::adjacent_find( ca );
        assert( caiter == adjacent_find( ALL( ca ) ) );
        iter   = ext::adjacent_find( v );
        assert( iter == adjacent_find( ALL( v ) ) );
        citer  = ext::adjacent_find( cv );
        assert( citer == adjacent_find( ALL( cv ) ) );

        aiter  = ext::adjacent_find( p, bin_predicate() );
        assert( aiter == adjacent_find( ALL( p ), bin_predicate() ) );
        caiter = ext::adjacent_find( cp, bin_predicate() );
        assert( caiter == adjacent_find( ALL( cp ), bin_predicate() ) );
        aiter  = ext::adjacent_find( a, bin_predicate() );
        assert( aiter == adjacent_find( ALL( a ), bin_predicate() ) );
        caiter = ext::adjacent_find( ca, bin_predicate() );
        assert( caiter == adjacent_find( ALL( ca ), bin_predicate() ) );       
        iter   = ext::adjacent_find( v, bin_predicate() );
        assert( iter == adjacent_find( ALL( v ), bin_predicate() ) );
        citer  = ext::adjacent_find( cv, bin_predicate() );
        assert( citer == adjacent_find( ALL( cv ), bin_predicate() ) );      
    }



    void test_find_first_of()
    {
        TEST_DATA;
        vector<int> seeking( 2 );
	seeking.push_back( 6 ); seeking.push_back( 5 );

        aiter  = find_first_of( p, seeking );
        caiter = find_first_of( cp, seeking );
        aiter  = find_first_of( a, seeking );
        caiter = find_first_of( ca, seeking );
        iter   = find_first_of( v, seeking  );
        citer  = find_first_of( cv, seeking );
        assert( citer == find_first_of( cv.begin(), cv.end(), 
					seeking.begin(), seeking.end() ) );

        aiter  = find_first_of( p, seeking, bin_predicate() );
        caiter = find_first_of( cp, seeking, bin_predicate() );
        aiter  = find_first_of( a, seeking, bin_predicate() );
        caiter = find_first_of( ca, seeking, bin_predicate() );
        iter   = find_first_of( v, seeking, bin_predicate() );
        citer  = find_first_of( cv, seeking, bin_predicate() );
        assert( citer == find_first_of( cv.begin(), cv.end(), seeking.begin(), 
                                        seeking.end(), bin_predicate() ) );    
    }



    void test_count()
    {
        TEST_DATA;
        const int val = 5;
        size_t ac;
        ac = count( p, val );
        ac = count( cp, val );
        ac = count( a, val );
        ac = count( ca, val );
        vector<int>::difference_type cc;
        cc = count( v, val );
        cc = count( cv, val );
        assert( cc == count( cv.begin(), cv.end(), val ) );
    }



    void test_count_if()
    {
        TEST_DATA;
        size_t ac;
        ac = count_if( p, predicate() );
        ac = count_if( cp, predicate() );
        ac = count_if( a, predicate() );
        ac = count_if( ca, predicate() );
        vector<int>::difference_type cc;
        cc = count_if( v, predicate() );
        cc = count_if( cv, predicate() );
        assert( cc == count_if( cv.begin(), cv.end(), predicate() ) );
    }



    void test_mismatch()
    {
#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
        TEST_DATA;
        int seeking[] = { 1, 2, 3};
        pair<int*, int*>            pp  = ext::mismatch( p, seeking );
        pair<const int*, int*>      pcp = ext::mismatch( cp, seeking );
        pair<int*, int*>            pa  = ext::mismatch( a, seeking );
        pair<const int*, int*>      pca = ext::mismatch( ca, seeking );
        pair<iterator, int*>        pv  = ext::mismatch( v, seeking );
        pair<const_iterator, int*>  pcv = ext::mismatch( cv, seeking );
        assert( pcv == mismatch( cv.begin(), cv.end(), seeking ) );

        pp  = ext::mismatch( p, seeking, bin_predicate() ); 
        pcp = ext::mismatch( cp, seeking, bin_predicate() );
        pa  = ext::mismatch( a, seeking, bin_predicate() ); 
        pca = ext::mismatch( ca, seeking, bin_predicate() );
        pv  = ext::mismatch( v, seeking, bin_predicate() );
        pcv = ext::mismatch( cv, seeking, bin_predicate() );
        assert( pcv == mismatch( cv.begin(), cv.end(), 
                                 seeking, bin_predicate() ) );
#endif
    }



    void test_equal()
    {
        TEST_DATA;
        ext::equal( a, a );
        ext::equal( ca, ca );
        ext::equal( p, p );
        ext::equal( cp, cp );
        ext::equal( v, v );
        assert( ext::equal( cv, cv ) == 
                equal( cv.begin(), cv.end(), cv.begin() ) );

        //
        // @note: clash with std::equal
        //
        ext::equal( p, p, bin_predicate() );
        ext::equal( cp, cp, bin_predicate() );
        ext::equal( a, a, bin_predicate() );
        ext::equal( ca, ca, bin_predicate() );
        ext::equal( v, v, bin_predicate() );
        assert( ext::equal( cv, cv, bin_predicate() ) ==
                equal( cv.begin(), cv.end(), cv.begin(), bin_predicate() ) );

    }



    void test_search()
    {
        TEST_DATA;
        vector<int> seeking( 2 );
	seeking.push_back( 3 ); seeking.push_back( 6 );

        aiter  = search( p, seeking );
        caiter = search( cp, seeking );
        aiter  = search( a, seeking );
        caiter = search( ca, seeking );
        iter   = search( v, seeking );
        citer  = search( cv, seeking );
        assert( citer == search( cv.begin(), cv.end(), 
				 seeking.begin(), seeking.end() ) );

        aiter  = search( p, seeking, bin_predicate() );
        caiter = search( cp, seeking, bin_predicate() );
        aiter  = search( a, seeking, bin_predicate() );
        caiter = search( ca, seeking, bin_predicate() );
        iter   = search( v, seeking, bin_predicate() );
        citer  = search( cv, seeking, bin_predicate() );
        assert( citer == search( cv.begin(), cv.end(), 
				 seeking.begin(), seeking.end(), 
                                 bin_predicate() ) );
    }



    void test_search_n()
    {
        TEST_DATA;
        const int val = 6;
        const int cnt = 1;
        aiter  = ext::search_n( p, cnt, val ); 
        caiter = ext::search_n( cp, cnt, val );
        aiter  = ext::search_n( a, cnt, val ); 
        caiter = ext::search_n( ca, cnt, val );
        iter   = ext::search_n( v, cnt, val );
        citer  = ext::search_n( cv, cnt, val );
        assert( citer == search_n( cv.begin(), cv.end(), cnt, val ) );

        aiter  = ext::search_n( p, cnt, val, bin_predicate() ); 
        caiter = ext::search_n( cp, cnt, val, bin_predicate()  );
        aiter  = ext::search_n( a, cnt, val, bin_predicate() ); 
        caiter = ext::search_n( ca, cnt, val, bin_predicate()  );
        iter   = ext::search_n( v, cnt, val, bin_predicate()  );
        citer  = ext::search_n( cv, cnt, val, bin_predicate()  );
        assert( citer == search_n( cv.begin(), cv.end(), cnt, val, 
                                   bin_predicate() ) ); 
    }



    void test_find_end()
    {
        TEST_DATA;
        vector<int> seeking( 2 );
	seeking.push_back( 3 ); seeking.push_back( 5 );

        aiter  = find_end( p, seeking );
        caiter = find_end( cp, seeking );
        aiter  = find_end( a, seeking );
        caiter = find_end( ca, seeking );
        iter   = find_end( v, seeking );
        citer  = find_end( cv, seeking );
        assert( citer == find_end( cv.begin(), cv.end(), 
                                   seeking.begin(), seeking.end() ) );

        aiter  = find_end( p, seeking, bin_predicate() );
        caiter = find_end( cp, seeking, bin_predicate() );
        aiter  = find_end( a, seeking, bin_predicate() );
        caiter = find_end( ca, seeking, bin_predicate() );
        iter   = find_end( v, seeking, bin_predicate() );
        citer  = find_end( cv, seeking, bin_predicate() );
        assert( citer == find_end( cv.begin(), cv.end(), 
                                   seeking.begin(), seeking.end(),
                                   bin_predicate() ) );
    }

    /////////////////////////////////////////////////////////////////////////
    // Modifying Sequance Operations
    /////////////////////////////////////////////////////////////////////////

    void test_copy()
    {
        TEST_DATA;
        vector<int> copy1( a_size ), copy2( a_size );

        copy( a, copy1 );
        copy( begin( a ), end( a ), begin( copy2 ) );
        assert( ext::equal( copy1, copy2 ) );

	copy( a, out );
    }



    void test_copy_backward()
    {
        TEST_DATA;
        vector<int> copy1( a_size ), copy2( a_size );

        copy_backward( a, copy1 );
        copy_backward( begin( a ), end( a ), end( copy2 ) );
        assert( ext::equal( copy1, copy2 ) );
    }



    void test_swap_ranges()
    {
        TEST_DATA;
        vector<int> copy1( a_size ), copy2( a_size ), a2( a_size );
        copy( a, a2 );
        vector<int>::iterator iter2;

        iter  = swap_ranges( a, copy1 );
        swap_ranges( copy1, a );
        iter2 = swap_ranges( begin( a ), end( a ), begin( copy2 ) );
        assert( ext::equal( a2, copy2 ) );
    }



    void test_transform()
    {
        TEST_DATA;
        vector<int>  a2( a_size ), a3( a_size ), a4( a_size ), a5( a_size );

        iter = transform( a, a2, unary_fun() );
        iter = transform( begin( a ), end( a ), begin( a3 ), unary_fun() );
        assert( ext::equal( a2, a3 ) );

        iter = transform_( a, a2, a4, binary_fun() );
        iter = transform( begin( a ), end( a ), begin( a2 ), 
                           begin( a5 ), binary_fun() );
        assert( ext::equal( a4, a5 ) );
    }



    void test_replace()
    {
        TEST_DATA;
        int what = 5, with_what = 6;
        vector<int> copy = v;
        replace( v, what, with_what );
        replace( copy.begin(), copy.end(), what, with_what );
        assert( ext::equal( v, copy ) );
    }



    void test_replace_if()
    {
        TEST_DATA;
        int with_what = 6;
        vector<int> copy = v;
        replace_if( v, predicate(), with_what );
        replace_if( copy.begin(), copy.end(), predicate(), with_what );
        assert( ext::equal( v, copy ) );

    }



    void test_replace_copy()
    {
        TEST_DATA;
        int what = 5, with_what = 6;
        vector<int> copy1( v.size() ), copy2( v.size() );
        iter = replace_copy( v, copy1, what, with_what );
        iter = replace_copy( v.begin(), v.end(), copy2.begin(), 
                             what, with_what );
        assert( copy1 == copy2 );
    }



    void test_replace_copy_if()
    {
        TEST_DATA;
        int with_what = 6;
        vector<int> copy1( v.size() ), copy2( v.size() );
        iter = replace_copy_if( v, copy1, predicate(), with_what );
        iter = replace_copy_if( v.begin(), v.end(), copy2.begin(), 
                                predicate(), with_what );
        assert( ext::equal( copy1, copy2 ) );
    }



    void test_fill()
    {
        const int size = 10;
        const int with = 1;
        vector<int> a( size ), a2( size );
        fill( a, with );
        fill( begin( a2 ), end( a2 ), with );
        assert( ext::equal( a, a2 ) );
    }



    void test_fill_n()
    {
        const int size = 10, n = 5;
        const int with = 1;
        vector<int> a( size ), a2( size );
        fill_n_( a, n, with );
        fill_n( begin( a2 ), n, with );
        assert( ext::equal( a, a2 ) );        
    }



    void test_generate()
    {
        const int size = 10;
        vector<int> a( size ), a2( size );
        generate( a, generator() );
        generate( begin( a2 ), end( a2 ), generator() );
        assert( ext::equal( a, a2 ) );        
    }



    void test_generate_n()
    {
        const int n = 5;
        vector<int> a( n ), a2( n );
        ext::generate_n( a, n, generator() );
        generate_n( begin( a2 ), n, generator() );
        assert( ext::equal( a, a2 ) );        
    }



    void test_remove()
    {
        TEST_DATA;
        vector<int> a2( a_size );
        const int what = 3;
        copy( a, a2 ); 

        aiter = remove( a, what );
        iter = remove( begin( a2 ), end( a2 ), what );
        assert( ext::equal( a, a2 ) ); // ?
    }



    void test_remove_if()
    {
        TEST_DATA;
        vector<int> a2( a_size );
        const int what = 3;
        copy( a, a2 ); 

        aiter = remove_if( a, predicate() );
        iter  = remove_if( begin( a2 ), end( a2 ), predicate() );
        assert( ext::equal( a, a2 ) );
    }



    void test_remove_copy()
    {
        TEST_DATA;
        vector<int>::iterator iter2;
	const int new_size = a_size - 3;
        vector<int>  a2( new_size ), a3( new_size );
        const int what = 3;

        iter  = remove_copy( a, a2, what );
        iter2 = remove_copy( begin( a ), end( a ), begin( a3 ), what );

        assert( ext::equal( a2, a3 ) );    
    }



    void test_remove_copy_if()
    {
        TEST_DATA;
        vector<int>::iterator iter2;
	const int new_size = a_size - 2;
        vector<int>  a2( new_size ), a3( new_size );

        iter  = remove_copy_if( a, a2, predicate() );
        iter2 = remove_copy_if( begin( a ), end( a ), 
                                 begin( a3 ), predicate() ); 
        assert( ext::equal( a2, a3 ) );
    }



    void test_unique()
    {
        TEST_DATA;
        vector<int>  a2( a_size ), a3( a_size );
        copy( a, a2 ); copy( a, a3 );

        iter = unique( a2 );
        iter = unique( begin( a3 ), end( a3 ) );
        assert( ext::equal( a2, a3 ) );

        iter = unique_( a2, bin_predicate() );
        iter = unique( begin( a3 ), end( a3 ), bin_predicate() );
        assert( ext::equal( a2, a3 ) );
	print2( a3, a2 );
    }



    void test_unique_copy()
    {
        TEST_DATA;
        vector<int>  a2( a_size ), a3( a_size );
        fill( a2, 0 ); fill( a3, 0 );

        iter = unique_copy( a, a2 );
        iter = unique_copy( begin( a ), end( a ), begin( a3 ) );
        assert( ext::equal( a2, a3 ) );

        iter = unique_copy_( a, a2, bin_predicate() );
        iter = unique_copy( begin( a ), end( a ), 
			    begin( a3 ), bin_predicate() );
        assert( ext::equal( a2, a3 ) );
	print2( a3, a2 );
    }



    void test_reverse()
    {
        TEST_DATA;
        vector<int> v1( v );
        reverse( v1 );
        reverse( v.begin(), v.end() );
        assert( v1 == v );
    }



    void test_reverse_copy()
    {
        TEST_DATA;
        vector<int> v1( v.size() ), v2( v.size() );
        iter = reverse_copy( v, v1 );
        iter = reverse_copy( v.begin(), v.end(), v2.begin() );
        assert( v1 == v2 ); 
    }



    void test_rotate()
    {
        TEST_DATA;
        vector<int> v1( v );

        vector<int>::iterator middle1 = v.begin() + v.size() / 2;
	vector<int>::iterator middle2 = v1.begin() + v.size() / 2;
        rotate( v, middle1 );
        rotate( begin( v1 ), middle2, end( v1 ) );
        assert( ext::equal( v, v1 ) ); 
	print2( v, v1 );
    }



    void test_rotate_copy()
    {
        TEST_DATA;
        vector<int> v1( v.size() ), v2( v.size() );

        vector<int>::iterator middle = v.begin() + v.size() / 2;
        iter = rotate_copy( v, middle, v1 );
        iter = rotate_copy( v.begin(), middle, v.end(), v2.begin()  );
        assert( ext::equal( v1, v2 ) ); 
	print2( v1, v2 );
    }



    void test_random_shuffle()
    {
        TEST_DATA;
	random_shuffle( a );
	random_shuffle( a, unary_fun( a_size ) );
	random_shuffle( v );
        random_shuffle( v, unary_fun( v.size() ) );
	random_shuffle( p, unary_fun( v.size() ) );
        random_shuffle( p );
    } 

    /////////////////////////////////////////////////////////////////////////
    // Sorted Sequences
    /////////////////////////////////////////////////////////////////////////

    void test_sort()
    {
        TEST_DATA;
        vector<int> v2( v );
        ext::sort( v );
        sort( v2.begin(), v2.end() );
        assert( v == v2 );

        ext::sort( v, bin_predicate() );
        sort( v2.begin(), v2.end(), bin_predicate() );
        assert( v == v2 );
    }



    void test_stable_sort()
    {
        TEST_DATA;
        vector<int> v2( v );
        ext::stable_sort( v );
        stable_sort( v2.begin(), v2.end() );
        assert( v == v2 );

        ext::stable_sort( v, bin_predicate() );
        stable_sort( v2.begin(), v2.end(), bin_predicate() );
        assert( v == v2 );
    }



    void test_partial_sort()
    {
        TEST_DATA;
        vector<int> v2( v );
        iterator middle1 = v.begin() + v.size() / 2;
        iterator middle2 = v2.begin() + v2.size() / 2;

        partial_sort( v, middle1 );
        partial_sort( v2.begin(), middle2, v2.end() );
        assert( v == v2 );

        partial_sort_( v, middle1, bin_predicate() );
        partial_sort( v2.begin(), middle2, v2.end(), bin_predicate() );
        assert( v == v2 );
    }



    void test_partial_sort_copy()
    {
        TEST_DATA;
        vector<int> v2( v.size() ), v3( v.size() );
        iter = partial_sort_copy( v, v2 );
        iter = partial_sort_copy( v.begin(), v.end(), v3.begin(), v3.end() );
        assert( v2 == v3 );

        iter = partial_sort_copy( v, v2, bin_predicate() );
        iter = partial_sort_copy( v.begin(), v.end(), 
                                  v3.begin(), v3.end(), bin_predicate() );
        assert( v2 == v3 );
    }



    void test_nth_element()
    {
        TEST_DATA;
        vector<int> v2( v );
        iterator nth1 = v.begin() + v.size() / 2 ;
        iterator nth2 = v2.begin() + v2.size() / 2 ;

        nth_element( v, nth1 );
        nth_element( v2.begin(), nth2, v2.end() );
        assert( v == v2 );

        nth_element_( v, nth1, bin_predicate() );
        nth_element( v2.begin(), nth2, v2.end(), bin_predicate() );
        assert( v == v2 );
	print2( v, v2 );

    }



    void test_lower_bound()
    {
        TEST_DATA;
        vector<int> v2( v );
        ext::sort( v ); ext::sort( v2 );
        const int bound = 5;
        iterator iter2;

        iter   = lower_bound( v, bound );
        iter2  = lower_bound( v2.begin(), v2.end(), bound );
        assert( *iter == *iter2 ); 
        citer = lower_bound( cv, bound ); 
        iter  = lower_bound_( v, bound, bin_predicate() );
        iter2 = lower_bound( v2.begin(), v2.end(), bound, bin_predicate() );
        assert( *iter == *iter2 );
        citer = lower_bound_( cv, bound, bin_predicate() );
    }



    void test_upper_bound()
    {
        TEST_DATA;
        vector<int> v2( v );
        ext::sort( v ); ext::sort( v2 );
        const int bound = 5;
        iterator iter2;

        iter   = upper_bound( v, bound );
        iter2  = upper_bound( v2.begin(), v2.end(), bound );
        assert( *iter == *iter2 ); 
        citer = upper_bound( cv, bound ); 
        iter  = upper_bound_( v, bound, bin_predicate() );
        iter2 = upper_bound( v2.begin(), v2.end(), bound, bin_predicate() );
        assert( *iter == *iter2 );
        citer = upper_bound_( cv, bound, bin_predicate() );

    }



    void test_equal_range()
    {
        TEST_DATA;
        vector<int> v2( v );
        ext::sort( v ); ext::sort( v2 );
        const int bound = 5;
        pair<const_iterator, const_iterator> pci;
        pair<iterator, iterator>             pi1, pi2;

        pi1   = equal_range( v, bound );
        pi2   = equal_range( v2.begin(), v2.end(), bound );
        assert( *pi1.first == *pi2.first ); 
        pci   = equal_range( cv, bound ); 
        pi1  = equal_range_( v, bound, bin_predicate() );
        pi2 = equal_range( v2.begin(), v2.end(), bound, bin_predicate() );
        assert( *pi1.first == *pi2.first ); 
        pci = equal_range_( cv, bound, bin_predicate() );
    }



    void test_binary_search()
    {
        TEST_DATA;
        vector<int> v2( v );
        ext::sort( v ); ext::sort( v2 );
        const int bound = 5;
        bool b1 = binary_search( v, bound );
        bool b2 = binary_search( v2.begin(), v2.end(), bound );
        assert( b1 == b2 );

        b1 = binary_search_( v, bound, bin_predicate() );
        b2 = binary_search( v2.begin(), v2.end(), bound, bin_predicate() );
        assert( b1 == b2 );
    }



    void test_merge()
    {
        TEST_DATA;
        vector<int> v2( v ), v3( 2 * v.size() ), v4( 2 * v.size() );
        ext::sort( v ); ext::sort( v2 );

        iter = merge( v, v2, v3 );
        iter = merge( v.begin(), v.end(), v2.begin(), v2.end(), v4.begin() );
        assert( v3 == v4 );

        iter = merge( v, v2, v3, bin_predicate() );
        iter = merge( v.begin(), v.end(), v2.begin(), 
                      v2.end(), v4.begin(), bin_predicate() );
        assert( v3 == v4 );

    }



    void test_implace_merge()
    {
        TEST_DATA;
        vector<int> v2( v );
        iterator middle1 = v.begin() + v.size() / 2;
        iterator middle2 = v2.begin() + v2.size() / 2;;

        inplace_merge( v, middle1 );
        inplace_merge( v2.begin(), middle2, v2.end() );
        assert( v == v2 );

        inplace_merge_( v, middle1, bin_predicate() );
        inplace_merge( v2.begin(), middle2, v2.end(), bin_predicate() );
        assert( v == v2 );
    }



    void test_partition()
    {
        TEST_DATA;
        vector<int> v2( v );
        iter = partition( v, predicate() );
        iter = partition( v2.begin(), v2.end(), predicate() );
        assert( v == v2 ); 
    }



    void test_stable_partition()
    {
        TEST_DATA;
        vector<int> v2( v );
        iter = partition( v, predicate() );
        iter = partition( v2.begin(), v2.end(), predicate() );
        assert( v == v2 ); 
    }

    /////////////////////////////////////////////////////////////////////////
    // Set Algorithms
    /////////////////////////////////////////////////////////////////////////

    void test_includes()
    {
        TEST_DATA;
        vector<int> seeking( 2 ); 
	seeking.push_back( 1 ); seeking.push_back( 2 );

        bool b1 = includes( v, seeking );
        bool b2 = includes( v.begin(), v.end(),seeking.begin(),seeking.end() );
        assert( b1 == b2 );

        b1 = includes( v, seeking, bin_predicate() );
        b2 = includes( v.begin(), v.end(), 
                       seeking.begin(), seeking.end(), bin_predicate() );
        assert( b1 == b2 );
    }



    void test_set_union()
    {
        TEST_DATA;
        vector<int> v1( v.size() + cv.size() ), v2( v.size() + cv.size() );
        fill( v1, 0 ); fill( v2, 0 );

        iter = set_union( v, cv, v1 );
        iter = set_union( v.begin(), v.end(), 
                          cv.begin(), cv.end(), v2.begin() );
        assert( v1 == v2 );

        iter = set_union( v, cv, v1, bin_predicate() );
        iter = set_union( v.begin(), v.end(), 
                          cv.begin(), cv.end(), v2.begin(), bin_predicate() );
        assert( v1 == v2 );
    }



    void test_set_intersection()
    {
        TEST_DATA;
        vector<int> v1( v.size() ), v2( v.size() );
        fill( v1, 0 ); fill( v2, 0 );

        iter = set_intersection( v, cv, v1 );
        iter = set_intersection( v.begin(), v.end(), 
                                 cv.begin(), cv.end(), v2.begin() );
        assert( v1 == v2 );

        iter = set_intersection( v, cv, v1, bin_predicate() );
        iter = set_intersection( v.begin(), v.end(), 
                                 cv.begin(), cv.end(), v2.begin(), bin_predicate() );
        assert( v1 == v2 );   
    }



    void test_set_difference()
    {
        TEST_DATA;
        vector<int> v1( v.size() ), v2( v.size() );
        fill( v1, 0 ); fill( v2, 0 );

        iter = set_difference( v, cv, v1 );
        iter = set_difference( v.begin(), v.end(), 
                               cv.begin(), cv.end(), v2.begin() );
        assert( v1 == v2 );

        iter = set_difference( v, cv, v1, bin_predicate() );
        iter = set_difference( v.begin(), v.end(), 
                               cv.begin(), cv.end(), v2.begin(), 
                               bin_predicate() );
        assert( v1 == v2 );
    }



    void test_set_symmetric_difference()
    {
        TEST_DATA;
        vector<int> v1( v.size() ), v2( v.size() );
        fill( v1, 0 ); fill( v2, 0 );

        iter = set_symmetric_difference( v, cv, v1 );
        iter = set_symmetric_difference( v.begin(), v.end(), 
                                         cv.begin(), cv.end(), v2.begin() );
        assert( v1 == v2 );

        iter = set_symmetric_difference( v, cv, v1, bin_predicate() );
        iter = set_symmetric_difference( v.begin(), v.end(), 
                                         cv.begin(), cv.end(), v2.begin(), 
                                         bin_predicate() );
        assert( v1 == v2 );
    }

    /////////////////////////////////////////////////////////////////////////
    // Heap Operations
    /////////////////////////////////////////////////////////////////////////

    void test_push_heap()
    {
        TEST_DATA;
        vector<int> v1( v ), v2( v ), v3( v );
        ext::make_heap( v ); ext::make_heap( v1 );

        ext::push_heap( v );
        push_heap( v1.begin(), v1.end() );
        assert( v == v1 );

        ext::make_heap( v2, bin_predicate() );
	ext::make_heap( v3, bin_predicate() );
        ext::push_heap( v2, bin_predicate() );
        push_heap( v3.begin(), v3.end(), bin_predicate() );
        assert( v2 == v3 );  
    }



    void test_pop_heap()
    {
        TEST_DATA;
        vector<int> v1( v );
        ext::make_heap( v ); ext::make_heap( v1 );

        ext::pop_heap( v );
        pop_heap( v1.begin(), v1.end() );
        assert( v == v1 );

        ext::make_heap( v, bin_predicate() ); 
	ext::make_heap( v1, bin_predicate() );
        ext::pop_heap( v, bin_predicate() );
        pop_heap( v1.begin(), v1.end(), bin_predicate() );
        assert( v == v1 );  
    }



    void test_make_heap()
    {
        TEST_DATA;
        vector<int> v1( v );

        ext::make_heap( v );
        make_heap( v1.begin(), v1.end() );
        assert( v == v1 );

        ext::make_heap( v, bin_predicate() );
        make_heap( v1.begin(), v1.end(), bin_predicate() );
        assert( v == v1 );  
    }



    void test_sort_heap()
    {
        TEST_DATA;
        vector<int> v1( v );
        ext::make_heap( v ); ext::make_heap( v1 );

        ext::sort_heap( v );
        sort_heap( v1.begin(), v1.end() );
        assert( v == v1 );

        ext::make_heap( v, bin_predicate() ); 
	ext::make_heap( v1, bin_predicate() );
        ext::sort_heap( v, bin_predicate() );
        sort_heap( v1.begin(), v1.end(), bin_predicate() );
        assert( v == v1 );  
    }

    /////////////////////////////////////////////////////////////////////////
    // Minimum and Maximum
    /////////////////////////////////////////////////////////////////////////

    void test_min_eleement()
    {
        TEST_DATA;
        iterator iter2;
        iter  = min_element( v );
        iter2 = min_element( v.begin(), v.end() );
        assert( *iter == *iter2 );
        citer  = min_element( cv );

        iter  = min_element_( v, bin_predicate() );
        iter2 = min_element( v.begin(), v.end(), bin_predicate() );
        assert( *iter == *iter2 );
        citer  = min_element_( cv, bin_predicate() );
    }



    void test_max_element()
    {
        TEST_DATA;
        iterator iter2;
        iter  = max_element( v );
        iter2 = max_element( v.begin(), v.end() );
        assert( *iter == *iter2 );
        citer  = max_element( cv );

        iter  = max_element_( v, bin_predicate() );
        iter2 = max_element( v.begin(), v.end(), bin_predicate() );
        assert( *iter == *iter2 );
        citer  = max_element_( cv, bin_predicate() );
    }



    void test_lexicographical_compare()
    {
        TEST_DATA;
        bool b1 = lexicographical_compare( v, ca );
        bool b2 = lexicographical_compare( v.begin(), v.end(), 
                                           begin( ca ), end( ca ) );
        assert( b1 == b2 );

        b1 = lexicographical_compare( v, ca, bin_predicate() );
        b2 = lexicographical_compare( v.begin(), v.end(), 
                                      begin( ca ), end( ca ), 
                                      bin_predicate() );
        assert( b1 == b2 );
    }

    /////////////////////////////////////////////////////////////////////////
    // Permutations
    /////////////////////////////////////////////////////////////////////////

    void test_next_permutation()
    {
        TEST_DATA;
        vector<int> v1( v );

        ext::next_permutation( v );
        next_permutation( v1.begin(), v1.end() );
        assert( v == v1 );

        ext::next_permutation( v, bin_predicate() );
        next_permutation( v1.begin(), v1.end(), bin_predicate() );
        assert( v == v1 );
    }



    void test_prev_permutation()
    {
        TEST_DATA;
        vector<int> v1( v );

        ext::prev_permutation( v );
        prev_permutation( v1.begin(), v1.end() );
        assert( v == v1 );

        ext::prev_permutation( v, bin_predicate() );
        prev_permutation( v1.begin(), v1.end(), bin_predicate() );
        assert( v == v1 );
    }

    /////////////////////////////////////////////////////////////////////////
    // Generalized Numeric Algorithms
    /////////////////////////////////////////////////////////////////////////

    void test_accumulate()
    {
        TEST_DATA;
        assert( accumulate( v, 0 ) ==
                accumulate( v.begin(), v.end(), 0 ) );
        assert( accumulate_( v, 0, binary_fun() ) ==
                accumulate( v.begin(), v.end(), 0, binary_fun() ) );
    }



    void test_inner_product()
    {
        TEST_DATA;
        assert( inner_product( v, ca, 0 ) ==
                inner_product( v.begin(), v.end(), ca, 0 ) );

        assert( inner_product( v, ca, 0, binary_fun(), binary_fun() ) ==
                inner_product( v.begin(), v.end(), ca, 0,
                               binary_fun(), binary_fun() ) );

    }



    void test_partial_sum()
    {
        TEST_DATA;
        vector<int> v1( v.size() ), v2( v.size() );

        iter = partial_sum( v, v1 );
        iter = partial_sum( v.begin(), v.end(), v2.begin() );
        assert( v1 == v2 );

        iter = partial_sum_( v, v1, binary_fun() );
        iter = partial_sum( v.begin(), v.end(), v2.begin(), binary_fun() );
        assert( v1 == v2 );
    }



    void test_adjacent_difference()
    {
        TEST_DATA;
        vector<int> v1( v.size() ), v2( v.size() );

        iter = adjacent_difference( v, v1 );
        iter = adjacent_difference( v.begin(), v.end(), v2.begin() );
        assert( v1 == v2 );

        iter = adjacent_difference_( v, v1, binary_fun() );
        iter = adjacent_difference( v.begin(), v.end(), 
				    v2.begin(), binary_fun() );
    }

    /////////////////////////////////////////////////////////////////////////
    // Boost algorithm extensions
    /////////////////////////////////////////////////////////////////////////

    /*
    void test_iota()
    {
        TEST_DATA;
        vector<int> v1( v );
        
        iota( v, 0 );
        iota( v1.begin(), v1.end(), 0 );
        
        assert( v == v1 );
    }
    
    
    
    void test_contains()
    {
        TEST_DATA;
        vector<int> v1( v );
        const int n = 0;
        
        bool b1 = contains( v, n );
        bool b2 = contains( v1.begin(), v1.end(), n );
        assert( b1 == b2 );
    }
    
    
    
    void test_all()
    {
        TEST_DATA;
        vector<int> v1( v );
                
        bool b1 = all( v, predicate() );
        bool b2 = all( v1.begin(), v1.end(), predicate() );
        assert( b1 == b2 );
    }
    
    
    
    void test_none()
    {
        TEST_DATA;
        vector<int> v1( v );
                
        bool b1 = none( v, predicate() );
        bool b2 = none( v1.begin(), v1.end(), predicate() );
        assert( b1 == b2 );
    }
    
    
    
    void test_any_if()
    {
        TEST_DATA;
        vector<int> v1( v );
                
        bool b1 = any_if( v, predicate() );
        bool b2 = any_if( v1.begin(), v1.end(), predicate() );
        assert( b1 == b2 );
    }
    
    
    
    void test_is_sorted()
    {
        TEST_DATA;
        vector<int> v1( v );
                
        bool b1 = is_sorted( v );
        bool b2 = is_sorted( v1.begin(), v1.end() );
        assert( b1 == b2 );
    }
    */
} // namespace ''






