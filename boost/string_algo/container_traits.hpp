//  Boost string_algo library container_traits.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_CONTAINER_TRAITS_HPP
#define BOOST_STRING_CONTAINER_TRAITS_HPP

#include <boost/config.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/bool_c.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/list.hpp>
#include <boost/string_algo/detail/yes_no_type.hpp>

// Forward definitions for std containers
namespace std {
    template<typename T, typename TraitsT, typename AllocT> class basic_string;
    template<typename T, typename AllocT> class list;
	template<typename T, typename AllocT > class rope;
	template<typename T, typename AllocT > class slist;
}

namespace boost {

    namespace string_algo{

//  contatiner traits  -----------------------------------------------//

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

//  contatiner traits definition -------------------------------------//

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
            typedef boost::mpl::false_c native_replace;
            // insert/erase methods preserve iterators
            typedef boost::mpl::false_c stable_iterators;
            // const-time insert method
            typedef boost::mpl::false_c const_time_insert;
            // const-time erase method
            typedef boost::mpl::false_c const_time_erase;
        };      


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
            typedef boost::mpl::false_c native_replace;
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
            typedef boost::mpl::true_c native_replace;
            typedef boost::mpl::false_c stable_iterators;
            typedef boost::mpl::false_c const_time_insert;
            typedef boost::mpl::false_c const_time_erase;
        };

        // SGI's rope container traits
        template< typename T, typename AllocT >
        struct container_traits< std::rope<T,AllocT> >
        {
            // Input types
            typedef std::rope<T,AllocT> type;
            typedef typename type::iterator iterator_type;
            typedef typename type::const_iterator const_iterator_type;
            typedef type& reference_type;
            typedef const type& const_reference_type;

            // Container features
            typedef boost::mpl::true_c native_replace;
            typedef boost::mpl::true_c stable_iterators;
            typedef boost::mpl::true_c const_time_insert;
            typedef boost::mpl::true_c const_time_erase;
        };

        // SGI's slist container traits
        template< typename T, typename AllocT >
        struct container_traits< std::slist<T,AllocT> >
        {
            // Input types
            typedef std::slist<T,AllocT> type;
            typedef typename type::iterator iterator_type;
            typedef typename type::const_iterator const_iterator_type;
            typedef type& reference_type;
            typedef const type& const_reference_type;

            // Container features
            typedef boost::mpl::false_c native_replace;
            typedef boost::mpl::true_c stable_iterators;
            typedef boost::mpl::true_c const_time_insert;
            typedef boost::mpl::true_c const_time_erase;
        };

#else // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION       

//  contatiner traits definition -------------------------------------//

        // Generic container traits
        template< typename ContainerT >
        struct default_container_traits
        {
            // Input types
            typedef ContainerT type;
            typedef typename type::iterator iterator_type;
            typedef typename type::const_iterator const_iterator_type;
            typedef type& reference_type;
            typedef const type& const_reference_type;

            // Container features
            typedef boost::mpl::false_c native_replace;
            typedef boost::mpl::false_c stable_iterators;
            typedef boost::mpl::false_c const_time_insert;
            typedef boost::mpl::false_c const_time_erase;
        };      


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
            typedef boost::mpl::false_c native_replace;
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
            typedef boost::mpl::true_c native_replace;
            typedef boost::mpl::false_c stable_iterators;
            typedef boost::mpl::false_c const_time_insert;
            typedef boost::mpl::false_c const_time_erase;
        };

		// Rope container traits
		/*
			Traits for rope like containers
		*/
		template< typename ContainerT >
        struct rope_container_traits
        {
            // Input types
            typedef ContainerT type;
            typedef typename type::iterator iterator_type;
            typedef typename type::const_iterator const_iterator_type;
            typedef type& reference_type;
            typedef const type& const_reference_type;

            // Container features
            typedef boost::mpl::true_c native_replace;
            typedef boost::mpl::true_c stable_iterators;
            typedef boost::mpl::true_c const_time_insert;
            typedef boost::mpl::true_c const_time_erase;
        };

