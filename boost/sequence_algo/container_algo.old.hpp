// (C) Copyright Thorsten Ottosen 2002. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// (C) Copyright Jeremy Siek 2001. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// Mutating algorithms originally written by Vladimir Prus' 
// <ghost@cs.msu.su> code from Boost Wiki

// Problem: should member functions be called automatically? Or should the user 
// know that it is better to call map::find() than find( map )?

// Thu May 30     I strongly disagree to have two different type for
//                begin() and end() in std::pair<Iter1,Iter2>
//                -- makes no sense if they describe a range [Herve]

// Thu May 30     Proposed to put container interfaces
//                into separate file for <algorithm> extensions [Herve]


#ifndef BOOST_SEQUENCE_ALGO_CONTAINER_ALGORITHM_HPP
#define BOOST_SEQUENCE_ALGO_CONTAINER_ALGORITHM_HPP

//#include <boost/sequence_algo/algorithm.hpp>
#include "container_traits.hpp"
#include <boost/concept_check.hpp>
#include <algorithm>
#include <numeric>

namespace boost 
{
    /////////////////////////////////////////////////////////////////////////
    // Interface
    /////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////
    // Nonmodifying Sequence Operations
    /////////////////////////////////////////////////////////////////////////

    /*
    
    Unary_function    for_each( const Container&, Unary_function ); 
    
    iterator          find( Container&, const T& );   
    iterator          find( const Container&, const T& ); 
    
    iterator          find_if( Container&, Predicate );  
    iterator          find_if( const Container&, Predicate );
    
    iterator          adjacent_find( Container& );
    iterator          adjacent_find( Container&, Binary_predicate ); 
    iterator          adjacent_find( const Container& );
    iterator          adjacent_find( const Container&, Binary_predicate );  
    
    iterator          find_first_of( Container1&, const Container2& ); 
    iterator          find_first_of( const Container1&, const Container2& ); 
    iterator          find_first_of( Container1&, const Container2&, 
                                     Binary_predicate );   
    iterator          find_first_of( const Container1&, const Container2&, 
                                     Binary_predicate ); 
    
    difference_type   count( const Container&, const T& );
    
    difference_type   count_if( const Container&, Predicate );    
    
    pair              mismatch( Container1&, Container2& );
    pair              mismatch( const Container1&, Container2& );
    pair              mismatch( Container1&, const Container2& ); 
    pair              mismatch( const Container1&, const Container2& ); 
    pair              mismatch_( Container1&, Container2&
                                Binary_predicate );                      
    pair              mismatch_( const Container1&, Container2&
                                Binary_predicate );                                    
    pair              mismatch_( Container1&, const Container2&
                                Binary_predicate );                   
    pair              mismatch_( const Container1&, const Container2&
                                Binary_predicate ); 
                                
    bool              equal( const Container1&, const Container2& );  
    bool              equal_( const Container1&, const Container2&,
                             Binary_predicate );  
                             
    iterator          search( Container1&, const Container2& ); 
    iterator          search( const Container1&, const Container2& ); 
    iterator          search( Container1&, const Container2&.
                              Binary_predicate );                              
    iterator          search( Container1&, const Container2&.
                              Binary_predicate );
                               
    iterator          search_n( Container&, Integer, const T& );
    iterator          search_n( const Container&, Integer, const T& );
    iterator          search_n( Container&, Integer, const T&, 
                                Binary_predicate );        
    iterator          search_n( const Container&, Integer, const T&, 
                                Binary_predicate );        
                                  
    iterator          find_end( Container1&, const Container3& ); 
    iterator          find_end( const Container1&, const Container3& ); 
    iterator          find_end( Container1&, const Container3&, 
                                Binary_predicate ); 
    iterator          find_end( const Container1&, const Container3&, 
                                Binary_predicate ); 
        
    */

    /////////////////////////////////////////////////////////////////////////
    // Modifying Sequance Operations
    /////////////////////////////////////////////////////////////////////////

    /*
    
    iterator          copy( const Container1&, Container2& ); 
    
    iterator          copy_backward( const Container1&, Container2& );
    
    iterator          swap_ranges( Container1&, Container2& );
    
    iterator          transform( const Container1&, Container2&, 
                                 Unary_function );
    iterator          transform( const Container1&, const Container2&,
                                 Container3&, Binary_function );
    
    void              replace( Container&, const T&, const T& ); 
    
    void              replace_if( Container&, Predicate, const T& );
    
    iterator          replace_copy( const Container1&, Container2&, 
                                    const T&, const T& ); 
    
    iterator          replace_copy_if( const Container1&, Container2&,
                                       Predicate, const T& );
                                        
    void              fill( Container&, const T& );
     
    iterator          fill_n( Container&, Integer, const T& );
    
    void              generate( Container&, Generator );           
    
    iterator          generare_n( Container&, Integer, Generator );

    iterator          remove( Container&, const T );
     
    iterator          remove_if( Container&, Predicate ); 

    iterator          remove_copy( const Container1&, Container2&, const T& ); 
    
    iterator          remove_copy_if( const Container1&, Container2&, 
                                      Predicate );
    iterator          unique( Container& );
    iterator          unique( Container&, Binary_predicate );
     
    iterator          unique_copy( const Container1&, Container2& );
    iterator          unique_copy( const Container1&, Container2&, 
                                   Binary_predicate );
    
    void              reverse( Container& );
    
    iterator          reverse_copy( const Container1&, Container2 );
     
    iterator          rotate( Container&, iterator );
    
    iterator          rotate_copy( const Container1&, iterator, Container& ) 

    void              random_shuffle( Container& );
    void              random_shuffle( Container&, Random_number_generator ); 
    
    */    

    /////////////////////////////////////////////////////////////////////////
    // Sorted Sequences
    /////////////////////////////////////////////////////////////////////////

    /*
    
    void              sort( Container& );
    void              sort( Container&, Compare );
     
    void              stable_sort( Container& );
    void              stable_sort( Container&, Compare );
    
    void              partial_sort( Container&, Random_access_iterator );
    void              partial_sort( Container&, Random_access_iterator,     
                                    Compare );
                                    
    iterator          partial_sort_copy( const Container1&, Container2& );
    iteratot          partial_sort_copy( const Container1&, Container2&,
                                         Compare );
                                      
    void              nth_element( Container1&, Random_access_iterator );
    void              nth_element( Container1&, Random_access_iterator, 
                                   Compare );
    
    iterator          lower_bound( Container&, const T& );
    iterator          lower_bound( const Container&, const T& );
    iterator          lower_bound( Container&, const T&, Compare );  
    iterator          lower_bound( const Container&, const T&, Compare );
    
    iterator          upper_bound( Container&, const T& );
    iterator          upper_bound( const Container&, const T& );
    iterator          upper_bound( Container&, const T&, Compare );
    iterator          upper_bound( const Container&, const T&, Compare );
    
    pair              equal_range( Container&, const T& );
    pair              equal_range( const Container&, const T& );
    pair              equal_range( Container&, const T&, Compare );
    pair              equal_range( const Container&, const T&, Compare );
     
    bool              binary_search( const Container&, const T& );
    bool              binary_search( const Container&, const T&, Compare );  
    
    iterator          merge( const Container1&, const Container2&, 
                             Container3& );
    iterator          merge( const Container1&, const Container2&, 
                             Container3&, Compare );
                            
    void              inplace_merge( Container&, 
                                     Bidirectional_iterator middle ); 
    void              inplace_merge( Container&, Bidirectional_iterator middle,
                                     Compare );                                  
                                     
    iterator          partition( Container&, Predicate );
    
    iterator          stable_partition( Container&, Predicate ); 
   
    */

