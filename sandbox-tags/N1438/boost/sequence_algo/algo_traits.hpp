/**
 * short: traits for support of container algorithms.
 */
// (C) Copyright Thorsten Ottosen 2002. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#ifndef BOOST_ALGO_TRAITS_HPP
#define BOOST_ALGO_TRAITS_HPP

#include <cstddef>  
#include <boost/type_traits.hpp>

namespace boost
{

    /////////////////////////////////////////////////////////////////////////
    // Interface
    /////////////////////////////////////////////////////////////////////////

    template< typename Container >
    struct algo_traits;

    /////////////////////////////////////////////////////////////////////////
    // Implementation
    /////////////////////////////////////////////////////////////////////////

#if 0
    
    /**
    * Apparently these helpers cannot be put into the 'algo_traits' class
    * since the compiler will instantiate all templates
    * ( at least vc7 does ) even though the instantiation
    * is not needed.
    */

    template< int >
    struct algo_traits_helper
    {
        template< typename Container >
        struct types
        {
            typedef Container container;
            typedef typename  container::iterator         iterator;
            typedef typename  container::const_iterator   const_iterator;
            typedef typename  container::size_type        size_type;
            typedef typename  container::difference_type  difference_type; 
        };
    };

    template<>
    struct algo_traits_helper<1>
    {
        template< typename Container >
        struct types 
        {
            typedef Container       container;
            typedef container*      iterator;
            typedef const iterator  const_iterator;
            typedef std::size_t     size_type;
            typedef std::size_t     difference_type;
        };
    };

    template< typename Container >
    struct algo_traits
    {
        typedef Container container; 

        enum
        {
            is_array = boost::is_array<container>::value
        };

        typedef typename algo_traits_helper<is_array>:: 
            template types<container>::iterator         iterator;
        typedef typename algo_traits_helper<is_array>:: 
            template types<container>::const_iterator   const_iterator;
        typedef typename algo_traits_helper<is_array>:: 
            template types<container>::size_type        size_type;
        typedef typename algo_traits_helper<is_array>:: 
            template types<container>::difference_type  difference_type;        

    };


#else // standard complient compilers


    template< typename Container >
    struct algo_traits
    {
        typedef Container container;
        typedef typename  container::iterator         iterator;
        typedef typename  container::const_iterator   const_iterator;
        typedef typename  container::size_type        size_type;
        typedef typename  container::difference_type  difference_type;     
    };

    template< typename T, std::size_t sz >
    struct algo_traits<T[sz]>
    {
        typedef T               type;
        typedef type*           iterator;
        typedef const iterator  const_iterator;
        typedef std::size_t     size_type;
        typedef std::size_t     difference_type;
    };

#endif

} // namespace 'boost'

#endif