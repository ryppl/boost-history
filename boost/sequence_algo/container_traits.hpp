/**
 * short: traits for support of container algorithms.
 */
// (C) Copyright Thorsten Ottosen 2002-2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// (C) Copyright Jeremy Siek 2001. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.


#ifndef BOOST_SEQUENCE_ALGO_CONTAINER_TRAITS_HPP
#define BOOST_SEQUANCE_ALGO_CONTAINER_TRAITS_HPP

#include <boost/config.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/mpl/if.hpp>
#include <cstddef>  
#include <iterator>

namespace boost
{

    /////////////////////////////////////////////////////////////////////////
    // Interface
    /////////////////////////////////////////////////////////////////////////

    template< typename Container >
    struct container_traits;
   
    //
    // Valid types from the traits:
    //------------------------------------------
    // size_type
    // iterator
    // const_iterator
    // result_iterator (will follow the constness of the argument)
    // difference_type
    // container_category (tag)
    //
    // Valid free standing functions:
    //------------------------------------------
    // iterator         begin( Container& );
    // conat_iterator   begin( const Container& );
    // iterator         end( Container& );  
    // const_iterator   end( const Container& );
    // size_type        size( const Container& )
    //

    /////////////////////////////////////////////////////////////////////////
    // Implementation
    /////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////
    //
    // Container tags:
    //    - is_container
    //    - is_sequence
    //    - is_contiguous_sequence  (arrays and vector)
    //    - is_associative
    //    - is_pair
    //    - is_iterator_range  ( perhaps only this one and away with pair )
    //
    // 
    //
    // implemented by SFINAE helper functions
    //
    ///////////////////////////////////////////////////////////////////////

    struct container_tag {};
    struct sequence_container_tag : public container_tag {};
    struct contiguous_sequence_container_tag : public sequence_container_tag{};
    struct associative_sequence_container_tag : public container_tag {};
    struct iterator_range_container_tag : public container_tag {};
    
    namespace detail
    {

	namespace container
	{
	    template< int sz >
	    struct has_size
	    {
		char size[sz];
	    };

	    typedef has_size<1> false_t;
	    typedef has_size<2> true_t;

	    template< typename T, typename U >
	    true_t  is_pair( const std::pair<T,U>& );
	    false_t is_pair( ... );
 
	    template< typename C, typename T, typename D,
		      typename P, typename R >
 	    true_t  is_iterator( const std::iterator<C,T,D,P,R>& );	
	    template< typename C >
	    true_t  is_iterator( const std::back_insert_iterator<C>& );
	    template< typename C >
	    true_t  is_iterator( const std::front_insert_iterator<C>& );
	    template< typename C >
	    true_t  is_iterator( const std::insert_iterator<C>& );
	    template< typename T, typename C, typename Tr >
	    true_t  is_iterator( const std::istream_iterator<T,C,Tr>& );
	    template< typename T, typename C, typename Tr >
	    true_t  is_iterator( const std::ostream_iterator<T,C,Tr>& );
	    template< typename C, typename Tr >
	    true_t  is_iterator( const std::istreambuf_iterator<C,Tr>& );
	    template< typename C, typename Tr >
	    true_t  is_iterator( const std::ostreambuf_iterator<C,Tr>& );
	    false_t is_iterator( ... );

// 	    template< typename C >
// 	    true_t  is_container( const C&, const typename C::iterator& = 
// 				  typename C::iterator() );
// 	    template< typename T, std::size_t sz >
// 	    trut_t  is_container( const T (&)[sz] );
// 	    template< typename T, typename U >
// 	    true_t  is_container( const std::pair<T,U>& );
// 	    false_t is_container( ... );

// 	    template< typename C >
// 	    true_t  is_associative_container( const C&, 
// 					      const typename C::key_type =
// 					      typename C::key_type() );
// 	    false_t is_associative_container( ... );

// 	    template< typename T, typename U >
// 	    true_t  is_iterator_range( const std::pair<T,U>& );
// 	    template< typename C >
// 	    true_t  is_iterator_range( const C&, 
// 				       const typename C::iterator_range_tag& =
// 				       typename C::iterator_range_tag() );
// 	    false_t is_iterator_range( ... );
				       