    /////////////////////////////////////////////////////////////////////////
    // Set Algorithms
    /////////////////////////////////////////////////////////////////////////

    /*
    
    bool              includes( const Container1&, const Container2& );
    bool              includes( const Container1&, const Container2&, 
                                Compare );
                                
    iterator          set_union( const Container1&, const Container2&, 
                                 Container3& c3 );
    iterator          set_union( const Container1&, const Container2&, 
                                 Container3& c3, Compare );
                                 
    iterator          set_intersection( const Container1&, const Container2&, 
                                        Container3& c3 );
    iterator          set_intersection( const Container1&, const Container2&, 
                                        Container3& c3, Compare );
                                        
    iterator          set_difference( const Container1&, const Container2&, 
                                      Container3& c3 );
    iterator          set_difference( const Container1&, const Container2&, 
                                      Container3& c3, Compare );
                                 
    iterator          set_symmetric_difference( const Container1&,             
                                                const Container2&, 
                                                Container3& c3 );
    iterator          set_symmetric_difference( const Container1&, 
                                                const Container2&, 
                                                Container3& c3, Compare );
    */ 

    /////////////////////////////////////////////////////////////////////////
    // Heap Operations
    /////////////////////////////////////////////////////////////////////////

    /*
    
    void              push_heap( Container& );
    void              push_heap( Container&, Compare );
    
    void              pop_heap( Container& );
    void              pop_heap( Container&, Compare );

    void              make_heap( Container& );
    void              make_heap( Container&, Compare );
    
    void              sort_heap( Container& ); 
    void              sort_heap( Container&, Compare );  
    
    */ 

    /////////////////////////////////////////////////////////////////////////
    // Minimum and Maximum
    /////////////////////////////////////////////////////////////////////////

    /*
    
    iterator          min_element( Container& );
    iterator          min_element( const Container& );
    iterator          min_element( Container&, Binary_predicate );
    iterator          min_element( const Container&, Binary_predicate );
    
    iterator          max_element( Container& );
    iterator          max_element( const Container& );
    iterator          min_element( Container&, Binary_predicate ); 
    iterator          min_element( const Container&, Binary_predicate ); 
    
    bool              lexicographical_compare( const Container1&, 
                                               const Container2& );
    bool              lexicographical_compare( const Container1&, 
                                               const Container2&, 
                                               Binary_predicate );
    
    */

    /////////////////////////////////////////////////////////////////////////
    // Permutations
    /////////////////////////////////////////////////////////////////////////

    /*
        
    bool              next_permutation( Container& );
    bool              next_premutation( Container&, Compare );
    
    bool              prev_permutation( Container& );
    bool              prev_premutation( Container&, Compare );
    
    */

    /////////////////////////////////////////////////////////////////////////
    // Generalized Numeric Algorithms
    /////////////////////////////////////////////////////////////////////////

    /* 
    
    T                 accumulate( const Container&, T );
    T                 accumulate( const Container&, T, Binary_function );
     
    T                 inner_product( const Container1&, const Container2&,
                                      T );
    T                 inner_product( const Container1&, const Container2&, 
                                     T, Binary_function1, Binary_function2 );
    
    iterator          partial_sum( const Container1&, Container2& );
    iterator          partial_sum( const Container1&, Container2&, 
                                   Binary_function );
                                   
    iterator          adjacent_difference( const Container1&, Container2& );
    iterator          adjacent_difference( const Container1&, Container2&,
                                           Binary_function );                                           
    */

    /////////////////////////////////////////////////////////////////////////
    // Boost algorithm extensions
    /////////////////////////////////////////////////////////////////////////

    /*
    
    void              iota( Container, const T& );
    
    bool              contains( const Container&, const T& );
    
    bool              all( const Container&, Predicate );
    
    bool              none( const Container&, Predicate );
    
    bool              any_if( const Container&, Predicate );
    
    bool              is_sorted( const Container& );
    
    */

    /////////////////////////////////////////////////////////////////////////
    // Implementation
    /////////////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////////////
    // Macros
    /////////////////////////////////////////////////////////////////////////

    
#define CHECK_UNARY_FUNCTION( Fun, Container )               
#define CHECK_BINARY_PREDICATE( Pred, Container1, Container2 )
#define CHECK_EQAULITY_COMPARABLE( T )

    ///////////////////////////////////////////////////////////////////////////
    // typedef to ease returning of pairs
    ///////////////////////////////////////////////////////////////////////////

    template< typename C1, typename C2 >
    struct pair_return
    {
        typedef typename std::pair
	< 
	    typename container_traits<C1>::iterator,
	    typename container_traits<C2>::iterator 
	> 
	mutable_pair;

        typedef typename std::pair
	<
	    typename container_traits<C1>::iterator,
	    typename container_traits<C2>::const_iterator 
	>                                          
	mutable_const_pair;

        typedef typename std::pair
	< 
	    typename container_traits<C1>::const_iterator,
	    typename container_traits<C2>::iterator 
	>                                         
	const_mutable_pair;

        typedef typename std::pair
	< 
	    typename container_traits<C1>::const_iterator,
	    typename container_traits<C2>::const_iterator 
	>                                       
	const_pair;

	typedef typename std::pair
	<
	    typename container_traits<C1>::result_iterator,
	    typename container_traits<C2>::result_iterator
	>
	result_pair;

    }; // 'pair_return'

    /////////////////////////////////////////////////////////////////////////
    // Nonmodifying Sequence Operations
    /////////////////////////////////////////////////////////////////////////

    //
    // @note: for compilers without ordering of template functions
    //        we need to use a different approach which will remove
    //        the possibility to pass r-values as arguments.
    //

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING

    template< typename Container, typename Unary_function >
    inline Unary_function 
    for_each( Container& c, Unary_function fun )
    {
        return std::for_each( begin( c ), end( c ), fun );
    }

    template< typename Container, typename Unary_function >
    inline Unary_function 
    for_each( const Container& c, Unary_function fun )
    {
        return std::for_each( begin( c ), end( c ), fun );
    }



    template< typename Container, typename T >
    inline typename container_traits<Container>::iterator 
    find( Container& c, const T& value )
    {
        return std::find( begin( c ), end( c ), value );
    }

    template< typename Container, typename T >
    inline typename container_traits<Container>::const_iterator 
    find( const Container& c, const T& value )
    {
        return std::find( begin( c ), end( c ), value );
    }



    template< typename Container, typename Predicate >
    inline typename container_traits<Container>::iterator 
    find_if( Container& c, Predicate pred )
    {
        return std::find_if( begin( c ), end( c ), pred );
    }

    template< typename Container, typename Predicate >
    inline typename container_traits<Container>::const_iterator
    find_if( const Container& c, Predicate pred )
    {
        return std::find_if( begin( c ), end( c ), pred );
    }   


    namespace ext
    {

