//  (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_CONTAINER_TRAITS_DETAIL_CONTAINER_TRAITS_HPP
#define BOOST_CONTIANER_TRAITS_DETAIL_CONTAINER_TRAITS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/container_traits/config.hpp>
#include <string>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/apply_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/detail/iterator.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>

namespace boost {
    namespace container_traits_detail {

        using type_traits::yes_type;
        using type_traits::no_type;
        
// Default container traits -----------------------------------------------------------------

        // Default container helper 
        /*
            Wraps std::container compliant containers
        */
        template< typename ContainerT >     
        struct default_container_traits
        {
            typedef BOOST_DEDUCED_TYPENAME ContainerT::value_type value_type;
            typedef BOOST_DEDUCED_TYPENAME ContainerT::iterator iterator;
            typedef BOOST_DEDUCED_TYPENAME ContainerT::const_iterator const_iterator;
            typedef BOOST_CT_DEDUCED_TYPENAME 
                ::boost::mpl::if_< ::boost::is_const<ContainerT>,
                    const_iterator,
                    iterator 
                >::type result_iterator;
            typedef BOOST_DEDUCED_TYPENAME ContainerT::difference_type difference_type;
            typedef BOOST_DEDUCED_TYPENAME ContainerT::size_type size_type;
            
            // static operations
            template< typename C >
            static size_type size( const C& c )
            {
                return c.size();
            }

            template< typename C >
            static bool empty( const C& c )
            {
                return c.empty();
            }

            template< typename C >
            static iterator begin( C& c )
            {
                return c.begin();
            }

            template< typename C >
            static const_iterator begin( const C& c )
            {
                return c.begin();
            }

            template< typename C >
            static iterator end( C& c )
            {
                return c.end();
            }

            template< typename C >
            static const_iterator end( const C& c )
            {
                return c.end();
            }

        }; 

        template<typename T>
        struct default_container_traits_selector
        {
            typedef default_container_traits<T> type;
        };

// Pair container traits ---------------------------------------------------------------------
                
        // pair selector
        template< typename T, typename U >
        yes_type is_pair_impl( const std::pair<T,U>* );
        no_type is_pair_impl( ... );

        template<typename T> struct is_pair
        {
        private:
            static T* t;
        public:
            BOOST_STATIC_CONSTANT( bool, value=
                sizeof(is_pair_impl(t))==sizeof(yes_type) );
        };

        // pair helper
        template< typename PairT >
        struct pair_container_traits
        {
            //
            // consider making struct cuttoff 
            //
            typedef BOOST_DEDUCED_TYPENAME PairT::first_type element_type;

            typedef BOOST_CT_DEDUCED_TYPENAME ::boost::detail::
                iterator_traits<element_type>::value_type value_type;
            typedef std::size_t size_type;
            typedef BOOST_CT_DEDUCED_TYPENAME ::boost::detail::
                iterator_traits<element_type>::difference_type difference_type;

            typedef element_type iterator;
            typedef element_type const_iterator;
            typedef element_type result_iterator;

            // static operations
            template< typename P >
            static size_type size( const P& p )
            {
                return std::distance( p.first, p.second );
            }

            template< typename P >
            static bool empty( const P& p )
            {
                return p.first==p.second;
            }

            template< typename P > 
            static const_iterator begin( const P& p )
            {
                return p.first;
            }

            template< typename P >
            static const_iterator end( const P& p )
            {
                return p.second;
            }
        }; // 'pair_container_helper'

        template<typename T>
        struct pair_container_traits_selector
        {
            typedef pair_container_traits<T> type;
        };

// Array container traits ---------------------------------------------------------------

#ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        // array traits ( partial specialization )
        template< typename T >
        struct array_traits;

        template< typename T, std::size_t sz >
        struct array_traits<T[sz]>
        {
            // typedef
            typedef T* iterator;
            typedef const T* const_iterator;
            typedef T value_type;
            typedef std::size_t size_type;
            typedef std::ptrdiff_t difference_type;

