//  Boost string_algo library container_traits.hpp header file  -------------//

//  (C) Copyright Pavol Droba 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

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

//  Orignal idea of container traits was proposed by Jeremy Siek and
//  Thorsten Ottosen. This implemnetation is lightweighted version
//  of container_traits addapter for usage with string_algo library

#ifndef BOOST_STRING_CONTAINER_TRAITS_HPP
#define BOOST_STRING_CONTAINER_TRAITS_HPP

#include <boost/string_algo/config.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/mpl/apply_if.hpp>

// Implementation
#include <boost/string_algo/detail/container_traits.hpp>

/*! \file
	Defines container_traits class and related free-standing fuctions.
	This facility is used to unify the access to different type of containers.
	It allows the algoritms in the library to work with std containers, c-style
	array, null-terminated c-strings ( end more ) using the same interface.
*/

namespace boost {
    namespace string_algo {

//  container_traits template class -----------------------------------------//
		
		//! container_traits class
		/*!
			Container traits provides uniform access to different types of 
			containers. This functionality allows to write generic algorithms
			which work with several different kinds of containers.

			Currently following container types are supported:
				- containers with stl compatible container interface ( see ContainerConcept )
					( i.e. std::vector<>, std::list<>, std::string<> ... )
				- c-style array 
				   ( char[10], int[15] ... )
				- null-terminated c-strings
					( char*, wchar_T* )
				- std::pair of iterators 
					( i.e std::pair< vector<int>::iterator,vector<int>::iterator > )

			Container traits provide a supset of container interface operations.
			All are accessible using free-standing functions.

			Following operations are supported:
				- size()
				- empty()
				- begin()
				- end()

			Container traits have somewhat limited functionality on compilers not
			supporting partial template specialization and partial template ordering.
		*/
        template< typename T >
        struct container_traits
        {
        private:
            typedef BOOST_STRING_TYPENAME ::boost::mpl::apply_if< 
                    ::boost::string_algo::detail::is_pair<T>, 
                        detail::pair_container_traits_selector<T>,
                        BOOST_STRING_TYPENAME ::boost::mpl::apply_if< 
                        ::boost::is_array<T>, 
                            detail::array_container_traits_selector<T>,
                            BOOST_STRING_TYPENAME ::boost::mpl::apply_if<
                            ::boost::is_pointer<T>,
                                detail::pointer_container_traits_selector<T>,
                                detail::default_container_traits_selector<T>
                            >
                        > 
                >::type container_helper_type;
        public:
			//! Function type		
			typedef container_helper_type function_type;        
            //! Value type
			typedef BOOST_STRING_TYPENAME 
                container_helper_type::value_type value_type;
			//! Size type
            typedef BOOST_STRING_TYPENAME 
                container_helper_type::size_type size_type;
			//! Iterator type
            typedef BOOST_STRING_TYPENAME 
                container_helper_type::iterator iterator;
			//! Const iterator type
            typedef BOOST_STRING_TYPENAME 
                container_helper_type::const_iterator const_iterator;
			//! Result iterator type ( iterator of const_iterator, depending on the constness of the conainer )
			typedef BOOST_STRING_TYPENAME 
                container_helper_type::result_iterator result_iterator;
			//! Difference type
            typedef BOOST_STRING_TYPENAME 
                container_helper_type::difference_type difference_type;

        }; // 'container_traits'

//  container_traits related functions -----------------------------------------//

		//! Free-standing size() function
		/*!
			Get the size of the container. Uses container_traits.
		*/
        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::size_type
        size( const C& c )
        {
            return container_traits<C>::function_type::size( c ); 
        }

		//! Free-standing empty() function
		/*!
			Check whenever the container is empty. Uses container traits.
		*/
	    template< typename C >
        inline bool empty( const C& c )
        {
            return container_traits<C>::function_type::empty( c );
        }

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING

		//! Free-standing begin() function
		/*!
			Get the begin iterator of the container. Uses container_traits.
			Variant for non-const input.
		*/
        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::iterator
        begin( C& c )
        {
            return container_traits<C>::function_type::begin( c ); 
        }

		//! Free-standing begin() function
		/*!
			Get the begin iterator of the container. Uses container_traits.
			Variant for const input.
		*/
        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::const_iterator
        begin( const C& c )
        {
            return container_traits<C>::function_type::begin( c ); 
        }

		//! Free-standing end() function
		/*!
			Get the begin iterator of the container. Uses container_traits.
			Variant for non-const input.
		*/
        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::iterator
        end( C& c )
        {
            return container_traits<C>::function_type::end( c );
        }

		//! Free-standing end() function
		/*!
			Get the begin iterator of the container. Uses container_traits.
			Variant for const input.
		*/
        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::const_iterator
        end( const C& c )
        {
            return container_traits<C>::function_type::end( c );
        }

#else // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

		//! Free-standing begin() function
		/*!
			Get the begin iterator of the container. Uses container_traits.
			This variant is provided for compilers without function 
			template ordering.
		*/
        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::result_iterator
        begin( C& c )
        {
            return container_traits<C>::function_type::begin( c );
        }

		//! Free-standing end() function
		/*!
			Get the begin iterator of the container. Uses container_traits.
			This variant is provided for compilers without function 
			template ordering.
		*/
        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::result_iterator
        end( C& c )
        {
            return container_traits<C>::function_type::end( c );
        }

#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
 
    } // namespace string_algo
} // namespace boost

#endif // BOOST_STRING_CONTAINER_TRAITS_HPP