	template< typename Container >
	inline typename container_traits<Container>::iterator
	adjacent_find( Container& c )
	{
	    return std::adjacent_find( begin( c ), end( c ) );
	}

	template< typename Container >
	inline typename container_traits<Container>::const_iterator
	adjacent_find( const Container& c )
	{
	    return std::adjacent_find( begin( c ), end( c ) );
	}

	template< typename Container, typename Binary_predicate >
	inline typename container_traits<Container>::iterator 
	adjacent_find( Container& c, Binary_predicate pred )
	{
	    return std::adjacent_find( begin( c ), end( c ), pred );
	}

	template< typename Container, typename Binary_predicate >
	inline typename container_traits<Container>::const_iterator 
	adjacent_find( const Container& c, Binary_predicate pred )
	{
	    return std::adjacent_find( begin( c ), end( c ), pred );
	}

    } // namespace 'ext'




    template< typename Container1, typename Container2 >
    inline typename container_traits<Container1>::iterator 
    find_first_of( Container1& c1, const Container2& c2 )
    {
        return std::find_first_of( begin( c1 ), end( c1 ), 
				   begin( c2 ), end( c2 ) );
    }

    template< typename Container1, typename Container2 >
    inline typename container_traits<Container1>::const_iterator 
    find_first_of( const Container1& c1, const Container2& c2 )
    {
        return std::find_first_of( begin( c1 ), end( c1 ), 
				   begin( c2 ), end( c2 ) );
    }

    template< typename Container1, typename Container2, 
    typename Binary_predicate >
    inline typename container_traits<Container1>::iterator 
    find_first_of( Container1& c1, const Container2& c2, 
                   Binary_predicate pred )
    {
        return std::find_first_of( begin( c1 ), end( c1 ), 
				   begin( c2 ), end( c2 ), pred );
    }

    template< typename Container1, typename Container2, 
    typename Binary_predicate >
    inline typename container_traits<Container1>::const_iterator 
    find_first_of( const Container1& c1, const Container2& c2, 
                   Binary_predicate pred )
    {
        return std::find_first_of( begin( c1 ), end( c1 ), 
				   begin( c2 ), end( c2 ), pred );
    }



    template< typename Container, typename T >
    inline typename container_traits<Container>::difference_type
    count( const Container& c, const T& value )
    {
        return std::count( begin( c ), end( c ), value );
    }

    

    template< typename Container, typename Predicate >
    inline typename container_traits<Container>::difference_type
    count_if( const Container& c, Predicate pred )
    {
        return std::count_if( begin( c ), end( c ), pred );
    }


    namespace ext
    {
	template< typename Container1, typename Container2 >
	inline typename pair_return<Container1,Container2>::mutable_pair
	mismatch( Container1& c1, Container2& c2 )
	{
	    return std::mismatch( begin( c1 ), end( c1 ), begin( c2 ) );
	}
	
	template< typename Container1, typename Container2 >
	inline typename pair_return<Container1,Container2>::const_mutable_pair
	mismatch( const Container1& c1, Container2& c2 )
	{
	    return std::mismatch( begin( c1 ), end( c1 ), begin( c2 ) );
	}
	
	template< typename Container1, typename Container2 >
	inline typename pair_return<Container1,Container2>::mutable_const_pair 
	mismatch( Container1& c1, const Container2& c2 )
	{
	    return std::mismatch( begin( c1 ), end( c1 ), begin( c2 ) );
	}
	
	template< typename Container1, typename Container2 >
	inline typename pair_return<Container1,Container2>::const_pair
	mismatch( const Container1& c1, const Container2& c2 )
	{
	    return std::mismatch( begin( c1 ), end( c1 ), begin( c2 ) );
	}
	
	template< typename Container1, typename Container2, 
		  typename Binary_predicate >
	inline typename pair_return<Container1, Container2>::mutable_pair
	mismatch( Container1& c1, Container2& c2, Binary_predicate pred )
	{
	    return std::mismatch( begin( c1 ), end( c1 ), begin( c2 ), pred );
	}

	template< typename Container1, typename Container2, 
		  typename Binary_predicate >
	inline typename pair_return<Container1,Container2>::const_mutable_pair
	mismatch( const Container1& c1, Container2& c2, Binary_predicate pred )
	{
	    return std::mismatch( begin( c1 ), end( c1 ), begin( c2 ), pred );
	}
	
	template< typename Container1, typename Container2, 
		  typename Binary_predicate >
	inline typename pair_return<Container1,Container2>::mutable_const_pair
	mismatch( Container1& c1, const Container2& c2, Binary_predicate pred )
	{
	    return std::mismatch( begin( c1 ), end( c1 ), begin( c2 ), pred );
	}
	
	template< typename Container1, typename Container2, 
		  typename Binary_predicate >
	inline typename pair_return<Container1,Container2>::const_pair
	mismatch( const Container1& c1, const Container2& c2, 
		   Binary_predicate pred )
	{
	    return std::mismatch( begin( c1 ), end( c1 ), begin( c2 ), pred );
	}



	template< typename Container1, typename Container2 >
	inline bool 
	equal( const Container1& c1, const Container2& c2 )
	{
	    return std::equal( begin( c1 ), end( c1 ), begin( c2 ) );
	}
	
	template< typename Container1, typename Container2, 
		  typename Binary_predicate > 
	inline bool 
	equal( const Container1& c1, const Container2& c2, 
	       Binary_predicate pred )
	{
	    return std::equal( begin( c1 ), end( c1 ), begin( c2 ), pred );
	}
	
    } // namespace 'ext'


    template< typename Container1, typename Container2 >
    inline typename container_traits<Container1>::iterator 
    search( Container1& c1, const Container2& c2 )
    {
        return std::search( begin( c1 ), end( c1 ), begin( c2 ), end( c2 ) );
    }

    template< typename Container1, typename Container2 >
    inline typename container_traits<Container1>::const_iterator 
    search( const Container1& c1, const Container2& c2 )
    {
        return std::search( begin( c1 ), end( c1 ), begin( c2 ), end( c2 ) );
    }

    template< typename Container1, typename Container2,
    typename Binary_predicate >
    inline typename container_traits<Container1>::iterator
    search( Container1& c1, const Container2& c2, Binary_predicate pred )
    {
        return std::search( begin( c1 ), end( c1 ), 
			    begin( c2 ), end( c2 ), pred );
    }

    template< typename Container1, typename Container2,
    typename Binary_predicate >
    inline typename container_traits<Container1>::const_iterator
    search( const Container1& c1, const Container2& c2, Binary_predicate pred )
    {
        return std::search( begin( c1 ), end( c1 ), 
			    begin( c2 ), end( c2 ), pred );
    }



    namespace ext
    {

	template< typename Container, typename Integer, typename T >
	inline typename container_traits<Container>::iterator 
	search_n( Container& c, Integer count, const T& value )
	{
	    return std::search_n( begin( c ), end( c ), count, value );
	}

	template< typename Container, typename Integer, typename T >
	inline typename container_traits<Container>::const_iterator 
	search_n( const Container& c, Integer count, const T& value )
	{
	    return std::search_n( begin( c ), end( c ), count, value );
	}

