//  Boost string_algo library container_traits.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_CONTAINER_TRAITS_HPP
#define BOOST_STRING_CONTAINER_TRAITS_HPP

#include <boost/string_algo/config.hpp>
#include <list>
#include <string>
#include <vector>
#include <deque>
#include <boost/mpl/bool_c.hpp>

namespace boost {

    namespace string_algo{

//  contatiner traits  -----------------------------------------------//

        // Generic container traits
        template< typename ContainerT >
        struct container_traits
        {
            // Input types
            typedef ContainerT type;
            typedef typename type::iterator iterator_type;
            typedef typename type::const_iterator const_iterator_type;
            typedef type& reference_type;
            typedef const type& const_reference_type;

            // Container features

            // Container has basic_string like replace function
            typedef boost::mpl::false_c has_replace;
            // insert/erase methods preserve iterators
            typedef boost::mpl::false_c stable_iterators;
            // const-time insert method
            typedef boost::mpl::false_c const_time_insert;
            // const-time erase method
            typedef boost::mpl::false_c const_time_erase;
        };      

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

        // List container traits
        /*
            Traits for list-like containers
        */
        template< typename ContainerT >
        struct list_container_traits    
        {
            // Input types
            typedef ContainerT type;
            typedef typename type::iterator iterator_type;
            typedef typename type::const_iterator const_iterator_type;
            typedef type& reference_type;
            typedef const type& const_reference_type;

            // Container features
            typedef boost::mpl::false_c has_replace;
            typedef boost::mpl::true_c stable_iterators;
            typedef boost::mpl::true_c const_time_insert;
            typedef boost::mpl::true_c const_time_erase;
        };

        // basic_string container traits
        /*
            Traits for string like containers
        */
        template< typename ContainerT >
        struct string_container_traits
        {
            // Input types
            typedef ContainerT type;
            typedef typename type::iterator iterator_type;
            typedef typename type::const_iterator const_iterator_type;
            typedef type& reference_type;
            typedef const type& const_reference_type;

            // Container features
            typedef boost::mpl::true_c has_replace;
            typedef boost::mpl::false_c stable_iterators;
            typedef boost::mpl::false_c const_time_insert;
            typedef boost::mpl::false_c const_time_erase;
        };

#else // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION       

        // List container traits
        template< typename T, typename AllocT >
        struct container_traits< std::list<T,AllocT> >
        {
            // Input types
            typedef std::list<T,AllocT> type;
            typedef typename type::iterator iterator_type;
            typedef typename type::const_iterator const_iterator_type;
            typedef type& reference_type;
            typedef const type& const_reference_type;

            // Container features
            typedef boost::mpl::false_c has_replace;
            typedef boost::mpl::true_c stable_iterators;
            typedef boost::mpl::true_c const_time_insert;
            typedef boost::mpl::true_c const_time_erase;
        };

        // basic_string container traits
        template< typename CharT, typename TraitsT, typename AllocT >
        struct container_traits< std::basic_string<CharT, TraitsT, AllocT> >
        {
            // Input types
            typedef std::basic_string<CharT, TraitsT, AllocT> type;
            typedef typename type::iterator iterator_type;
            typedef typename type::const_iterator const_iterator_type;
            typedef type& reference_type;
            typedef const type& const_reference_type;

            // Container features
            typedef boost::mpl::true_c has_replace;
            typedef boost::mpl::false_c stable_iterators;
            typedef boost::mpl::false_c const_time_insert;
            typedef boost::mpl::false_c const_time_erase;
        };

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

//  contatiner categorization function  --------------------------------//

        namespace detail {

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

            template< typename T, typename AllocT >
            list_container_traits< std::list<T,AllocT> > 
            container_traits_select( const std::list<T,AllocT>& )
            {
                return list_container_traits< std::list<T,AllocT> >();
            }

            template< typename CharT, typename TraitsT, typename AllocT >
            string_container_traits< std::basic_string<CharT, TraitsT, AllocT> > 
            container_traits_select( const std::basic_string<CharT, TraitsT, AllocT>& )
            {
                return string_container_traits< std::basic_string<CharT, TraitsT, AllocT> >();
            }

            template< typename T, typename AllocT >
            container_traits< std::vector<T,AllocT> > 
            container_traits_select( const std::vector<T,AllocT>& )
            {
                return container_traits< std::vector<T,AllocT> >();
            }

            template< typename T, typename AllocT >
            container_traits< std::deque<T,AllocT> > 
                container_traits_select( const std::deque<T,AllocT>& )
            {
                return container_traits< std::deque<T,AllocT> >();
            }

#else // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION       

            template< typename ContainerT >
            container_traits<ContainerT> 
            container_traits_select( ContainerT& )
            {
                return container_traits<ContainerT>();
            }

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION      

//  contatiner traits selectors  --------------------------------//

#define BOOST_STRING_CONTAINER_TRAIT_SELECT( T0 ) \
            template< typename TraitsT > \
            inline typename TraitsT::T0 T0##_select( const TraitsT& ) \
            { \
                typedef typename TraitsT::T0 trait_type; \
                return trait_type(); \
            }

            BOOST_STRING_CONTAINER_TRAIT_SELECT(has_replace)
            BOOST_STRING_CONTAINER_TRAIT_SELECT(stable_iterators)
            BOOST_STRING_CONTAINER_TRAIT_SELECT(const_time_insert)
            BOOST_STRING_CONTAINER_TRAIT_SELECT(const_time_erase)

#undef BOOST_STRING_CONTAINER_TRAIT_SELECT
        
        } // namespace detail

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_CONTAINER_TRAITS_HPP