//  contatiner traits selection implemetaion ------------------------------//

        namespace detail {          
            
            // list selection --------------------------------------------//
            template< typename T, typename AllocT >
            yes_type is_list_tester( const std::list<T,AllocT>& );
			template< typename T, typename AllocT >
			yes_type is_list_tester( const std::slist<T,AllocT>& );
            no_type is_list_tester(...);

            struct list_traits_selector
            {
                template< typename T >
                struct selector
                {
                    static T& t;
                    BOOST_STATIC_CONSTANT(bool, 
                        value=( sizeof(is_list_tester(t))==sizeof(yes_type) ) );

                    // Container traits
                    typedef list_container_traits<T> traits; 
                };
            };

            // basic_string selection ------------------------------------//
            template< typename T, typename TraitsT, typename AllocT >
            yes_type is_string_tester( const std::basic_string<T,TraitsT,AllocT>& );
            no_type is_string_tester(...);
            
            struct string_traits_selector
            {
                template< typename T >
                struct selector
                {
                    static T& t;
                    BOOST_STATIC_CONSTANT(bool, 
                        value=( sizeof(is_string_tester(t))==sizeof(yes_type) ) );
                
                    // Container traits
                    typedef string_container_traits<T> traits; 
                };
            };

            // rope selection ------------------------------------//
            template< typename T, typename TraitsT, typename AllocT >
            yes_type is_rope_tester( const std::rope<T,AllocT>& );
            no_type is_rope_tester(...);
            
            struct rope_traits_selector
            {
                template< typename T >
                struct selector
                {
                    static T& t;
                    BOOST_STATIC_CONSTANT(bool, 
                        value=( sizeof(is_rope_tester(t))==sizeof(yes_type) ) );
                
                    // Container traits
                    typedef rope_container_traits<T> traits; 
                };
            };

			// default selector ----------------------------------------//
            // this selector should be last in the selector list
            struct default_traits_selector
            {
                template< typename T >
                struct selector
                {
                    BOOST_STATIC_CONSTANT(bool, value=true );

                    // Default container traits
                    typedef default_container_traits<T> traits; 
                };
            };
            
            // Selector predicate metaclass  ----------------------------------//
            template< typename T>
            struct is_selected
            {
                template< typename SelectorT >
                struct apply
                {
                    typedef SelectorT::template selector<T> selector;

                    BOOST_STATIC_CONSTANT(bool, value=( selector::value ) );
                    typedef boost::mpl::bool_c<value> type;
                };
            };

            // Container traits selection implementation -----------------------//    
            
            // Traits selector
            template< typename T, typename SelectorList >
            struct container_traits_selector
            {
            private:
                typedef SelectorList selectors;

                typedef typename boost::mpl::find_if< 
                    selectors,
                    is_selected<T> >::type iter;
                
                typedef typename iter::type selector;
                typedef typename selector::template selector<T> selector_type;

            public:
                typedef typename selector_type::traits type;
            };                  
        
            // Traits selectors list
            typedef boost::mpl::list4< 
                string_traits_selector,
                list_traits_selector,
				rope_traits_selector,
                default_traits_selector >::type container_traits_selectors;

        } // namespace detail

        // Generic container traits
        template< typename ContainerT >
        struct container_traits
        {
        private:
            // Container traits resolving
            typedef typename detail::container_traits_selector<
                ContainerT,
                detail::container_traits_selectors >::type traits_type;

        public:
            // Input types
            typedef typename traits_type::type type;
            typedef typename traits_type::iterator_type iterator_type;
            typedef typename traits_type::const_iterator_type const_iterator_type;
            typedef typename traits_type::reference_type reference_type;
            typedef typename traits_type::const_reference_type const_reference_type;

            // Container features

            typedef typename traits_type::native_replace native_replace;
            typedef typename traits_type::stable_iterators stable_iterators;
            typedef typename traits_type::const_time_insert const_time_insert;
            typedef typename traits_type::const_time_erase const_time_erase;                
        };      

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION       

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_CONTAINER_TRAITS_HPP