	    template< typename C >
	    struct tag_generator
	    {
// 		BOOST_STATIC_CONSTANT( bool, is_container_ );
// 		BOOST_STATIC_CONSTANT( bool, is_sequence_ );
// 		BOOST_STATIC_CONSTANT( bool, is_contiguous_sequence_ );
// 		BOOST_STATIC_CONSTANT( bool, is_associative_ );
// 		BOOST_STATIC_CONSTANT( bool, is_iterator_range_ );

			       		    
// 	typedef typename mpl::if_c< is_pair_, 
// 			 typename detail::pair_container_helper<C>,
// 	        typename mpl::if_c< is_array_, 
// 			 typename detail::array_container_helper<C>,
// 		typename mpl::if_c< is_iterator, 
//                          typename detail::iterator_container_helper<C>,
// 			 typename detail::default_container_helper<C>
// 	                          >::type 
// 		                  >::type 
		
		
	    }; // struct 'tag_generator'

// 	    template
// 	    true_t  is_sequence_container()
//
// hm...how to destinguish sequence and maps? is_container && not_is_map
//                                            && not_is_iterator_range
//
	    
		} // namespace 'container'


    
	//
	// Implemented using "poor man's partial template specialization".
	//
	
	///////////////////////////////////////////////////////////////////////
	// partial specializations
	///////////////////////////////////////////////////////////////////////

	template< typename Cont >		
	struct default_container_helper
	{
	    BOOST_STATIC_CONSTANT( bool, is_const_ = 
				   ::boost::is_const<Cont>::value );
	    
	    typedef typename Cont::size_type          size_type;
	    typedef typename Cont::iterator           iterator;
	    typedef typename Cont::const_iterator     const_iterator;
	    typedef typename 
	    mpl::if_c< is_const_,
		       const_iterator,
		       iterator >                     result_iterator;
	    typedef typename Cont::difference_type    difference_type;
	    

	    template< typename C >
	    static inline size_type 
	    size( const C& c )
	    {
		return c.size();
	    }

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING

	    template< typename C >
	    static inline iterator 
	    begin( C& c )
	    {
		return c.begin();
	    }

	    template< typename C >
	    static inline const_iterator 
	    begin( const C& c )
	    {
		return c.begin();
	    }
	    
	    template< typename C >
	    static inline iterator 
	    end( C& c )
	    {
		return c.end();
	    }
	    
	    template< typename C >
	    static inline const_iterator 
	    end( const C& c )
	    {
		return c.end();
	    }
	
#else // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
	    
	    template< typename C >
	    static inline result_iterator
	    begin( C& c )
	    {
		return c.begin();
	    }
	    
	    template< typename C >
	    static inline result_iterator
	    end( C& c )
	    {
		return c.end();
	    }

#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING    

	}; // 'default_container_helper'



	template< typename Pair >
	struct pair_container_helper
	{
	    typedef typename Pair::first_type              element_type;

#ifndef BOOST_NO_STD_ITERATOR_TRAITS

	    typedef typename 
	    std::iterator_traits< element_type >::difference_type 
	                                                   size_type;
#else
	    typedef std::size_t                            size_type;
#endif
	    typedef element_type                           iterator;
	    typedef element_type                           const_iterator;
	    typedef element_type                           result_iterator;
	    typedef size_type                              difference_type; 

	    template< typename P >
	    static inline size_type 
	    size( const P& p )
	    {
		return std::distance( p.first, p.second );
	    }

	    template< typename P >
	    static inline const_iterator 
	    begin( const P& p )
	    {
		return p.first;
	    }

	    template< typename P >
	    static inline const_iterator 
	    end( const P& p )
	    {
		return p.second;
	    }

	}; // 'pair_container_helper'




	template< typename Iterator >
	struct iterator_container_helper
	{
	    typedef std::size_t    size_type;
	    typedef Iterator       iterator;
	    typedef Iterator       const_iterator;
	    typedef Iterator       result_iterator;
	    typedef std::size_t    difference_type;

	    template< typename I >
	    static inline iterator 
	    begin( const I& i )
	    {
		return i;
	    }
	    
	    //
	    // @todo: does this holds for all iterators?
	    //
 	    template< typename I >
 	    static inline iterator
 	    end( const I& i )
 	    {
 		return iterator();
 	    }

	};




	template< typename Array >
	struct array_container_helper
	{
#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

	    template< typename T >
	    struct array_traits;

