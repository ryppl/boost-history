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


#ifndef BOOST_ALGO_TRAITS_HPP
#define BOOST_ALGO_TRAITS_HPP

#include <boost/config.hpp>
#include <boost/type_traits.hpp>
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
    // difference_type
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
    
    //
    // Implemented using "poor man's partial template specialization".
    //

    namespace detail 
    {   

	//////////////////////////////////////////////////////////////////////
	// enumeration for distinguish template specializtions
	//////////////////////////////////////////////////////////////////////
 
	namespace container
	{
	    enum Container_t
	    {
		stl_t         = 0,
		pair_t        = 1,
		array_t       = 2
	    };
	}

	///////////////////////////////////////////////////////////////////////
	// partial specializations
	///////////////////////////////////////////////////////////////////////

	template< container::Container_t >		
	struct container_helper
	{
	    template< typename C >
	    struct traits
	    {
		typedef typename C::size_type          size_type;
		typedef typename C::iterator           iterator;
		typedef typename C::const_iterator     const_iterator;
		typedef typename C::difference_type    difference_type;

	    };

	    template< typename C >
	    static inline typename traits<C>::size_type 
	    size( const C& c )
	    {
		return c.size();
	    }

	    template< typename C >
	    static inline typename traits<C>::iterator 
	    begin( C& c )
	    {
		return c.begin();
	    }

	    template< typename C >
	    static inline typename traits<C>::const_iterator 
	    begin( const C& c )
	    {
		return c.begin();
	    }
	    
	    template< typename C >
	    static inline typename traits<C>::iterator 
	    end( C& c )
	    {
		return c.end();
	    }
	    
	    template< typename C >
	    static inline typename traits<C>::const_iterator 
	    end( const C& c )
	    {
		return c.end();
	    }
	    
	}; // 'container_helper'



	template<>
	struct container_helper< container::pair_t >
	{
	    template< typename P >
	    struct traits
	    {
		typedef typename P::first_type             element_type;

#ifndef BOOST_NO_STD_ITERATOR_TRAITS

		typedef typename 
		std::iterator_traits< element_type >::difference_type 
                                                           size_type;
#else
		typedef std::size_t                        size_type;
#endif
		typedef element_type                       iterator;
		typedef element_type                       const_iterator;
		typedef size_type                          difference_type; 
	    };

	    template< typename P >
	    static inline typename traits<P>::size_type 
	    size( const P& p )
	    {
		return std::distance( p.first, p.second );
	    }

	    template< typename P >
	    static inline typename traits<P>::const_iterator 
	    begin( const P& p )
	    {
		return p.first;
	    }

	    template< typename P >
	    static inline typename traits<P>::const_iterator 
	    end( const P& p )
	    {
		return p.second;
	    }

	}; // 'container_helper'



#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

	template<>
	struct container_helper< container::array_t >
	{

	    template< typename T >
	    struct array_traits;

	    template< typename T, std::size_t sz >
	    struct array_traits<T[sz]>
	    {
		typedef T*              iterator;
		typedef const T*        const_iterator;
		enum { size = sz };
	    };

	    template< typename A >
	    struct traits
	    {		
		typedef std::size_t                            size_type; 
		typedef typename array_traits<A>::iterator     iterator;
		typedef typename array_traits<A>::const_iterator  
	                                                       const_iterator;
		typedef std::size_t                            difference_type;
		enum { sz = array_traits<A>::size };
	    };
	    
	    template< typename A >
	    static inline typename traits<A>::size_type 
	    size( const A& a )
	    {
		    return traits<A>::sz;
	    }
	    
	    template< typename A >
	    static inline typename traits<A>::iterator 
	    begin( A& a )
	    {
		return a;
	    }
	    
	    template< typename A >
	    static inline typename traits<A>::const_iterator 
	    begin( const A& a )
	    {
		return a;
	    }
	    
	    template< typename A >
	    static inline typename traits<A>::iterator 
	    end( A& a )
	    {
		return a + traits<A>::sz;
	    }
	    
	    template< typename A >
	    static inline typename traits<A>::const_iterator 
	    end( const A& a )
	    {
		return a + traits<A>::sz;
	    }

	}; // 'container_helper'

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

	///////////////////////////////////////////////////////////////////////
	// SFINAE helper functions
	///////////////////////////////////////////////////////////////////////

	namespace container
	{
	    template< int sz >
	    struct has_size
	    {
		char size[sz];
	    };

	    typedef has_size<1> false_t;
	    typedef has_size<2> true_t;

	    template< typename T, typename T2 >
	    true_t  is_pair( const std::pair<T,T2>& );
	    false_t is_pair( ... );

	    template< typename T, typename T2 >
	    true_t  is_const_pair( const std::pair<T,T2>& );
	    false_t is_const_pair( ... );
	}

    } // namespace 'detail'
    
    ///////////////////////////////////////////////////////////////////////////
    // 'algo_traits'
    ///////////////////////////////////////////////////////////////////////////


    template< typename C >
    struct container_traits
    {
    private:
	static C& c;

	enum
	{
	    is_pair_  = sizeof( detail::container::is_pair( c ) ) == 
	                  sizeof( detail::container::true_t ) ? true : false,
	    is_array_ = is_array<C>::value                    ? true : false
	};

	    
	typedef typename mpl::if_c< is_pair_, 
			 detail::container_helper<detail::container::pair_t>,
				    typename mpl::if_c< is_array_, 
			 detail::container_helper<detail::container::array_t>,
			 detail::container_helper<detail::container::stl_t> 
		                                      >::type 
	                          >::type container_helper_t;
    public:

	typedef container_helper_t             function_t;
	
	typedef typename container_helper_t:: 
	template traits<C>::size_type          size_type;
	typedef typename container_helper_t::
	template traits<C>::iterator           iterator;
	typedef typename container_helper_t:: 
	template traits<C>::const_iterator     const_iterator;
	typedef typename container_helper_t:: 
	template traits<C>::difference_type    difference_type;
	    
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
    container_traits<C>::const_iterator
    begin( const C& c )
    {
	return constainer_traits<C>::function_t::begin( c );
    }

    template< typename C >
    inline typename
    container_traits<C>::const_iterator
    end( const C& c )
    {
	return constainer_traits<C>::function_t::end( c );
    }

#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
 
} // namespace 'boost'

#endif