            // size of the array ( static );
            BOOST_STATIC_CONSTANT( size_type, array_size = sz );
        };

#else // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

        // array traits ( no partial specialization )
        /*
            without parial specialization we are able to
            provide support only for a limited number of
            types. Currently the primitive numeric types 
            are supported
        */
        template< typename T, typename BaseT >
        struct array_traits_impl
        {
            typedef BaseT value_type;
            typedef BaseT* iterator;
            typedef const BaseT* const_iterator;
            typedef std::size_t size_type;
            typedef std::ptrdiff_t difference_type;

            // size of the array
            BOOST_STATIC_CONSTANT( size_type, array_size = sizeof(T)/sizeof(BaseT) );
            BOOST_STATIC_ASSERT(( array_size > 0 ));
        };
        
        template< typename T, typename BaseT >
        struct array_traits_impl_selector
        {
            typedef array_traits_impl<T,BaseT> type;
        };

        struct array_traits_void
        {
            typedef void type;
        };

        template< typename T, typename BaseT >
        struct array_traits_cv_selector
        {
            typedef BOOST_CT_DEDUCED_TYPENAME 
                ::boost::mpl::apply_if< 
                    ::boost::is_convertible<T,BaseT*>,
                    array_traits_impl_selector<T,BaseT>,
                    ::boost::mpl::apply_if< 
                        ::boost::is_convertible<T,const BaseT*>,
                            array_traits_impl_selector<T, const BaseT>,
                            ::boost::mpl::apply_if< 
                                ::boost::is_convertible<T, volatile BaseT*>,
                                array_traits_impl_selector<T, volatile BaseT>,
                                array_traits_impl_selector<T, const volatile BaseT>
                            >
                        >
                >::type type;
        };

        template< typename T >
        struct array_traits_select
        {
            template< typename T1, typename T2 >
            struct apply
            {
                typedef BOOST_CT_DEDUCED_TYPENAME
                    ::boost::mpl::apply_if< 
                        ::boost::is_convertible<T,const volatile T2*>,
                        array_traits_cv_selector<T,T2>,
                        ::boost::mpl::identity<T1> >::type type;
            };
        };

        template< typename T >
        struct array_traits_selector 
        {
        private:
            // supported array base types
#ifndef BOOST_NO_INTRINSIC_WCHAR_T
            typedef BOOST_DEDUCED_TYPENAME
                ::boost::mpl::vector10<
                    wchar_t,
#else // BOOST_NO_INTRINSIC_WCHAR_T
            typedef BOOST_DEDUCED_TYPENAME
                ::boost::mpl::vector9<
#endif // BOOST_NO_INTRINSIC_WCHAR_T
                    char,
                    signed char,
                    unsigned char, // why 3 char types?
                    signed short,
                    unsigned short,
                    signed int,
                    unsigned int,
                    signed long,
                    unsigned long
            >::type array_base_types;

        public:
            typedef BOOST_DEDUCED_TYPENAME
                ::boost::mpl::fold<
                    array_base_types,
                    ::boost::string_algo::detail::array_traits_void,
                    ::boost::string_algo::detail::array_traits_select<T> >::type type;
        };

        template< typename T >
        struct array_traits
        {
            typedef BOOST_DEDUCED_TYPENAME
                array_traits_selector<T>::type traits_type;

            typedef BOOST_DEDUCED_TYPENAME
                traits_type::value_type value_type;
            typedef BOOST_DEDUCED_TYPENAME
                traits_type::iterator iterator;
            typedef BOOST_DEDUCED_TYPENAME
                traits_type::const_iterator const_iterator;
            typedef BOOST_DEDUCED_TYPENAME
                traits_type::size_type size_type;
            typedef BOOST_DEDUCED_TYPENAME
                traits_type::difference_type difference_type;

            BOOST_STATIC_CONSTANT( size_type, array_size = traits_type::array_size );
        };

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
        