	template< typename Container, typename Integer, 
		  typename T, typename Binary_predicate >
	inline typename container_traits<Container>::iterator
	search_n( Container& c, Integer count, const T& value,
		   Binary_predicate pred )
	{
	    return std::search_n( begin( c ), end( c ), count, value, pred );
	}

	template< typename Container, typename Integer, 
		  typename T, typename Binary_predicate >
	inline typename container_traits<Container>::const_iterator
	search_n( const Container& c, Integer count, const T& value,
		   Binary_predicate pred )
	{
	    return std::search_n( begin( c ), end( c ), count, value, pred );
	}

    } // namespace 'ext'


    template< typename Container1, typename Container2 >
    inline typename container_traits<Container1>::iterator 
    find_end( Container1& c1, const Container2& c2 )
    {
        return std::find_end( begin( c1 ), end( c1 ),
			      begin( c2 ), end( c2 ) ); 
    }

    template< typename Container1, typename Container2 >
    inline typename container_traits<Container1>::const_iterator 
    find_end( const Container1& c1, const Container2& c2 )
    {
        return std::find_end( begin( c1 ), end( c1 ),
			      begin( c2 ), end( c2 ) ); 
    }

    template< typename Container1, typename Container2,
    typename Binary_predicate >
    inline typename container_traits<Container1>::iterator 
    find_end( Container1& c1, const Container2& c2, Binary_predicate pred )
    {
        return std::find_end( begin( c1 ), end( c1 ), 
			      begin( c2 ), end( c2 ), pred );
    }

    template< typename Container1, typename Container2,
    typename Binary_predicate >
    inline typename container_traits<Container1>::const_iterator 
    find_end( const Container1& c1, const Container2& c2, 
              Binary_predicate pred )
    {
        return std::find_end( begin( c1 ), end( c1 ), 
			      begin( c2 ), end( c2 ), pred );
    }

#else // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

    template< typename Container, typename Unary_function >
    inline Unary_function 
    for_each( Container& c, Unary_function fun )
    {
        return std::for_each( begin( c ), end( c ), fun );
    }



    template< typename Container, typename T >
    inline typename container_traits<Container>::iterator 
    find( Container& c, const T& value )
    {
        return std::find( begin( c ), end( c ), value );
    }



    template< typename Container, typename Predicate >
    inline typename container_traits<Container>::iterator 
    find_if( Container& c, Predicate pred )
    {
        return std::find_if( begin( c ), end( c ), pred );
    }



    namespace ext
    {

	template< typename Container >
	inline typename container_traits<Container>::iterator
	adjacent_find( Container& c )
	{
	    return std::adjacent_find( begin( c ), end( c ) );
	}



	template< typename Container, typename Binary_predicate >
	inline typename container_traits<Container>::iterator 
	adjacent_find( Container& c, Binary_predicate pred )
	{
	    return std::adjacent_find( begin( c ), end( c ), pred );
	}

    } // namespace 'ext'



    template< typename Container1, typename Container2 >
    inline typename container_traits<Container1>::iterator 
    find_first_of( Container1& c1, const Container2& c2 )
    {
        return std::find_first_of( begin( c1 ), end( c1 ), 
				   begin( c2 ), end( c2 ) );
    }

    template< typename Container1, typename Container2, 
    typename Binary_predicate >
    inline typename container_traits<Container1>::iterator 
    find_first_of( Container1& c1, const Container2& c2, 
                   Binary_predicate pred )
    {
        return std::find_first_of( begin( c1 ), end( c1 ), 
				   begin( c2 ), end( c2 ), pred );
    }



    template< typename Container, typename T >
    inline typename container_traits<Container>::difference_type
    count( const Container& c, const T& value )
    {
        return std::count( begin( c ), end( c ), value );
    }

    

    template< typename Container, typename Predicate >
    inline typename container_traits<Container>::difference_type
    count_if( const Container& c, Predicate pred )
    {
        return std::count_if( begin( c ), end( c ), pred );
    }


    namespace ext
    {
	template< typename Container1, typename Container2 >
	inline typename pair_return<Container1,Container2>::result_pair
	mismatch( Container1& c1, Container2& c2 )
	{
	    return std::mismatch( begin( c1 ), end( c1 ), begin( c2 ) );
	}
		
	template< typename Container1, typename Container2, 
		  typename Binary_predicate >
	inline typename pair_return<Container1, Container2>::result_pair
	mismatch( Container1& c1, Container2& c2, Binary_predicate pred )
	{
	    return std::mismatch( begin( c1 ), end( c1 ), begin( c2 ), pred );
	}



	template< typename Container1, typename Container2 >
	inline bool 
	equal( const Container1& c1, const Container2& c2 )
	{
	    return std::equal( begin( c1 ), end( c1 ), begin( c2 ) );
	}
	
	template< typename Container1, typename Container2, 
		  typename Binary_predicate > 
	inline bool 
	equal( const Container1& c1, const Container2& c2, 
	       Binary_predicate pred )
	{
	    return std::equal( begin( c1 ), end( c1 ), begin( c2 ), pred );
	}
	
    } // namespace 'ext'


    template< typename Container1, typename Container2 >
    inline typename container_traits<Container1>::iterator 
    search( Container1& c1, const Container2& c2 )
    {
        return std::search( begin( c1 ), end( c1 ), begin( c2 ), end( c2 ) );
    }

    template< typename Container1, typename Container2,
    typename Binary_predicate >
    inline typename container_traits<Container1>::iterator
    search( Container1& c1, const Container2& c2, Binary_predicate pred )
    {
        return std::search( begin( c1 ), end( c1 ), 
			    begin( c2 ), end( c2 ), pred );
    }



    namespace ext
    {

	template< typename Container, typename Integer, typename T >
	inline typename container_traits<Container>::iterator 
	search_n( Container& c, Integer count, const T& value )
	{
	    return std::search_n( begin( c ), end( c ), count, value );
	}

	template< typename Container, typename Integer, 
		  typename T, typename Binary_predicate >
	inline typename container_traits<Container>::iterator
	search_n( Container& c, Integer count, const T& value,
		   Binary_predicate pred )
	{
	    return std::search_n( begin( c ), end( c ), count, value, pred );
	}

    } // namespace 'ext'


    template< typename Container1, typename Container2 >
    inline typename container_traits<Container1>::iterator 
    find_end( Container1& c1, const Container2& c2 )
    {
        return std::find_end( begin( c1 ), end( c1 ),
			      begin( c2 ), end( c2 ) ); 
    }

    template< typename Container1, typename Container2,
    typename Binary_predicate >
    inline typename container_traits<Container1>::iterator 
    find_end( Container1& c1, const Container2& c2, Binary_predicate pred )
    {
        return std::find_end( begin( c1 ), end( c1 ), 
			      begin( c2 ), end( c2 ), pred );
    }

#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING


    /////////////////////////////////////////////////////////////////////////
    // Modifying Sequance Operations
    /////////////////////////////////////////////////////////////////////////

    template< typename Container1, typename Container2 >
    inline typename container_traits<Container2>::iterator
    copy( const Container1& c1, Container2& c2 )
    {
        return std::copy( begin( c1 ), end( c1 ), begin( c2 ) );
    }



    template< typename Container1, typename Container2 > 
    //BidirectionalIterator> inline BidirectionalIterator
    inline typename container_traits<Container2>::iterator
    copy_backward( const Container1& c1, Container2& c2 ) 
    {
        return std::copy_backward( begin( c1 ), end( c1 ), end( c2 ) ); 
    }


#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING

