// (C) Copyright Jeremy Siek 2001. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// (C) Copyright Thorsten Ottosen 2002. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// Mutating algorithms taken from Vladimir Prus <ghost@cs.msu.su>
// code from Boost Wiki

// Problem: should member functions be called automatically? Or should the user 
// know that it is better to call map::find() than find( map )?

// Thu May 30     I strongly disagree to have two different type for
//                begin() and end() in std::pair<Iter1,Iter2>
//                -- makes no sense if they describe a range [Herve]

// Thu May 30     Proposed to put container interfaces
//                into separate file for <algorithm> extensions [Herve]


#ifndef BOOST_CONTAINER_ALGORITHM_HPP
#define BOOST_CONTAINER_ALGORITHM_HPP

// STL algorithm extensions 
#define BOOST_NEW_ALGO_CLASH // remove ambiguit from array traits
#include <boost/array_traits.hpp>
#include <boost/concept_check.hpp>
#include <boost/config.hpp>
#include <boost/type_traits/function_traits.hpp>
#include <boost/detail/iterator.hpp>

//#include <boost/sequence_algo/algorithm.hpp>
/**
*   make_pair( first, last );
*/
#include <algorithm>
#include <numeric>

namespace boost 
{
	///////////////////////////////////////////////////////////////////////////
	// Interface
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// Nonmodifying Sequence Operations
	///////////////////////////////////////////////////////////////////////////
	/*
	
	Unary_function    for_each( const Container&, Unary_function ); 
	iterator          find( const Container&, const T& ); 
	iterator          find_if( const Container&, Predicate );
	iterator          adjacent_find( const Container& );
	iterator          adjacent_find( const Container&, Binary_predicate );  
	iterator          find_first_of( const Container1&, const Container2& ); *
	iterator          find_first_of( const Container1&, const Container2&, 
									 Binary_predicate ); *
	difference_type   count( const Container&, const T& );
	difference_type   count_if( const Container&, Predicate );    
	pair              mismatch( const Container1&, const Container2& ); *
	pair              mismatch( const Container1&, const Container2&
								Binary_predicate ); *
	bool              equal( const Container1&, const Container2& ); * 
	bool              equal( const Container1&, const Container2&,
							 Binary_predicate ); * 
	iterator          search( const Container1&, const Container2& ); *
	iterator          search( const Container1&, const Container2&.
							  Binary_predicate ); *
	iterator          search_n( const Container&, Integer, const T& );
	iterator          search_n( const Container&, Integer, const T&, 
								Binary_predicate );          
	iterator          find_end( const Container1&, const Container3& ); *
	iterator          find_end( const Container1&, const Container3&, 
								Binary_predicate ); *
		
	*/

	///////////////////////////////////////////////////////////////////////////
	// Implementation
	///////////////////////////////////////////////////////////////////////////
#if 1
	//#ifdef  BOOST_NO_FUNCTION_TEMPLATE_ORDERING
	// We need the tags because the compiler will not know how to
	// choose between begin( pair<T1,T2>& ) and begin( Container& )...
	// Instead we rely on full class specialization which is
	// supported by most compilers

	namespace detail 
	{

		// tag classes for dispatch 
		class container_algo_pair_tag
		{
		};
		class container_algo_container_tag
		{
		};
		class container_algo_array_tag
		{
		};
		class container_algo_iter_tag
		{
		};

		template <typename Container>
		struct container_algo_make_tag
		{
			typedef container_algo_container_tag       tag;
			typedef typename Container::size_type      size_type;
			typedef typename Container::iterator       result_type;
			typedef typename Container::const_iterator const_result_type;
		};

		template <typename T>
		struct container_algo_make_tag< std::pair<T,T> >
		{
			typedef container_algo_pair_tag            tag;
			typedef typename iterator_traits<T>::difference_type size_type;
			//typedef T                                  size_type;?
			typedef T                                  result_type;
			typedef T                                  const_result_type;
		};

		template< typename T, size_t sz >
		struct container_algo_make_tag< T[sz] >
		{
			typedef container_algo_array_tag                  tag;
			typedef typename array_traits<T[sz]>::size_type   size_type;
			typedef typename array_traits<T[sz]>::iterator    result_type;
			typedef const result_type                         const_result_type; 
		};

		template< typename Iterator >
		struct container_algo_make_tag;

		// for container_tag
		template <typename Container>
		inline typename
		container_algo_make_tag<Container>::result_type
		begin(Container& c, container_algo_container_tag const&)
		{
			return c.begin();
		}

		template <typename Container>
		inline typename
		container_algo_make_tag<Container>::const_result_type
		begin(const Container& c, container_algo_container_tag const&)
		{
			return c.begin();
		}