        // array lenght resolving
        /*
            Lenght of string contained in a static array could
            be different from the size of the array.
            For string processing we need the lenght without
            terminating 0.

            Therefore, the lenght is calulated for char and wchar_t
            using char_traits, rather then simply returning
            the array size.
        */
        template< typename T >
        struct array_length_selector
        {
            template< typename TraitsT >
            struct array_length
            {
                typedef BOOST_DEDUCED_TYPENAME 
                    TraitsT::size_type size_type;

                BOOST_STATIC_CONSTANT(
                    size_type,
                    array_size=TraitsT::array_size );

                template< typename A >
                static size_type length( const A& )
                {
                    return array_size;
                }

                template< typename A >
                static bool empty( const A& )
                {
                    return array_size==0;
                }
            };
        };

        // specialization for char
        template<>
        struct array_length_selector<char>
        {
            template< typename TraitsT >
            struct array_length
            {
                typedef BOOST_DEDUCED_TYPENAME 
                    TraitsT::size_type size_type;

                template< typename A >
                static size_type length( const A& a )
                {
                    if( empty( a ) )
                        return 0;
                    return std::char_traits<char>::length(a);
                }
                
                template< typename A >
                static bool empty( const A& a )
                {
                    return a==0 || a[0]==0; 
                }
            };
        };

        // specialization for wchar_t
        template<>
        struct array_length_selector<wchar_t>
        {
            template< typename TraitsT >
            struct array_length
            {
                typedef BOOST_DEDUCED_TYPENAME 
                    TraitsT::size_type size_type;

                template< typename A >
                static size_type length( const A& a )
                {
                    if( empty( a ) ) 
                        return 0;
                    return std::char_traits<wchar_t>::length(a);
                }

                template< typename A >
                static bool empty( const A& a )
                {
                    return a==0 || a[0]==0; 
                }
            };
        };

        template< typename T >
        struct array_container_traits
        {
        private:
            // resolve array traits
            typedef array_traits<T>   traits_type;

        public:
            typedef BOOST_DEDUCED_TYPENAME
                traits_type::value_type value_type;
            typedef BOOST_DEDUCED_TYPENAME
                traits_type::iterator iterator;
            typedef BOOST_DEDUCED_TYPENAME
                traits_type::const_iterator const_iterator;
            typedef BOOST_DEDUCED_TYPENAME
                traits_type::size_type size_type;
            typedef BOOST_DEDUCED_TYPENAME
                traits_type::difference_type difference_type;

            typedef BOOST_CT_DEDUCED_TYPENAME
                ::boost::mpl::if_< ::boost::is_const<T>,
                    const_iterator,
                    iterator 
                >::type result_iterator;
            
        private:
            // resolve array size
            typedef BOOST_CT_DEDUCED_TYPENAME
                ::boost::remove_cv<value_type>::type char_type;
            typedef BOOST_DEDUCED_TYPENAME
                array_length_selector<char_type>::
                    BOOST_NESTED_TEMPLATE array_length<traits_type> array_length_type;

        public:
            BOOST_STATIC_CONSTANT( size_type, array_size = traits_type::array_size );

            // static operations
            template< typename A >
            static size_type size( const A& a )
            {
                return array_length_type::length(a);
            }

            template< typename A >
            static bool empty( const A& a )
            {
                return array_length_type::empty(a);
            }
            
            template< typename A >
            static iterator begin( A& a )
            {
                return a;
            }

            template< typename A >
            static const_iterator begin( const A& a )
            {
                return a;
            }

            template< typename A >
            static iterator end( A& a )
            {
                return a + array_length_type::length( a ); // note: problemematic if &a[0] since a can be 0
            }

            template< typename A >
            static const_iterator end( const A& a )
            {
                return a + array_length_type::length( a );
            }

        }; 

        template<typename T>
        struct array_container_traits_selector
        {
            typedef array_container_traits<T> type;
        };

// Pointer container traits ---------------------------------------------------------------