    template< typename Container1, typename Container2 >
    inline typename container_traits<Container2>::iterator 
    swap_ranges( Container1& c1, Container2& c2 ) //ForwardIterator out);
    {
        return std::swap_ranges( begin( c1 ), end( c1 ), begin( c2 ) );
    }

#else


    template< typename Container1, typename Container2 >
    inline typename container_traits<Container2>::iterator 
    swap_ranges( Container1& c1, Container2& c2 ) //ForwardIterator out);
    {
        return std::swap_ranges( begin( c1 ), end( c1 ), begin( c2 ) );
    }

#endif

    template< typename Container1, typename Container2, 
    typename Unary_function > 
    inline typename container_traits<Container2>::iterator
    transform( const Container1& c1, Container2& c2, Unary_function fun )
    {
        return std::transform( begin( c1 ), end( c1 ), begin( c2 ), fun );
    }

    template< typename Container1, typename Container2,
    typename Container3, typename Binary_function >
    inline typename container_traits<Container3>::iterator
    transform_( const Container1& c1, const Container2& c2, Container3& c3,   
                Binary_function fun )
    {
        return std::transform( begin( c1 ), end( c1 ), begin( c2 ), 
                               begin( c3 ), fun ); 
    }



    template< typename Container, typename T >
    inline void
    replace( Container& c, const T& what, const T& with_what )
    {
        return std::replace( begin( c ), end( c ), what, with_what );
    }



    template< typename Container, typename Predicate , typename T >
    inline void
    replace_if( Container& c, Predicate pred, const T& value )
    {
        return std::replace_if( begin( c ), end( c ), pred, value );
    }



    template< typename Container1, typename Container2, // OutputIterator, 
    typename T > 
    inline typename container_traits<Container2>::iterator
    replace_copy( const Container1& c1, Container2& c2, 
                  const T& what, const T& with_what )
    {
        return std::replace_copy( begin( c1 ), end( c1 ), begin( c2 ),
                                  what, with_what );
    }



    template< typename Container1, typename Container2, //OutputIterator, 
    typename Predicate, typename T >
    inline typename container_traits<Container2>::iterator
    replace_copy_if( const Container1& c1, Container2& c2,
                     Predicate pred, const T& value)
    {
        return std::replace_copy_if( begin( c1 ), end( c1 ), 
                                     begin( c2 ), pred, value );     
    }



    template< typename Container, typename T >
    inline void
    fill( Container& c, const T& value )
    {
        std::fill( begin( c ), end( c ), value );
    }



    template< typename Container, typename Integer, typename T >
    inline void
    fill_n_( Container& c, Integer size, const T& value )
    {
        std::fill_n( begin( c ), size, value );
    }



    template< typename Container, typename Generator >
    inline void
    generate( Container& c, Generator gen )
    {
        std::generate( begin( c ), end( c ), gen );
    }



    namespace ext
    {

	template< typename Container, typename Integer, typename Generator >
	void
	generate_n( Container& c, Integer size, Generator gen )
	{
	    std::generate_n( begin( c ), size, gen );
	}
	
    } // namespace 'ext'



    template< typename Container, typename T >
    inline typename container_traits<Container>::iterator
    remove( Container& c, const T& what )
    {
        return std::remove( begin( c ), end( c ), what );
    }



    template< typename Container, typename Predicate >
    inline typename container_traits<Container>::iterator
    remove_if( Container& c, Predicate pred )
    {
        return std::remove_if( begin( c ), end( c ), pred );
    }



    template< typename Container1, typename Container2, typename T >
    inline typename container_traits<Container2>::iterator
    remove_copy( const Container1& c1, Container2& c2, const T& value )
    {
        return std::remove_copy( begin( c1 ), end( c1 ), begin( c2 ), value );
    }



    template< typename Container1, typename Container2, typename Predicate >
    inline typename container_traits<Container2>::iterator
    remove_copy_if( const Container1& c1, Container2& c2, Predicate pred )
    {
        return std::remove_copy_if( begin( c1 ), end( c1 ), 
                                    begin( c2 ), pred );
    }



    template< typename Container >
    inline typename container_traits<Container>::iterator
    unique( Container& c )
    {
        return std::unique( begin( c ), end( c ) );
    }

    template< typename Container, typename Binary_predicate >
    inline typename container_traits<Container>::iterator
    unique_( Container& c, Binary_predicate pred )
    {
        return std::unique( begin( c ), end( c ), pred );
    }



    template< typename Container1, typename Container2 >
    inline typename container_traits<Container2>::iterator
    unique_copy( const Container1& c1, Container2& c2 )
    {
        return std::unique_copy( begin( c1 ), end( c1 ), begin( c2 ) );
    }

    template< typename Container1, typename Container2, 
    typename Binary_predicate >
    inline typename container_traits<Container2>::iterator
    unique_copy_( const Container1& c1, Container2& c2, Binary_predicate pred )
    {
        return std::unique_copy( begin( c1 ), end( c1 ), begin( c2 ), pred );
    }



    template< typename Container >
    inline void
    reverse( Container& c )
    {
        std::reverse( begin( c ), end( c ) );
    }



    template< typename Container1, typename Container2 >
    inline typename container_traits<Container2>::iterator
    reverse_copy( const Container1& c1, Container2& c2 )
    {
        return std::reverse_copy( begin( c1 ), end( c1 ), begin( c2 ) );
    }



    template< typename Container > 
    inline void
    rotate( Container& c, 
	    typename container_traits<Container>::iterator middle )
    {
        std::rotate( begin( c ), middle, end( c ) );
    }



    template< typename Container1, typename Container2 >
    inline typename container_traits<Container2>::iterator
    rotate_copy( Container1& c1, 
                 typename container_traits<Container2>::iterator middle, 
                 Container2& c2 ) 
    {
        return std::rotate_copy( begin( c1 ), middle, end( c1 ), begin( c2 ) );
    }

    template< typename Container1, typename Container2 >
    inline typename container_traits<Container2>::const_iterator
    rotate_copy( const Container1& c1, 
                 typename container_traits<Container2>::const_iterator middle, 
                 Container2& c2 ) 
    {
        return std::rotate_copy( begin( c1 ), middle, end( c1 ), begin( c2 ) );
    }



    template< typename Container >
    inline void
    random_shuffle( Container& c )
    {
        std::random_shuffle( begin( c ), end( c ) );
    }

    template< typename Container, typename Generator >
    inline void
    random_shuffle( Container& c, Generator gen )
    {
        std::random_shuffle( begin( c ), end( c ), gen );
    }

    /////////////////////////////////////////////////////////////////////////
    // Sorted Sequences
    /////////////////////////////////////////////////////////////////////////

    namespace ext
    {
	template< typename Container >
	inline void
	sort( Container& c )
	{
	    std::sort( begin( c ), end( c ) );
	}
	
	template< typename Container, typename Compare >
	inline void
	sort( Container& c, Compare comp )
	{
	    std::sort( begin( c ), end( c ), comp );
	}
	
	
	
	template< typename Container >
	inline void
	stable_sort( Container& c )
	{
	    std::stable_sort( begin( c ), end( c) );
	}
	