		template <typename Container>
		inline typename
		container_algo_make_tag<Container>::result_type
		end(Container& c, container_algo_container_tag const&)
		{
			return c.end();
		}

		template <typename Container>
		inline typename        
		container_algo_make_tag<Container>::const_result_type
		end(const Container& c, container_algo_container_tag const&)
		{
			return c.end();
		}

		template <typename Container>
		inline typename
		container_algo_make_tag<Container>::size_type
		size(const Container& c, container_algo_container_tag const&)
		{
			return c.size();
		}

		// for pair_tag
		template <typename Iter1, typename Iter2>
		inline typename
		container_algo_make_tag< std::pair<Iter1,Iter2> >::result_type
		begin(const std::pair<Iter1, Iter2>& p, container_algo_pair_tag const&)
		{
			return p.first;
		}

		template <typename Iter1, typename Iter2>
		inline typename
		container_algo_make_tag< std::pair<Iter1, Iter2> >::const_result_type
		end(const std::pair<Iter1, Iter2>& p, container_algo_pair_tag const&)
		{
			return p.second;
		}

		template <typename Iter1, typename Iter2>
		inline typename
		container_algo_make_tag< std::pair<Iter1,Iter2> >::size_type
		size(const std::pair<Iter1, Iter2>& p, container_algo_pair_tag const&)
		{
			return std::distance(p.first, p.second);
		}

		// for array_tag
		template< typename T, size_t sz >
		inline typename 
		container_algo_make_tag<T[sz]>::result_type
		begin( T t[sz] )
		{
			return begin( t );
		}

		template< typename T, size_t sz >
		inline typename
		container_algo_make_tag<T[sz]>::result_type 
		begin( const T t[sz] )
		{
			return begin( t );
		}

		template< typename T, size_t sz >
		inline typename
		container_algo_make_tag<T[sz]>::result_type 
		end( T t[sz] )
		{
			return end( t );
		}

		template< typename T, size_t sz >
		inline typename
		container_algo_make_tag<T[sz]>::result_type 
		end( const T t[sz] )
		{
			return end( t );
		}

		template< typename T, size_t sz >
		inline typename
		container_algo_make_tag<T[sz]>::size_type
		size( const T t[sz] )
		{
			return size( t );
		}

	} // namespace detail

	template <typename T>
	inline typename
	detail::container_algo_make_tag<T>::result_type
	begin(T& t)
	{
		return detail::begin(t, typename detail::container_algo_make_tag<T>::tag()); 
	}

	template <typename T>
	inline typename
	detail::container_algo_make_tag<T>::const_result_type
	begin(const T& t)
	{
		return detail::begin(t, typename 
							 detail::container_algo_make_tag<T>::tag()); 
	}

	template <typename T>
	inline typename
	detail::container_algo_make_tag<T>::result_type
	end(T& t)
	{
		return detail::end(t, typename 
						   detail::container_algo_make_tag<T>::tag()); 
	}

	template <typename T>
	inline typename
	detail::container_algo_make_tag<T>::const_result_type
	end(const T& t)
	{
		return detail::end(t, typename 
						   detail::container_algo_make_tag<T>::tag());
	}

	template <typename T>
	inline typename
	detail::container_algo_make_tag<T>::size_type
	size(const T& t)
	{
		return detail::size(t, typename 
							detail::container_algo_make_tag<T>::tag()); 
	}



#else
	// The compiler will handle it fine like this

	template <typename Container>
	typename Container::iterator
	begin(Container& c)
	{
		return c.begin();
	}

	template <typename Container>
	typename Container::const_iterator
	begin(const Container& c)
	{
		return c.begin();
	}

	template <typename Container>
	typename Container::iterator
	end(Container& c)
	{
		return c.end();
	}

	template <typename Container>
	typename Container::const_iterator
	end(const Container& c)
	{
		return c.end();
	}

	template <typename Container>
	typename Container::size_type
	size(const Container& c)
	{
		return c.size();
	}

	template <typename Iter1, typename Iter2>
	Iter1 begin(const std::pair<Iter1, Iter2>& p)
	{
		return p.first;
	}

	template <typename Iter1, typename Iter2>
	Iter2 end(const std::pair<Iter1, Iter2>& p)
	{
		return p.second;
	}

	template <typename Iter1, typename Iter2>
	typename boost::detail::iterator_traits<Iter1>::difference_type
	size(const std::pair<Iter1, Iter2>& p)
	{
		return std::distance(p.first, p.second);
	}
#endif