	    template< typename T, std::size_t sz >
	    struct array_traits<T[sz]>
	    {
		typedef T*              iterator;
		typedef const T*        const_iterator;
		BOOST_STATIC_CONSTANT( std::size_t, size = sz );
	    };

	    BOOST_STATIC_CONSTANT( bool, is_const_ = 
				   ::boost::is_const<Array>::value );

	    typedef std::size_t                            size_type; 
	    typedef typename array_traits<Array>::iterator iterator;
	    typedef typename array_traits<Array>::const_iterator  
	                                                   const_iterator;
	    typedef typename 
	    mpl::if_c< is_const_,
		       const_iterator,
		       iterator >                          result_iterator;
	    typedef std::size_t                            difference_type;

	    template< typename A >
	    static inline size_type 
	    size( const A& a )
	    {
		    return array_traits<A>::size;
	    }
	    
	    template< typename A >
	    static inline iterator 
	    begin( A& a )
	    {
		return a;
	    }
	    
	    template< typename A >
	    static inline const_iterator 
	    begin( const A& a )
	    {
		return a;
	    }
	    
	    template< typename A >
	    static inline iterator 
	    end( A& a )
	    {
		return a + array_traits<A>::size;
	    }
	    
	    template< typename A >
	    static inline const_iterator 
	    end( const A& a )
	    {
		return a + array_traits<A>::size;
	    }

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

	}; // 'array_container_helper'

    } // namespace 'detail'
    
    ///////////////////////////////////////////////////////////////////////////
    // 'container_traits'
    ///////////////////////////////////////////////////////////////////////////


    template< typename C >
    struct container_traits
    {
    private:
	static C& c;

	BOOST_STATIC_CONSTANT( bool, is_pair_ = 
			       sizeof( detail::container::is_pair( c ) ) == 
			       sizeof( detail::container::true_t ) );
	BOOST_STATIC_CONSTANT( bool, is_array_ = ::boost::is_array<C>::value );
	BOOST_STATIC_CONSTANT( bool, is_const_ = ::boost::is_const<C>::value );
	BOOST_STATIC_CONSTANT( bool, is_iterator = 
			       sizeof( detail::container::is_iterator( c ) ) 
			       == sizeof( detail::container::true_t ) );
			       		    
	typedef typename mpl::if_c< is_pair_, 
			 typename detail::pair_container_helper<C>,
	        typename mpl::if_c< is_array_, 
			 typename detail::array_container_helper<C>,
		typename mpl::if_c< is_iterator, 
                         typename detail::iterator_container_helper<C>,
			 typename detail::default_container_helper<C>
	                          >::type 
		                  >::type 
	                          >::type container_helper_t;
    public:

	typedef container_helper_t                             function_t;
	typedef typename container_helper_t::size_type         size_type;
	typedef typename container_helper_t::iterator          iterator;
	typedef typename container_helper_t::const_iterator    const_iterator;
	typedef typename container_helper_t::result_iterator   result_iterator;
	typedef typename container_helper_t::difference_type   difference_type;
	//typedef typename container::tag_generator<C>::tag   container_category;

    }; // 'container_traits'

    ///////////////////////////////////////////////////////////////////////////
    // size()/begin()/end() functions
    ///////////////////////////////////////////////////////////////////////////

    template< typename C >
    inline typename
    container_traits<C>::size_type
    size( const C& c )
    {
        return container_traits<C>::function_t::size( c ); 
    }

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
 
    template< typename C >
    inline typename
    container_traits<C>::iterator
    begin( C& c )
    {
        return container_traits<C>::function_t::begin( c ); 
    }

    template< typename C >
    inline typename
    container_traits<C>::const_iterator
    begin( const C& c )
    {
        return container_traits<C>::function_t::begin( c ); 
    }

    template< typename C >
    inline typename
    container_traits<C>::iterator
    end( C& c )
    {
        return container_traits<C>::function_t::end( c );
    }

    template< typename C >
    inline typename
    container_traits<C>::const_iterator
    end( const C& c )
    {
        return container_traits<C>::function_t::end( c );
    }

#else // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

    template< typename C >
    inline typename
    container_traits<C>::result_iterator
    begin( C& c )
    {
	return container_traits<C>::function_t::begin( c );
    }

    template< typename C >
    inline typename
    container_traits<C>::result_iterator
    end( C& c )
    {
	return container_traits<C>::function_t::end( c );
    }

#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
 
} // namespace 'boost'

#endif