	template< typename Container, typename Compare >
	inline void
	stable_sort( Container& c, Compare comp )
	{
	    std::stable_sort( begin( c ), end( c ), comp );
	}

    } // namespace 'ext'


    template< typename Container >
    inline void
    partial_sort( Container& c, 
                  typename container_traits<Container>::iterator middle )
    {
        std::partial_sort( begin( c ), middle, end( c ) );
    }

    template< typename Container, typename Compare >
    inline void
    partial_sort_( Container& c, 
                   typename container_traits<Container>::iterator middle,
                   Compare comp )
    {
        std::partial_sort( begin( c ), middle, end( c ), comp );
    }



    template< typename Container1, typename Container2 >
    inline typename container_traits<Container2>::iterator
    partial_sort_copy( const Container1& c1, Container2& c2 )
    {
        return std::partial_sort_copy( begin( c1 ), end( c1 ), 
                                       begin( c2 ), end( c2 ) );
    }

    template< typename Container1, typename Container2, typename Compare >
    inline typename container_traits<Container2>::iterator
    partial_sort_copy( const Container1& c1, Container2& c2, Compare comp )
    {
        return std::partial_sort_copy( begin( c1 ), end( c1 ), 
                                       begin( c2 ), end( c2 ), comp ); 
    }



    template< typename Container >
    inline void
    nth_element( Container& c, 
                 typename container_traits<Container>::iterator nth )
    {
        std::nth_element( begin( c ), nth, end( c ) );
    }

    template< typename Container, typename Compare >
    inline void
    nth_element_( Container& c, 
                  typename container_traits<Container>::iterator nth, 
                  Compare comp )
    {
        std::nth_element( begin( c ), nth, end( c ), comp );
    }



#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING

    template< typename Container, typename T >
    inline typename container_traits<Container>::iterator
    lower_bound( Container& c, const T& value )
    {
        return std::lower_bound( begin( c ), end( c ), value );
    }

    template< typename Container, typename T >
    inline typename container_traits<Container>::const_iterator
    lower_bound( const Container& c, const T& value )
    {
        return std::lower_bound( begin( c ), end( c ), value );
    }

    template< typename Container, typename T, typename Compare >
    inline typename container_traits<Container>::iterator
    lower_bound_( Container& c, const T& value, Compare comp )
    {
        return std::lower_bound( begin( c ), end( c ), value, comp );
    }

    template< typename Container, typename T, typename Compare >
    inline typename container_traits<Container>::const_iterator
    lower_bound_( const Container& c, const T& value, Compare comp )
    {
        return std::lower_bound( begin( c ), end( c ), value, comp );
    }



    template< typename Container, typename T >
    inline typename container_traits<Container>::iterator
    upper_bound( Container& c, const T& value )
    {
        return std::upper_bound( begin( c ), end( c ), value );
    }

    template< typename Container, typename T >
    inline typename container_traits<Container>::const_iterator
    upper_bound( const Container& c, const T& value )
    {
        return std::upper_bound( begin( c ), end( c ), value );
    }

    template< typename Container, typename T, typename Compare >
    inline typename container_traits<Container>::iterator
    upper_bound_( Container& c, const T& value, Compare comp )
    {
        return std::upper_bound( begin( c ), end( c ), value, comp );
    }

    template< typename Container, typename T, typename Compare >
    inline typename container_traits<Container>::const_iterator
    upper_bound_( const Container& c, const T& value, Compare comp )
    {
        return std::upper_bound( begin( c ), end( c ), value, comp );
    }



    template< typename Container, typename T >
    inline typename pair_return<Container,Container>::mutable_pair 
    equal_range( Container& c, const T& value )
    {
        return std::equal_range( begin( c ), end( c ), value );
    }

    template< typename Container, typename T >
    inline typename pair_return<Container,Container>::const_pair 
    equal_range( const Container& c, const T& value )
    {
        return std::equal_range( begin( c ), end( c ), value );
    }

    template< typename Container, typename T, typename Compare >
    inline typename pair_return<Container,Container>::mutable_pair
    equal_range_( Container& c, const T& value, Compare comp )
    {
        return std::equal_range( begin( c ), end( c ), value, comp );
    }

    template< typename Container, typename T, typename Compare >
    inline typename pair_return<Container,Container>::const_pair
    equal_range_( const Container& c, const T& value, Compare comp )
    {
        return std::equal_range( begin( c ), end( c ), value, comp );
    }

#else // BOOST_NO_FUNCTION_TEMPLATE_ORDERING


    template< typename Container, typename T >
    inline typename container_traits<Container>::iterator
    lower_bound( Container& c, const T& value )
    {
        return std::lower_bound( begin( c ), end( c ), value );
    }

    template< typename Container, typename T, typename Compare >
    inline typename container_traits<Container>::iterator
    lower_bound_( Container& c, const T& value, Compare comp )
    {
        return std::lower_bound( begin( c ), end( c ), value, comp );
    }



    template< typename Container, typename T >
    inline typename container_traits<Container>::iterator
    upper_bound( Container& c, const T& value )
    {
        return std::upper_bound( begin( c ), end( c ), value );
    }

    template< typename Container, typename T, typename Compare >
    inline typename container_traits<Container>::iterator
    upper_bound_( Container& c, const T& value, Compare comp )
    {
        return std::upper_bound( begin( c ), end( c ), value, comp );
    }



    template< typename Container, typename T >
    inline typename pair_return<Container,Container>::result_pair 
    equal_range( Container& c, const T& value )
    {
        return std::equal_range( begin( c ), end( c ), value );
    }

    template< typename Container, typename T, typename Compare >
    inline typename pair_return<Container,Container>::result_pair
    equal_range_( Container& c, const T& value, Compare comp )
    {
        return std::equal_range( begin( c ), end( c ), value, comp );
    }

#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING



    template< typename Container, typename T >
    inline bool
    binary_search( const Container& c, const T& value )
    {
        return std::binary_search( begin( c ), end( c ), value );
    }

    template< typename Container, typename T, typename Compare >
    inline bool
    binary_search_( const Container& c, const T& value, Compare comp )
    {
        return std::binary_search( begin( c ), end( c ), value, comp );
    }



    template< typename Container1, typename Container2, typename Container3 > 
    inline typename container_traits<Container3>::iterator
    merge( const Container1& c1, const Container2& c2, Container3& c3 )
    {
        return std::merge( begin( c1 ), end( c1 ), begin( c2 ), 
                           end( c2 ), begin( c3 ) );  
    }

    template< typename Container1, typename Container2, typename Container3, 
    typename Compare >
    inline typename container_traits<Container3>::iterator
    merge( const Container1& c1, const Container2& c2, Container3& c3,
           Compare comp )
    {
        return std::merge( begin( c1 ), end( c1 ), begin( c2 ), 
                           end( c2 ), begin( c3 ), comp ); 
    }



    template< typename Container >
    inline void
    inplace_merge( Container& c, 
                   typename container_traits<Container>::iterator middle )
    {
        std::inplace_merge( begin( c ), middle, end( c ) );
    }

    template< typename Container, typename Compare >
    inline void
    inplace_merge_( Container& c,
                    typename container_traits<Container>::iterator middle, 
                    Compare comp )
    {
        std::inplace_merge( begin( c ), middle, end( c ), comp );
    }