	///////////////////////////////////////////////////////////////////////////
	// Macros
	///////////////////////////////////////////////////////////////////////////

#define BOOST_FWD_ALGO_BE( A )       return std::A( begin( c ), end( c ) );
#define BOOST_FWD_ALGO_BEX( A, X )   return std::A( begin( c ), end( c ), X );
#define BOOST_FWD_ALGO_BEXY( A, X, Y ) return std::A( begin( c ), end( c ), X, Y );
#define BOOST_FWD_ALGO_BEXYZ( A, X, Y, Z ) \
                                return std::A( begin( c ), end( c ), X, Y, Z );
#define BOOST_FWD_ALGO_BEBE( A )     return std::A( begin( c1 ), end( c1 ), \
                                                    begin( c2 ), end( c2 ) );  
#define BOOST_FWD_ALGO_BEBEX( A, X ) return std::A( begin( c1 ), end( c1 ), \
                                                    begin( c2 ), end( c2 ), X );
#define BOOST_FWD_ALGO_BEB( A )      return std::A( begin( c1 ), end( c1 ), \
	                                                begin( c2 ) );
#define BOOST_FWD_ALGO_BEBX( A, X )  return  std::A( begin( c1 ), end( c1 ), \
                                                     begin( c2 ), X );  
	//
	// naming convention: 
	//  fun   = Unary_function,
	//  pred  = Predicate
	//  value = T (equality compareable)
	//	

#define BOOST_FWD_ALGO_FUN( A )                                              \
    /* @note: don't work with functors :(                                    \
	function_requires< UnaryFunctionConcept<Unary_function, void,            \
                  typename function_traits<Unary_function>::arg1_type > >(); \
    */				                                                         \
    BOOST_FWD_ALGO_BEX( A, fun )

#define BOOST_FWD_ALGO_EQ( A ) /*CHECL_EQ*/      BOOST_FWD_ALGO_BEX( A, value )
#define BOOST_FWD_ALGO_PRED( A ) /*CHECK_PRED*/    BOOST_FWD_ALGO_BEX( A, pred ) 
#define BOOST_FWD_ALGO_BPRED( A ) /*CHECK_BPRED*/  BOOST_FWD_ALGO_BEX( A, pred )
#define BOOST_FWD_ALGO_INT_EQ( A ) /*CHECK_INT, CHECK_EQ*/ \
                 BOOST_FWD_ALGO_BEXY( A, count, value )
#define BOOST_FWD_ALGO_INT_EQ_BPRED( A ) /**CHECK_INT, CHECK_EQ, CHECK_BPRED */ \
                                BOOST_FWD_ALGO_BEXYZ( A, count, value, pred );
#define BOOST_FWD_ALGO3_BPRED( A ) /*CHECK_BPRED*/ BOOST_FWD_ALGO_BEBX( A, pred )
#define BOOST_FWD_ALGO4_BPRED( A ) /*CHECK_BPRED*/BOOST_FWD_ALGO_BEBEX( A, pred )

