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
    This facility is used to unify the access to different types of containers.
    It allows the algoritms in the library to work with STL containers, c-style
    array, null-terminated c-strings (and more) using the same interface.
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
                    ( i.e. \c std::vector<>, \c std::list<>, \c std::string<> ... )
                - c-style array 
                   ( \c char[10], \c int[15] ... )
                - null-terminated c-strings
                    ( \c char*, \c wchar_T* )
                - std::pair of iterators 
                    ( i.e \c std::pair<vector<int>::iterator,vector<int>::iterator> )

            Container traits provide a supset of container interface operations.
            All are accessible using free-standing functions.

            Following operations are supported:
                - \c size()
                - \c empty()
                - \c begin()
                - \c end()

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

//  container_traits metafunctions -----------------------------------------//

        //! Container value_type trait
        /*!
            Extract the type of elemets conained in a container
        */
        template< typename C >
        struct container_value_type
        {
            typedef BOOST_STRING_TYPENAME container_traits<C>::value_type type;
        };
        
        //! Container difference trait
        /*!
            Extract the container's difference type
        */
        template< typename C >
        struct container_difference_type
        {
            typedef BOOST_STRING_TYPENAME container_traits<C>::difference_type type;
        };

        //! Container iterator trait
        /*!
            Extract the container's iterator type
        */
        template< typename C >
        struct container_iterator
        {
            typedef BOOST_STRING_TYPENAME container_traits<C>::iterator type;
        };

        //! Container const_iterator trait
        /*!
            Extract the container's const_iterator type
        */
        template< typename C >
        struct container_const_iterator
        {
            typedef BOOST_STRING_TYPENAME container_traits<C>::const_iterator type;
        };


        //! Container result_iterator
        /*!
            Extract the container's result_iterator type. This type maps to \c C::iterator
            for mutable container and \c C::const_iterator for const containers.
        */
        template< typename C >
        struct container_result_iterator
        {
            typedef BOOST_STRING_TYPENAME container_traits<C>::result_iterator type;
        };

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
        */
        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::iterator
        begin( C& c )
        {
            return container_traits<C>::function_type::begin( c ); 
        }

        //! Free-standing begin() function
        /*!
            \overload
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
        */
        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::iterator
        end( C& c )
        {
            return container_traits<C>::function_type::end( c );
        }

        //! Free-standing end() function
        /*!
            \overload           
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
            \overload
        */
        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::result_iterator
        begin( C& c )
        {
            return container_traits<C>::function_type::begin( c );
        }

        //! Free-standing end() function
        /*!
            \overload
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