    template< typename Container, typename Predicate >
    inline typename container_traits<Container>::iterator
    partition( Container& c, Predicate pred )
    {
        return std::partition( begin( c ), end( c ), pred );
    }



    template< typename Container, typename Predicate >
    inline typename container_traits<Container>::iterator
    stable_partition( Container& c, Predicate pred )
    {
        return std::stable_partition( begin( c ), end( c ), pred );
    }

    /////////////////////////////////////////////////////////////////////////
    // Set Algorithms
    /////////////////////////////////////////////////////////////////////////

    template< typename Container1, typename Container2 >
    inline bool 
    includes( const Container1& c1, const Container2& c2 )
    {
        return std::includes( begin( c1 ), end( c1 ),
                              begin( c2 ), end( c2 ) );
    }

    template< typename Container1, typename Container2, typename Compare >
    inline bool 
    includes( const Container1& c1, const Container2& c2, Compare comp )
    {
        return std::includes( begin( c1 ), end( c1 ),
                              begin( c2 ), end( c2 ), comp );
    }



    template< typename Container1, typename Container2, typename Container3 > 
    inline typename container_traits<Container3>::iterator
    set_union( const Container1& c1, const Container2& c2, Container3& c3 )
    {
        return std::set_union( begin( c1 ), end( c1 ), begin( c2 ), end( c2 ),
                               begin( c3 ) );
    }

    template<typename Container1, typename Container2, 
    typename Container3, typename Compare >
    inline typename container_traits<Container3>::iterator
    set_union( const Container1& c1, const Container2& c2,
               Container3 c3, Compare comp )
    {
        return std::set_union( begin( c1 ), end( c1 ), begin( c2 ), end( c2 ),
                               begin( c3 ), comp );
    }



    template< typename Container1, typename Container2, 
    typename Container3 > 
    typename container_traits<Container3>::iterator
    set_intersection( const Container1& c1, const Container2& c2, 
                      Container3& c3 )
    {
        return std::set_intersection( begin( c1 ), end( c1 ), 
                                      begin( c1 ), end( c1 ),
                                      begin( c3 ) ); 
    }

    template< typename Container1, typename Container2, 
    typename Container3, typename Compare > 
    typename container_traits<Container3>::iterator
    set_intersection( const Container1& c1,
                      const Container2& c2, Container3& c3,
                      Compare comp )
    {
        return std::set_intersection( begin( c1 ), end( c1 ), 
                                      begin( c1 ), end( c1 ),
                                      begin( c3 ), comp ); 
    }



    template< typename Container1, typename Container2, typename 
    Container3 >
    inline typename container_traits<Container3>::iterator
    set_difference( const Container1& c1, const Container2& c2,  
                    Container3& c3 )
    {
        return std::set_difference( begin( c1 ), end( c1 ), begin( c2 ), 
                                    end( c2 ), begin( c3 ) );
    }  

    template< typename Container1, typename Container2, typename 
    Container3, typename Compare >
    inline typename container_traits<Container3>::iterator
    set_difference( const Container1& c1, const Container2& c2,  
                    Container3& c3, Compare comp )
    {
        return std::set_difference( begin( c1 ), end( c1 ), begin( c2 ), 
                                    end( c2 ), begin( c3 ), comp );
    }




    template< typename Container1, typename Container2, typename Container3 >
    inline typename container_traits<Container3>::iterator
    set_symmetric_difference( const Container1& c1, const Container2& c2,
                              Container3& c3 )
    {
        return std::set_symmetric_difference( begin( c1 ), end( c1 ),
                                              begin( c2 ), end( c2 ), 
                                              begin( c3 ) );
    }


    template< typename Container1, typename Container2, 
    typename Container3, typename Compare >
    inline typename container_traits<Container3>::iterator
    set_symmetric_difference( const Container1& c1, const Container2& c2,
                              Container3& c3, Compare comp )
    {
        return std::set_symmetric_difference( begin( c1 ), end( c1 ),
                                              begin( c2 ), end( c2 ), 
                                              begin( c3 ), comp );
    }

    ///////////////////////////////////////////////////////////////////////////
    // Heap Operations
    ///////////////////////////////////////////////////////////////////////////

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING

    namespace ext
    {
	template< typename Container >
	inline void 
	push_heap( Container& c )
	{
	    std::push_heap( begin( c ), end( c ) );
	}
	
	template< typename Container, typename Compare >
	inline void
	push_heap( Container& c, Compare comp )
	{
	    std::push_heap( begin( c ), end( c ), comp );
	}
	

	
	template< typename Container >
	inline void 
	pop_heap( Container& c )
	{
	    std::pop_heap( begin( c ), end( c ) );
	}
	
	template< typename Container, typename Compare >
	inline void
	pop_heap( Container& c, Compare comp )
	{
	    std::pop_heap( begin( c ), end( c ), comp );
	}
	


	template< typename Container >
	inline void 
	make_heap( Container& c )
	{
	    std::make_heap( begin( c ), end( c ) );
	}
	
	template< typename Container, typename Compare >
	inline void
	make_heap( Container& c, Compare comp )
	{
	    std::make_heap( begin( c ), end( c ), comp );
	}



	template< typename Container >
	inline void 
	sort_heap( Container& c )
	{
	    std::sort_heap( begin( c ), end( c ) );
	}
	
	template< typename Container, typename Compare >
	inline void
	sort_heap( Container& c, Compare comp )
	{
        std::sort_heap( begin( c ), end( c ), comp );
	}

    } // namespace 'ext'

    /////////////////////////////////////////////////////////////////////////
    // Minimum and Maximum
    /////////////////////////////////////////////////////////////////////////

    template< typename Container >
    inline typename container_traits<Container>::iterator
    min_element( Container& c )
    {
        return std::min_element( begin( c ), end( c ) );
    }

    template< typename Container >
    inline typename container_traits<Container>::const_iterator
    min_element( const Container& c )
    {
        return std::min_element( begin( c ),end( c ) );
    }

    template< typename Container, typename Binary_predicate >
    inline typename container_traits<Container>::iterator
    min_element_( Container& c, Binary_predicate pred )
    {
        return std::min_element( begin( c ), end( c ), pred );
    }

    template< typename Container, typename Binary_predicate >
    inline typename container_traits<Container>::const_iterator
    min_element_( const Container& c, Binary_predicate pred )
    {
        return std::min_element( begin( c ), end( c ), pred );
    }



    template< typename Container >
    inline typename container_traits<Container>::iterator
    max_element( Container& c )
    {
        return std::max_element( begin( c ), end( c ) );
    }

    template< typename Container >
    inline typename container_traits<Container>::const_iterator
    max_element( const Container& c )
    {
        return std::max_element( begin( c ), end( c ) );
    }

    template< typename Container, typename Binary_predicate >
    inline typename container_traits<Container>::iterator
    max_element_( Container& c, Binary_predicate pred )
    {
        return std::max_element( begin( c ), end( c ), pred );
    }

    template< typename Container, typename Binary_predicate >
    inline typename container_traits<Container>::const_iterator
    max_element_( const Container& c, Binary_predicate pred )
    {
        return std::max_element( begin( c ), end( c ), pred );
    }

#else // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