#define BOOST_ALGO_START1( A, R, C, Arg1 )     R A( C c, Arg1 arg1 )                                                            
#define XXXXXXXX return std::A( begin( c ), end( c ), arg1 );         

	#define CHECK_UNARY_FUNCTION( Fun, Container )               
	#define CHECK_BINARY_PREDICATE( Pred, Container1, Container2 )
	#define CHECK_EQAULITY_COMPARABLE( T )

	///////////////////////////////////////////////////////////////////////////
	// Return type deduction
	///////////////////////////////////////////////////////////////////////////

	template< typename T >
	struct mutable_return
	{
		typedef typename T::iterator  iterator;
		typedef typename std::iterator_traits<iterator>::difference_type
		diff_type; 
	};



	template< typename T, size_t sz >
	struct mutable_return< T[sz] >
	{
		typedef typename array_traits<T[sz]>::iterator  iterator;
		typedef size_t                                  diff_type;
	};



	template< typename T >
	struct const_return
	{
		typedef typename T::const_iterator  iterator;
	};



	template< typename T, size_t sz >
	struct const_return< T[sz] >
	{
		typedef typename array_traits<const T[sz]>::iterator  iterator;
	};



	template< typename T1, typename T2 >
	struct pair_return
	{
		typedef typename std::pair< typename mutable_return<T1>::iterator,
		typename mutable_return<T2>::iterator > mutable_pair;
		
		typedef typename std::pair< typename mutable_return<T1>::iterator,
		typename const_return<T2>::iterator > mutable_const_pair;
		
		typedef typename std::pair< typename const_return<T1>::iterator,
		typename mutable_return<T2>::iterator > const_mutable_pair;
		
		typedef typename std::pair< typename const_return<T1>::iterator,
		typename const_return<T2>::iterator > const_pair;
	};


	///////////////////////////////////////////////////////////////////////////
	// Nonmodifying Sequence Operations
	///////////////////////////////////////////////////////////////////////////

	template< typename Container, typename Unary_function >
	inline Unary_function 
	for_each( const Container& c, Unary_function fun )
	{
		BOOST_FWD_ALGO_FUN( for_each );
	}



	template< typename Container, typename T >
	inline typename mutable_return<Container>::iterator 
	find( Container& c, const T& value )
	{
		BOOST_FWD_ALGO_EQ( find );
	}

	template< typename Container, typename T >
	inline typename const_return<Container>::iterator 
	find( const Container& c, const T& value )
	{
		BOOST_FWD_ALGO_EQ( find );
	}



	template< typename Container, typename Predicate >
	inline typename mutable_return<Container>::iterator 
	find_if( Container& c, Predicate pred )
	{
		BOOST_FWD_ALGO_PRED( find_if );
	}

	template< typename Container, typename Predicate >
	inline typename const_return<Container>::iterator
	find_if( const Container& c, Predicate pred )
	{
		BOOST_FWD_ALGO_PRED( find_if );
	}   



	template< typename Container >
	inline typename mutable_return<Container>::iterator
	adjacent_find( Container& c )
	{
		BOOST_FWD_ALGO_BE( adjacent_find );
	}

	template< typename Container >
	inline typename const_return<Container>::iterator
	adjacent_find( const Container& c )
	{
		BOOST_FWD_ALGO_BE( adjacent_find );
	}

	template< typename Container, typename Binary_predicate >
	inline typename mutable_return<Container>::iterator 
	adjacent_find( Container& c, Binary_predicate pred )
	{
		BOOST_FWD_ALGO_BPRED( adjacent_find );
	}

	template< typename Container, typename Binary_predicate >
	inline typename const_return<Container>::iterator 
	adjacent_find( const Container& c, Binary_predicate pred )
	{
		BOOST_FWD_ALGO_BPRED( adjacent_find );
	}



	template< typename Container1, typename Container2 >
	inline typename mutable_return<Container1>::iterator 
	find_first_of( Container1& c1, const Container2& c2 )
	{
		BOOST_FWD_ALGO_BEBE( find_first_of );
	}

	template< typename Container1, typename Container2 >
	inline typename const_return<Container1>::iterator 
	find_first_of( const Container1& c1, const Container2& c2 )
	{
		BOOST_FWD_ALGO_BEBE( find_first_of );
	}

	template< typename Container1, typename Container2, 
	typename Binary_predicate >
	inline typename mutable_return<Container1>::iterator 
	find_first_of( Container1& c1, const Container2& c2, 
				   Binary_predicate pred )
	{
		BOOST_FWD_ALGO4_BPRED( find_first_of );
	}

	template< typename Container1, typename Container2, 
	typename Binary_predicate >
	inline typename const_return<Container1>::iterator 
	find_first_of( const Container1& c1, const Container2& c2, 
				   Binary_predicate pred )
	{
		BOOST_FWD_ALGO4_BPRED( find_first_of );
	}



	template< typename Container, typename T >
	inline typename mutable_return<Container>::diff_type
	count( const Container& c, const T& value )
	{
		BOOST_FWD_ALGO_EQ( count );
	}



	template< typename Container, typename Predicate >
	inline typename mutable_return<Container>::diff_type
	count_if( const Container& c, Predicate pred )
	{
		BOOST_FWD_ALGO_PRED( count_if );
	}



	template< typename Container1, typename Container2 >
	inline typename pair_return<Container1,Container2>::mutable_pair
	mismatch( Container1& c1, Container2& c2 )
	{
		BOOST_FWD_ALGO_BEB( mismatch );
	}

	template< typename Container1, typename Container2 >
	inline typename pair_return<Container1,Container2>::const_mutable_pair
	mismatch( const Container1& c1, Container2& c2 )
	{
		BOOST_FWD_ALGO_BEB( mismatch );
	}

	template< typename Container1, typename Container2 >
	inline typename pair_return<Container1,Container2>::mutable_const_pair 
	mismatch( Container1& c1, const Container2& c2 )
	{
		BOOST_FWD_ALGO_BEB( mismatch );
	}

	template< typename Container1, typename Container2 >
	inline typename pair_return<Container1,Container2>::const_pair
	mismatch( const Container1& c1, const Container2& c2 )
	{
		BOOST_FWD_ALGO_BEB( mismatch );
	}

	template< typename Container1, typename Container2, 
	typename Binary_predicate >
	inline typename pair_return<Container1, Container2>::mutable_pair
	mismatch_( Container1& c1, Container2& c2, Binary_predicate pred )
	{
		BOOST_FWD_ALGO3_BPRED( mismatch );
	}

	template< typename Container1, typename Container2, 
	typename Binary_predicate >
	inline typename pair_return<Container1,Container2>::const_mutable_pair
	mismatch_( const Container1& c1, Container2& c2, Binary_predicate pred )
	{
		BOOST_FWD_ALGO3_BPRED( mismatch );
	}

	template< typename Container1, typename Container2, 
	typename Binary_predicate >
	inline typename pair_return<Container1,Container2>::mutable_const_pair
	mismatch_( Container1& c1, const Container2& c2, Binary_predicate pred )
	{
		BOOST_FWD_ALGO3_BPRED( mismatch );
	}

	template< typename Container1, typename Container2, 
	typename Binary_predicate >
	inline typename pair_return<Container1,Container2>::const_pair
	mismatch_( const Container1& c1, const Container2& c2, Binary_predicate pred )
	{
		BOOST_FWD_ALGO3_BPRED( mismatch );
	}



	template< typename Container1, typename Container2 >
	inline bool 
	equal( const Container1& c1, const Container2& c2 )
	{
		BOOST_FWD_ALGO_BEB( equal );
	}

	template< typename Container1, typename Container2, 
	typename Binary_predicate > 
	inline bool 
	equal_( const Container1& c1, const Container2& c2, Binary_predicate pred )
	{
		BOOST_FWD_ALGO3_BPRED( equal );
	}



	template< typename Container1, typename Container2 >
	inline typename mutable_return<Container1>::iterator 
	search( Container1& c1, const Container2& c2 )
	{
		BOOST_FWD_ALGO_BEBE( search );
	}

	template< typename Container1, typename Container2 >
	inline typename const_return<Container1>::iterator 
	search( const Container1& c1, const Container2& c2 )
	{
		BOOST_FWD_ALGO_BEBE( search );
	}

	template< typename Container1, typename Container2,
	typename Binary_predicate >
	inline typename mutable_return<Container1>::iterator
	search( Container1& c1, const Container2& c2, Binary_predicate pred )
	{
		BOOST_FWD_ALGO4_BPRED( search );
	}

	template< typename Container1, typename Container2,
	typename Binary_predicate >
	inline typename const_return<Container1>::iterator
	search( const Container1& c1, const Container2& c2, Binary_predicate pred )
	{
		BOOST_FWD_ALGO4_BPRED( search );
	}



	template< typename Container, typename Integer, typename T >
	inline typename mutable_return<Container>::iterator 
	search_n( Container& c, Integer count, const T& value )
	{
		BOOST_FWD_ALGO_INT_EQ( search_n );
	}

	template< typename Container, typename Integer, typename T >
	inline typename const_return<Container>::iterator 
	search_n( const Container& c, Integer count, const T& value )
	{
		BOOST_FWD_ALGO_INT_EQ( search_n );
	}

	template< typename Container, typename Integer, 
	typename T, typename Binary_predicate >
	inline typename mutable_return<Container>::iterator
	search_n( Container& c, Integer count, const T& value,
			  Binary_predicate pred )
	{
		BOOST_FWD_ALGO_INT_EQ_BPRED( search_n );
	}

	template< typename Container, typename Integer, 
	typename T, typename Binary_predicate >
	inline typename const_return<Container>::iterator
	search_n( const Container& c, Integer count, const T& value,
			  Binary_predicate pred )
	{
		BOOST_FWD_ALGO_INT_EQ_BPRED( search_n );
	}



	template< typename Container1, typename Container2 >
	typename mutable_return<Container1>::iterator 
	find_end( Container1& c1, const Container2& c2 )
	{
		BOOST_FWD_ALGO_BEBE( find_end ); 
	}

	template< typename Container1, typename Container2 >
	typename const_return<Container1>::iterator 
	find_end( const Container1& c1, const Container2& c2 )
	{
		BOOST_FWD_ALGO_BEBE( find_end ); 
	}

	template< typename Container1, typename Container2,
	typename Binary_predicate >
	typename mutable_return<Container1>::iterator 
	find_end( Container1& c1, const Container2& c2, Binary_predicate pred )
	{
		BOOST_FWD_ALGO4_BPRED( find_end );
	}

	template< typename Container1, typename Container2,
	typename Binary_predicate >
	typename const_return<Container1>::iterator 
	find_end( const Container1& c1, const Container2& c2, 
			  Binary_predicate pred )
	{
		BOOST_FWD_ALGO4_BPRED( find_end );
	}

	///////////////////////////////////////////////////////////////////////////
	// Modifying Sequance Operations
	///////////////////////////////////////////////////////////////////////////
	/*
copy 
copy_backward  
swap_ranges 
transform 
replace 
replace_if 
replace_copy 
replace_copy_if 
fill 
fill_n 
generate 
generate_n 
remove 
remove_if 
remove_copy 
remove_copy_if 
unique 
unique_copy 
reverse 
reverse_copy 
rotate 
rotate_copy 
random_shuffle 

*/
	template<class Container, class OutputIterator>
	inline OutputIterator
	copy(const Container& container, OutputIterator output);

	template<class Container, class BidirectionalIterator>
	inline BidirectionalIterator
	copy_backward(const Container& container, BidirectionalIterator out);

	template<class Container, class ForwardIterator>
	inline typename Container::iterator 
	swap_ranges(Container &container, ForwardIterator out);

	template<class Container, class OutputIterator, class UnaryFunction>
	inline OutputIterator
	transform(const Container& container, OutputIterator result,
			  UnaryFunction f);


	template<class Container, class UnaryFunction>
	inline typename Container::iterator
	transform (Container &container, UnaryFunction f);

	template<class Container, class T>
	inline void
	replace(Container& container, const T& what, const T& with_what);


	template<class Container, class Predicate, class T>
	inline void
	replace_if(Container& container, Predicate pred, const T& value);


	template<class Container, class OutputIterator, class T>
	inline OutputIterator
	replace_copy(const Container& container, OutputIterator out,
				 const T& what, const T& value);

	template<class Container, class OutputIterator, class Predicate, class T>
	inline OutputIterator
	replace_copy_if(const Container& container, OutputIterator out,
					Predicate pred, const T& value);

	template<class Container, class T>
	inline void
	fill(Container& container, const T& value);


	template<class Container, class Size, class T>
	inline void
	fill_n(Container& container, Size size, const T &value);


	template<class Container, class Generator>
	inline void
	generate(Container &container, Generator gen);


	template<class Container, class Size, class Generator>
	inline void
	generate_n(Container &container, Size size, Generator gen);

	template<class Container, class T>
	inline typename Container::iterator
	remove(Container& container, const T& what); 

	template<class Container, class Predicate>
	inline typename Container::iterator
	remove_if(Container& container, Predicate pred);

	template<class Container, class OutputIterator, class T>
	inline OutputIterator 
	remove_copy(const Container& container, OutputIterator out,
				const T& value);

	template<class Container, class OutputIterator, class Predicate>
	inline OutputIterator 
	remove_copy_if(const Container& container, OutputIterator out,
				   Predicate pred);

	template<class Container>
	inline typename Container::iterator
	unique(Container& container); 


	template<class Container, class OutputIterator>
	inline OutputIterator
	unique_copy(const Container& container, OutputIterator out);

	template<class Container>
	inline void
	reverse(Container &container);

	template<class Container>
	inline void
	reverse_copy(Container &container);

	template<class Container, class ForwardIterator>
	inline void
	rotate(Container& container, ForwardIterator middle);

	template<class Container, class ForwardIterator>
	inline void
	rotate_copy(Container& container, ForwardIterator middle);

	template<class Container>
	inline void
	random_shuffle(Container& container);

	template<class Container, class Predicate>
	inline typename Container::iterator
	partition(Container& container, Predicate pred);

	template<class Container, class Predicate>
	inline typename Container::iterator
	stable_partition(Container& container, Predicate pred);

	///////////////////////////////////////////////////////////////////////////
	// Sorted Sequences
	///////////////////////////////////////////////////////////////////////////
	/*
	sort 
	stable_sort 
	partial_sort 
	partial_sort_copy 
	nth_element 
	Binary search 
	lower_bound 
	upper_bound 
	equal_range 
	binary_search 
	merge 
	inplace_merge 
	partition 
	stable_partition 
	*/
	template<class Container>
	inline void
	sort(Container& container);


	//stable_sort
	template<class Container>
	inline void
	stable_sort(Container &container);


	//partial_sort
	template<class Container>
	inline void
	partial_sort(Container& container, typename Container::iterator middle);

	template<class Container1, class Container2>
	inline void
	partial_sort_copy(Container1& container1, Container2& container2);

	template<class Container>
	inline void
	nth_element(Container& container, typename Container::iterator nth);

	template<class Container, class T>
	inline bool
	binary_search(const Container& container, const T& value);

	template<class Container, class T>
	inline typename Container::const_iterator
	lower_bound(const Container& container, const T& value);

	template<class Container, class T>
	inline typename Container::iterator
	upper_bound(Container& container, const T& value);

	template<class Container, class T>
	inline std::pair<typename Container::iterator, typename Container::iterator>
	equal_range(Container& container, const T& value);


	template<class Container, class T>
	inline std::pair<typename Container::const_iterator, typename Container::const_iterator>
	equal_range(const Container& container, const T& value);

	template<class Container>
	inline void
	inplace_merge(Container& container, typename Container::iterator 
				  middle);

	template<class Container1, class Container2, class OutputIterator>
	inline OutputIterator
	merge(const Container1& container1, const Container2& container2,
		  OutputIterator i);


	template<class Container1, class Container2, class OutputIterator,
	class Compare>
	inline OutputIterator
	merge(const Container1& container1, const Container2& container2,
		  OutputIterator i, Compare comp);

	template<class Container, class Predicate>
	inline typename Container::iterator
	partition(Container& container, Predicate pred);


	template<class Container, class Predicate>
	inline typename Container::iterator
	stable_partition(Container& container, Predicate pred);

	///////////////////////////////////////////////////////////////////////////
	// Set Algorithms
	///////////////////////////////////////////////////////////////////////////
	/*
	includes 
set_union 
set_intersection 
set_difference 
set_symmetric_difference 
*/

	template<class Container1, class Container2>
	inline bool 
	includes(const Container1& container1, const Container2& container2);

	template<class Container1, class Container2, class Compare>
	inline bool 
	includes(const Container1& container1, const Container2& container2,
			 Compare comp);


	template<class Container1, class Container2, class OutputIterator>
	inline OutputIterator
	set_union(const Container1& container1, const Container2& container2,
			  OutputIterator i);


	template<class Container1, class Container2, class OutputIterator,
	class Compare>
	inline OutputIterator
	set_union(const Container1& container1, const Container2& container2,
			  OutputIterator i, Compare comp);

	template<class Container1, class Container2, class OutputIterator,
	class Compare>
	inline OutputIterator
	set_intersection(const Container1& container1,
					 const Container2& container2, OutputIterator i,
					 Compare comp);


	template<class Container1, class Container2, class OutputIterator>
	inline OutputIterator
	set_difference(const Container1& container1,
				   const Container2& container2,  OutputIterator i);


	template<class Container1, class Container2, class OutputIterator,
	class Compare>
	inline OutputIterator
	set_difference(const Container1& container1,
				   const Container2& container2, OutputIterator i, Compare 
				   comp);

	template<class Container1, class Container2, class OutputIterator>
	inline OutputIterator
	set_symmetric_difference(const Container1& container1,
							 const Container2& container2,
							 OutputIterator i);

	template<class Container1, class Container2, class OutputIterator,
	class Compare>
	inline OutputIterator
	set_symmetric_difference(const Container1& container1,
							 const Container2& container2,
							 OutputIterator i, Compare comp);
	///////////////////////////////////////////////////////////////////////////
	// Heap Operations
	///////////////////////////////////////////////////////////////////////////
	/*
  push_heap 
pop_heap 
make_heap 
sort_heap 
*/
	template<class Container>
	inline void 
	make_heap(Container& container);


	template<class Container, class Compare>
	inline void
	make_heap(Container& container, Compare comp);


	template<class Container>
	inline void 
	push_heap(Container& container);


	template<class Container, class Compare>
	inline void
	push_heap(Container& container, Compare comp);


	template<class Container>
	inline void 
	pop_heap(Container& container);


	template<class Container, class Compare>
	inline void
	pop_heap(Container& container, Compare comp);

	template<class Container>
	inline void 
	sort_heap(Container& container);


	template<class Container, class Compare>
	inline void
	sort_heap(Container& container, Compare comp);

	///////////////////////////////////////////////////////////////////////////
	// Minimum and Maximum
	///////////////////////////////////////////////////////////////////////////
	/*
	min_element 
	max_element 
	lexicographical_compare 
	*/

	template<class Container>
	inline typename Container::const_iterator
	min_element(const Container &container);

	template<class Container>
	inline typename Container::const_iterator
	max_element(const Container &container);

	template<class Container1, class Container2>
	inline bool
	lexicographical_compare
	(const Container1& c1, const Container2& c2);

	///////////////////////////////////////////////////////////////////////////
	// Permutations
	///////////////////////////////////////////////////////////////////////////
	/*
		next_permutation 
			prev_permutation
		*/
	template<class Container>
	inline bool
	next_permutation(Container& container); 


	template<class Container>
	inline bool
	prev_permutation(Container& container);

	///////////////////////////////////////////////////////////////////////////
	// Generalized Numeric Algorithms
	///////////////////////////////////////////////////////////////////////////
	/*  accumulate 
	inner_product 
	partial_sum 
	adjacent_difference 
	*/

	template<class Container, class ContainerType>
	inline ContainerType
	accumulate(const Container& container, ContainerType initial);


	template<class Container, class ContainerType, class BinaryOperation>
	inline ContainerType
	accumulate(const Container& container, const ContainerType& initial,
			   BinaryOperation op);

	template<class Container1, class Container2, class ContainerType>
	inline ContainerType                             
	inner_product(const Container1& c1, const Container2& c2,
				  const ContainerType initial);

	template<class Container, class OutputIterator>
	inline OutputIterator
	partial_sum(const Container& container, OutputIterator out);


	template<class Container, class OutputIterator>
	inline OutputIterator
	adjacent_difference(const Container& container, OutputIterator out);


	template<class Container, class OutputIterator, class Predicate>
	inline OutputIterator
	adjacent_difference(const Container& container, OutputIterator out,
						Predicate pred);

	///////////////////////////////////////////////////////////////////////////
	// Boost algorithm extensions
	///////////////////////////////////////////////////////////////////////////

	template <typename Container, typename T>
	void iota(Container& c, const T& value);

	template <typename Container, typename T>
	bool contains(const Container& c, const T& value);


	// container all
	template <typename Container, typename Predicate>
	bool all(const Container& c, Predicate p);


	// container none
	template <typename Container, typename Predicate>
	bool none(const Container& c, Predicate p);


	// container any_if
	template <typename Container, typename Predicate>
	bool any_if(const Container& c, Predicate p);


	template <typename Container>  
	bool is_sorted(const Container& c);


	template <typename Container, typename StrictWeakOrdering>
	bool is_sorted(const Container& c, StrictWeakOrdering comp);


	///////////////////////////////////////////////////////////////////////////
	// Implementation
	///////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////
	// Non-mutating algorithms
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// Mutating algorithms
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// Sorting algorithms
	///////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////
	// Boost algorithm extensions
	///////////////////////////////////////////////////////////////////////////
	/*
		// container iota
		template <typename Container, typename T>
		void iota(Container& c, const T& value)
		{
			iota(begin(c), end(c), value);
		}
	
		// container copy
		template <typename Container, typename OutIter>
		OutIter copy(const Container& c, OutIter result)
		{
			return std::copy(begin(c), end(c), result);
		}
	
		// container equal
		template <typename Container1, typename Container2>
		bool equal(const Container1& c1, const Container2& c2)
		{
			if( size(c1) != size(c2) )
				return false;
			return std::equal(begin(c1), end(c1), begin(c2));
		}
	
		// container sort
		template <typename Container>
		void sort(Container& c)
		{
			std::sort(begin(c), end(c));
		}
	
		template <typename Container, typename Predicate>
		void sort(Container& c, const Predicate& p)
		{
			std::sort(begin(c), end(c), p);
		}
	
		// container stable_sort
		template <typename Container>
		void stable_sort(Container& c)
		{
			std::stable_sort(begin(c), end(c));
		}
	
		template <typename Container, typename Predicate>
		void stable_sort(Container& c, const Predicate& p)
		{
			std::stable_sort(begin(c), end(c), p);
		}
	
		// container contains
		template <typename Container, typename T>
		bool contains(const Container& c, const T& value)
		{
			return contains(begin(c), end(c), value);
		}
	
		// container all
		template <typename Container, typename Predicate>
		bool all(const Container& c, Predicate p)
		{
			return all(begin(c), end(c), p);
		}
	
		// container none
		template <typename Container, typename Predicate>
		bool none(const Container& c, Predicate p)
		{
			return none(begin(c), end(c), p);
		}
	
		// container any_if
		template <typename Container, typename Predicate>
		bool any_if(const Container& c, Predicate p)
		{
			return any_if(begin(c), end(c), p);
		}
	
		// container count
		template <typename Container, typename T>
		std::size_t count(const Container& c, const T& value)
		{
			return std::count(begin(c), end(c), value);
		}
	
		// container count_if
		template <typename Container, typename Predicate>
		std::size_t count_if(const Container& c, Predicate p)
		{
			return std::count_if(begin(c), end(c), p);
		}
	
		// container is_sorted
		template <typename Container>
		bool is_sorted(const Container& c)
		{
			return is_sorted(begin(c), end(c));
		}
	
		template <typename Container, typename StrictWeakOrdering>
		bool is_sorted(const Container& c, StrictWeakOrdering comp)
		{
			return is_sorted(begin(c), end(c), comp);
		}
	*/


} // namespace boost

#endif // BOOST_CONTAINER_ALGORITHM_HPP

