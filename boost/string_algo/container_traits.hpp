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
//  of container_traits addapter fow usage with string_algo library

#ifndef BOOST_STRING_CONTAINER_TRAITS_HPP
#define BOOST_STRING_CONTAINER_TRAITS_HPP

#include <boost/string_algo/config.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/mpl/apply_if.hpp>

// Implementation
#include <boost/string_algo/detail/container_traits.hpp>

namespace boost {
    namespace string_algo {

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
        //
        // Valid free standing functions:
        //------------------------------------------
        // iterator         begin( Container& );
        // conat_iterator   begin( const Container& );
        // iterator         end( Container& );  
        // const_iterator   end( const Container& );
        // size_type        size( const Container& )
        //

        ///////////////////////////////////////////////////////////////////////////
        // container_traits
        ///////////////////////////////////////////////////////////////////////////
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
            typedef container_helper_type function_type;        
            typedef BOOST_STRING_TYPENAME 
                container_helper_type::value_type value_type;
            typedef BOOST_STRING_TYPENAME 
                container_helper_type::size_type size_type;
            typedef BOOST_STRING_TYPENAME 
                container_helper_type::iterator iterator;
            typedef BOOST_STRING_TYPENAME 
                container_helper_type::const_iterator const_iterator;
            typedef BOOST_STRING_TYPENAME 
                container_helper_type::result_iterator result_iterator;
            typedef BOOST_STRING_TYPENAME 
                container_helper_type::difference_type difference_type;

        }; // 'container_traits'

        ///////////////////////////////////////////////////////////////////////////
        // empty()/size()/begin()/end() functions
        ///////////////////////////////////////////////////////////////////////////

        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::size_type
        size( const C& c )
        {
            return container_traits<C>::function_type::size( c ); 
        }

        template< typename C >
        inline bool empty( const C& c )
        {
            return container_traits<C>::function_type::empty( c );
        }

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING

        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::iterator
        begin( C& c )
        {
            return container_traits<C>::function_type::begin( c ); 
        }

        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::const_iterator
        begin( const C& c )
        {
            return container_traits<C>::function_type::begin( c ); 
        }

        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::iterator
        end( C& c )
        {
            return container_traits<C>::function_type::end( c );
        }

        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::const_iterator
        end( const C& c )
        {
            return container_traits<C>::function_type::end( c );
        }

#else // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

        template< typename C >
        inline BOOST_STRING_TYPENAME container_traits<C>::result_iterator
        begin( C& c )
        {
            return container_traits<C>::function_type::begin( c );
        }

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
