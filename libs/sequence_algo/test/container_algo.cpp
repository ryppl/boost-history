//
// short: Test cases for container algortihms.
//
//    

// (C) Copyright Thorsten Ottosen 2002. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.


#include "../container_algo.hpp"

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
    // - all iterators are non-const?
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

}


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

    return 0;
} // 'main()'

namespace 
{

    //////////////////////////////////////////////////////////////////////////////
    // Implementation
    //////////////////////////////////////////////////////////////////////////////

    // enable ambiguity conflicts
    using namespace std;
    using namespace boost;

    #define TEST_DATA                                                        \
        pair<int, int>      p( 1, 2 );                                       \
        const pair<int,int> cp( 2, 3);                                       \
        int                 a[] = { 1, 2, 4, 2, 6, 7, 9, 3, 2, 5, 5, 3, 3 }; \
        const int           ca[] = { 1, 2, 3, 5, 7, 9, 11 };                 \
        vector<int>         v( a, a + sizeof( a ) / sizeof( a[0] ) );        \
        const vector<int>   cv( v );                                         \
        int*                                 aiter;                          \
        const int*                           caiter;                         \
        typedef vector<int>::iterator        iterator;                       \
        typedef vector<int>::const_iterator  const_iterator;                 \
        iterator iter; const_iterator citer; 

    struct unary_fun;
    struct predicate;
    struct bin_predicate;



    struct unary_fun : public unary_function<int,int>
    {
        int operator()( int i )
        {
            return ++i;
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



    void test_for_each()
    {
        TEST_DATA;
        //for_each( p, unary_fun() );  
        //for_each( cp, unary_fun() ) );  
        for_each( a, unary_fun() );
        for_each( ca, unary_fun() );
        for_each( v, unary_fun() );
        for_each( cv, unary_fun() );
    }



    void test_find()
    {
        TEST_DATA;
        aiter  = find( a, 5 );
        caiter = find( ca, 5 );
        iter   = find( v, 3 );
        citer  = find( cv, 3 );
    }



    void test_find_if()
    {
        TEST_DATA;
        aiter  = find_if( a, predicate() );
        caiter = find_if( ca, predicate() );
        iter   = find_if( v, predicate() );
        citer  = find_if( cv, predicate() );
    }



    void test_adjacent_find()
    {
        TEST_DATA;
        aiter  = adjacent_find( a );
        caiter = adjacent_find( ca );
        iter   = adjacent_find( v );
        citer  = adjacent_find( cv );

        aiter  = adjacent_find( a, bin_predicate() );
        caiter = adjacent_find( ca, bin_predicate() );
        iter   = adjacent_find( v, bin_predicate() );
        citer  = adjacent_find( cv, bin_predicate() );      
    }



    void test_find_first_of()
    {
        TEST_DATA;
        int seeking[] = { 6, 5};

        aiter  = find_first_of( a, seeking );
        caiter = find_first_of( ca, seeking );
        iter   = find_first_of( v, seeking  );
        citer  = find_first_of( cv, seeking );

        aiter  = find_first_of( a, seeking, bin_predicate() );
        caiter = find_first_of( ca, seeking, bin_predicate() );
        iter   = find_first_of( v, seeking, bin_predicate() );
        citer  = find_first_of( cv, seeking, bin_predicate() );      

    }



    void test_count()
    {
        TEST_DATA;
        const int val = 5;
        size_t ac;
        ac = count( a, val );
        ac = count( ca, val );
        vector<int>::difference_type cc;
        cc = count( v, val );
        cc = count( cv, val );
    }



    void test_count_if()
    {
        TEST_DATA;
        size_t ac;
        ac = count_if( a, predicate() );
        ac = count_if( ca, predicate() );
        vector<int>::difference_type cc;
        cc = count_if( v, predicate() );
        cc = count_if( cv, predicate() );
    }



    void test_mismatch()
    {
        //
        // @todo: Figure out the appropriate method of returning pairs
        //
        TEST_DATA;
        int seeking[] = { 1, 2, 3};
        pair<int*, int*>            pa  = mismatch( a, seeking );
        pair<const int*, int*>      pca = mismatch( ca, seeking );
        pair<iterator, int*>        pv  = mismatch( v, seeking );
        pair<const_iterator, int*>  pcv = mismatch( cv, seeking );

        pa  = mismatch_( a, seeking, bin_predicate() ); 
        pca = mismatch_( ca, seeking, bin_predicate() );
        pv  = mismatch_( v, seeking, bin_predicate() );
        pcv = mismatch_( cv, seeking, bin_predicate() );
    }


    void test_equal()
    {
        TEST_DATA;
        equal( a, a );
        equal( ca, ca );
        equal( v, v );
        equal( cv, cv );

        //
        // @note: clash with std::equal
        //
        equal_( a, a, bin_predicate() );
        equal_( ca, ca, bin_predicate() );
        equal_( v, v, bin_predicate() );
        equal_( cv, cv, bin_predicate() );
    }



    void test_search()
    {
        TEST_DATA;
        int seeking[] = { 3,6};

        aiter  = search( a, seeking );
        caiter = search( ca, seeking );
        iter   = search( v, seeking );
        citer  = search( cv, seeking );

        aiter  = search( a, seeking, bin_predicate() );
        caiter = search( ca, seeking, bin_predicate() );
        iter   = search( v, seeking, bin_predicate() );
        citer  = search( cv, seeking, bin_predicate() );
    }



    void test_search_n()
    {
        TEST_DATA;
        const int val = 6;
        const int cnt = 1;
        aiter  = search_n( a, cnt, val ); 
        caiter = search_n( ca, cnt, val );
        iter   = search_n( v, cnt, val );
        citer  = search_n( cv, cnt, val );

        aiter  = search_n( a, cnt, val, bin_predicate() ); 
        caiter = search_n( ca, cnt, val, bin_predicate()  );
        iter   = search_n( v, cnt, val, bin_predicate()  );
        citer  = search_n( cv, cnt, val, bin_predicate()  );
    }



    void test_find_end()
    {
        TEST_DATA;
        int seeking[] = { 3,5};

        aiter  = find_end( a, seeking );
        caiter = find_end( ca, seeking );
        iter   = find_end( v, seeking );
        citer  = find_end( cv, seeking );

        aiter  = find_end( a, seeking, bin_predicate() );
        caiter = find_end( ca, seeking, bin_predicate() );
        iter   = find_end( v, seeking, bin_predicate() );
        citer  = find_end( cv, seeking, bin_predicate() );
    }

} // namespace ''