        template<typename T>
        struct pointer_container_traits
        {
#ifdef BOOST_NO_INTRINSIC_WCHAR_T  
            BOOST_STATIC_ASSERT(( ::boost::is_convertible<char*,T>::value ));
#else
            BOOST_STATIC_ASSERT(( ::boost::is_convertible<char*,T>::value ||
                                  ::boost::is_convertible<wchar_t*,T>::value ));             
#endif
            typedef BOOST_CT_DEDUCED_TYPENAME ::boost::remove_pointer<T>::type value_type;

            typedef BOOST_CT_DEDUCED_TYPENAME
                ::boost::remove_cv<value_type>::type char_type;
            typedef ::std::char_traits<char_type> char_traits;

            typedef value_type*       iterator;
            typedef const value_type* const_iterator;
            typedef std::ptrdiff_t    difference_type;
            typedef std::size_t       size_type;

            typedef BOOST_CT_DEDUCED_TYPENAME
                ::boost::mpl::if_< ::boost::is_const<T>,
                    const_iterator,
                    iterator 
                >::type              result_iterator;

            // static operations
            template< typename P >
            static size_type size( const P& p )
            {
                if ( empty( p ) )
                    return 0;
                else
                    return char_traits::length(p);
            }

            template< typename P >
            static bool empty( const P& p )
            {
                return p==0 || p[0]==0; 
            }

            template< typename P >
            static iterator begin( P& p )
            {
                return p;
            }

            template< typename P >
            static const_iterator begin( const P& p )
            {
                return p;
            }

            template< typename P >
            static iterator end( P& p )
            {
                if( empty( p ) )
                    return p;
                return p+char_traits::length(p);
            }

            template< typename P >
            static const_iterator end( const P& p )
            {
                if( empty( p ) )
                    return p;
                return p+char_traits::length(p);
            }
        }; 

        template<typename T>
        struct pointer_container_traits_selector
        {
            typedef pointer_container_traits<T> type;
        };

// Iterator container traits ---------------------------------------------------------------
        
#ifdef BOOST_MSVC_STD_ITERATOR
        template< typename T1, T2 >
        yes_type is_iterator_impl( const std::iterator<T2,T2>* );
#else        
#if BOOST_DINKUMWARE_STDLIB == 1
        template< typename C, typename T, typename D >
        yes_type is_iterator_impl( const std::iterator<C,T,D>* );
#else
        template< typename C, typename T, typename D, typename P, typename R >
        yes_type is_iterator_impl( const std::iterator<C,T,D,P,R>* );
#endif
#endif
        no_type  is_iterator_impl( ... ); 
        
        template< typename T >
        struct is_iterator
        {
        private:
            static T* t;
        public:
            BOOST_STATIC_CONSTANT( bool, value = sizeof( is_iterator_impl(t) ) 
                                   == sizeof( yes_type ) );
        };
        
        
        
        template< typename IteratorT >
        struct iterator_container_traits
        {
            typedef BOOST_DEDUCED_TYPENAME 
                detail::iterator_traits<IteratorT>::value_type  value_type;
            typedef IteratorT                                   iterator;
            typedef IteratorT                                   const_iterator;
            typedef BOOST_DEDUCED_TYPENAME
                detail::iterator_traits<IteratorT>::difference_type
                                                                difference_type;
            typedef std::size_t                                 size_type;
            typedef IteratorT                                   result_iterator;
            
            template< typename I >
            static inline iterator
            begin( const I& i )
            {
                return i;
            }
            
            template< typename I >
            static inline iterator
            end( const I& i )
            {
                return I();
            }
            
            template< typename I >
            static inline bool
            empty( const I& i )
            {
                return i == end( i );
            }
            
            template< typename I >
            static inline size_type 
            size( const I& i );

        };
        
        
        
        template<typename T>
        struct iterator_container_traits_selector
        {
            typedef iterator_container_traits<T> type;
        };

// sizer helper -------------------------------------------------------------------------
        
        template< std::size_t sz >
        class container_traits_size
        {
            char give_size[sz];
        };

    } // namespace 'container_traits_detail'
} // namespace 'boost'


#endif  
