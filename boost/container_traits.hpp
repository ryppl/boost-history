//  (C) Copyright Pavol Droba and Thorsten Ottosen 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

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

#ifndef BOOST_CONTAINER_TRAITS_HPP
#define BOOST_CONTAINER_TRAITS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/mpl/apply_if.hpp>

#include <boost/container_traits/detail/container_traits.hpp>

namespace boost {

        ///////////////////////////////////////////////////////////////////////////
        // container_traits
        ///////////////////////////////////////////////////////////////////////////
        template< typename T >
        struct container_traits
        {
        private:
            // see if this can be cut off?
            //
            // struct container_helper_type : ...
            // 
            typedef BOOST_CT_DEDUCED_TYPENAME ::boost::mpl::apply_if< 
                    ::boost::container_traits_detail::is_pair<T>, 
                        ::boost::container_traits_detail::pair_container_traits_selector<T>,
                        BOOST_CT_DEDUCED_TYPENAME ::boost::mpl::apply_if<
                        ::boost::is_array<T>,
                            ::boost::container_traits_detail::array_container_traits_selector<T>,
                            BOOST_CT_DEDUCED_TYPENAME ::boost::mpl::apply_if<
                            ::boost::is_pointer<T>,
                                ::boost::container_traits_detail::pointer_container_traits_selector<T>,
                                BOOST_CT_DEDUCED_TYPENAME ::boost::mpl::apply_if<
                                ::boost::container_traits_detail::is_iterator<T>,
                                    ::boost::container_traits_detail::iterator_container_traits_selector<T>,
                                    ::boost::container_traits_detail::default_container_traits_selector<T>
                                >
                            >   
                        > 
                >::type container_helper_type;
        public:
            typedef container_helper_type __function_type;        
            typedef BOOST_DEDUCED_TYPENAME 
                container_helper_type::value_type value_type;
            typedef BOOST_DEDUCED_TYPENAME 
                container_helper_type::size_type size_type;
            typedef BOOST_DEDUCED_TYPENAME 
                container_helper_type::iterator iterator;
            typedef BOOST_DEDUCED_TYPENAME 
                container_helper_type::const_iterator const_iterator;
            typedef BOOST_DEDUCED_TYPENAME 
                container_helper_type::result_iterator result_iterator;
            typedef BOOST_DEDUCED_TYPENAME 
                container_helper_type::difference_type difference_type;

        }; // 'container_traits'

        ///////////////////////////////////////////////////////////////////////////
        // empty()/size()/begin()/end() functions
        ///////////////////////////////////////////////////////////////////////////

        template< typename C >
        inline BOOST_DEDUCED_TYPENAME container_traits<C>::size_type
        size( const C& c )
        {
            return container_traits<C>::__function_type::size( c ); 
        }

        template< typename C >
        inline bool 
        empty( const C& c )
        {
            return container_traits<C>::__function_type::empty( c );
        }

#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING

        template< typename C >
        inline BOOST_DEDUCED_TYPENAME container_traits<C>::iterator
        begin( C& c )
        {
            return container_traits<C>::__function_type::begin( c ); 
        }

        template< typename C >
        inline BOOST_DEDUCED_TYPENAME container_traits<C>::const_iterator
        begin( const C& c )
        {
            return container_traits<C>::__function_type::begin( c ); 
        }

        template< typename C >
        inline BOOST_DEDUCED_TYPENAME container_traits<C>::iterator
        end( C& c )
        {
            return container_traits<C>::__function_type::end( c );
        }

        template< typename C >
        inline BOOST_DEDUCED_TYPENAME container_traits<C>::const_iterator
        end( const C& c )
        {
            return container_traits<C>::__function_type::end( c );
        }

#else // BOOST_NO_FUNCTION_TEMPLATE_ORDERING

        template< typename C >
        inline BOOST_DEDUCED_TYPENAME container_traits<C>::result_iterator
        begin( C& c )
        {
            return container_traits<C>::__function_type::begin( c );
        }

        template< typename C >
        inline BOOST_DEDUCED_TYPENAME container_traits<C>::result_iterator
        end( C& c )
        {
            return container_traits<C>::__function_type::end( c );
        }

#endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
 
        template< typename T, std::size_t sz >
        container_traits_detail::container_traits_size<sz>
        sizer( const T (&array)[sz] );

        template< typename T, std::size_t sz >
        container_traits_detail::container_traits_size<sz>
        sizer( T (&array)[sz] );

} // namespace boost

#endif