    namespace ext
    {
	template< typename Container >
	inline void 
	push_heap( Container& c )
	{
	    std::push_heap( begin( c ), end( c ) );
	}
	
	template< typename Container, typename Compare >
	inline void
	push_heap( Container& c, Compare comp )
	{
	    std::push_heap( begin( c ), end( c ), comp );
	}
	
	
	
	template< typename Container >
	inline void 
	pop_heap( Container& c )
	{
	    std::pop_heap( begin( c ), end( c ) );
	}
	
	template< typename Container, typename Compare >
	inline void
	pop_heap( Container& c, Compare comp )
	{
	    std::pop_heap( begin( c ), end( c ), comp );
	}
	

	template< typename Container >
	inline void 
	make_heap( Container& c )
	{
	    std::make_heap( begin( c ), end( c ) );
	}
	
	template< typename Container, typename Compare >
	inline void
	make_heap( Container& c, Compare comp )
	{
	    std::make_heap( begin( c ), end( c ), comp );
	}



	template< typename Container >
	inline void 
	sort_heap( Container& c )
	{
	    std::sort_heap( begin( c ), end( c ) );
	}
	
	template< typename Container, typename Compare >
	inline void
	sort_heap( Container& c, Compare comp )
	{
	    std::sort_heap( begin( c ), end( c ), comp );
	}

    } // namespace 'ext'

    /////////////////////////////////////////////////////////////////////////
    // Minimum and Maximum
    /////////////////////////////////////////////////////////////////////////

    template< typename Container >
    inline typename container_traits<Container>::iterator
    min_element( Container& c )
    {
        return std::min_element( begin( c ), end( c ) );
    }

    template< typename Container, typename Binary_predicate >
    inline typename container_traits<Container>::iterator
    min_element_( Container& c, Binary_predicate pred )
    {
        return std::min_element( begin( c ), end( c ), pred );
    }



    template< typename Container >
    inline typename container_traits<Container>::iterator
    max_element( Container& c )
    {
        return std::max_element( begin( c ), end( c ) );
    }

    template< typename Container, typename Binary_predicate >
    inline typename container_traits<Container>::iterator
    max_element_( Container& c, Binary_predicate pred )
    {
        return std::max_element( begin( c ), end( c ), pred );
    }

#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING



    template< typename Container1, typename Container2 >
    inline bool
    lexicographical_compare( const Container1& c1, const Container2& c2 )
    {
        return std::lexicographical_compare( begin( c1 ), end( c1 ), 
                                             begin( c2 ), end( c2 ) );
    }

    template< typename Container1, typename Container2, 
    typename Binary_predicate > 
    inline bool
    lexicographical_compare( const Container1& c1, const Container2& c2, 
                             Binary_predicate pred )
    {
        return std::lexicographical_compare( begin( c1 ), end( c1 ), 
                                             begin( c2 ), end( c2 ), pred );
    }

    /////////////////////////////////////////////////////////////////////////
    // Permutations
    /////////////////////////////////////////////////////////////////////////

    namespace ext
    {
	template< typename Container >
	inline bool
	next_permutation( Container& c )
	{
	    return std::next_permutation( begin( c ), end( c ) );
	}
	
	template< typename Container, typename Compare >
	inline bool
	next_permutation( Container& c, Compare comp )
	{
	    return std::next_permutation( begin( c ), end( c ), comp );
	}
	
	
	
	template< typename Container >
	inline bool
	prev_permutation( Container& c )
	{
	    return std::prev_permutation( begin( c ), end( c ) );
	}
	
	template< typename Container, typename Compare >
	inline bool
	prev_permutation( Container& c, Compare comp )
	{
	    return std::prev_permutation( begin( c ), end( c ), comp );
	}

    } // namespace 'ext'

    /////////////////////////////////////////////////////////////////////////
    // Generalized Numeric Algorithms
    /////////////////////////////////////////////////////////////////////////

    template< typename Container, typename T >
    inline T
    accumulate( const Container& c, T initial )
    {
        return std::accumulate( begin( c ), end( c ), initial ); 
    }

    template< typename Container, typename T, typename Binary_function >
    inline T
    accumulate_( const Container& c, const T initial, Binary_function fun )
    {
        return std::accumulate( begin( c ), end( c ), initial, fun );
    }



    template< typename Container1, typename Container2, typename T >
    inline T                            
    inner_product( const Container1& c1, const Container2& c2,
                   const T initial )
    {
        return std::inner_product( begin( c1 ), end( c1 ), 
                                   begin( c2 ), initial );
    }

    template< typename Container1, typename Container2, typename T,
    typename Binary_function1, typename Binary_function2 >
    inline T                            
    inner_product( const Container1& c1, const Container2& c2,
                   const T initial, Binary_function1 fun1, 
                   Binary_function2 fun2 )
    {
        return std::inner_product( begin( c1 ), end( c1 ), 
                                   begin( c2 ), initial, fun1, fun2 );
    }



    template< typename Container1, typename Container2 >
    inline typename container_traits<Container2>::iterator
    partial_sum( const Container1& c, Container2& out )
    {
        return std::partial_sum( begin( c ), end( c ), begin( out ) );
    }

    template< typename Container1, typename Container2, 
    typename Binary_function > 
    inline typename container_traits<Container2>::iterator
    partial_sum_( const Container1& c, Container2& out, Binary_function fun )
    {
        return std::partial_sum( begin( c ), end( c ), begin( out ), fun );
    }



    template< typename Container1, typename Container2 >
    inline typename container_traits<Container2>::iterator
    adjacent_difference( const Container1& c, Container2& out )
    {
        return std::adjacent_difference( begin( c ), end( c ), begin( out ) );
    }


    template<typename Container1, typename Container2, typename Predicate >
    inline typename container_traits<Container2>::iterator
    adjacent_difference_( const Container1& c, Container2& out, 
                          Predicate pred )
    {
        return std::adjacent_difference( begin( c ), end( c ), 
                                         begin( out ), pred ); 
    }

    /////////////////////////////////////////////////////////////////////////
    // Boost algorithm extensions
    /////////////////////////////////////////////////////////////////////////

    /*
    template< typename Container, typename T >
    void 
    iota( Container& c, const T& value )
    {
        iota( begin( c ), end( c ), value );
    }



    template< typename Container, typename T >
    bool 
    contains( const Container& c, const T& value )
    {
        return contains( begin( c ), end( c ), value );
    }



    template< typename Container, typename Predicate >
    bool all( const Container& c, Predicate pred )
    {
        return all( begin( c ), end( c ), pred );
    }



    template< typename Container, typename Predicate >
    bool 
    none( const Container& c, Predicate pred )
    {
        return none( begin( c ), end( c ), pred );
    }



    template< typename Container, typename Predicate >
    bool 
    any_if( const Container& c, Predicate pred )
    {
        return any_if( begin( c ), end( c ), pred );
    }



    template< typename Container >
    bool 
    is_sorted( const Container& c )
    {
        return is_sorted( begin( c ), end( c ) );
    }

    template< typename Container, typename Compare >
    bool
    is_sorted( const Container& c, Compare comp )
    {
        return is_sorted( begin( c ), end( c ), comp );
    }
*/

}

// namespace boost

#endif // BOOST_CONTAINER_ALGORITHM_HPP

