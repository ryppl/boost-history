//  (C) Copyright Thorsten Ottosen 2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_CONTAINER_TRAITS_DETAIL_COMMON_HPP
#define BOOST_CONTAINER_TRAITS_DETAIL_COMMON_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/container_traits/config.hpp>
#include <boost/container_traits/detail/sfinae.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/type_traits/detail/ice_or.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <cstddef>

//////////////////////////////////////////////////////////////////////////////
// missing partial specialization  workaround.
//////////////////////////////////////////////////////////////////////////////

namespace boost 
{
    namespace container_traits_detail 
    {        
        // 1 = std containers
        // 2 = std::pair
        // 3 = const std::pair
        // 4 = array
        // 5 = const array
        // 6 = char*
        // 7 = const char*
        // 8 = whar_t*
        // 9 = const wchar_t*
        // 10 = iterator
        // 11 = string
        
        typedef mpl::int_<1>::type    std_container_;
        typedef mpl::int_<2>::type    std_pair_;
        typedef mpl::int_<3>::type    const_std_pair_;
        typedef mpl::int_<4>::type    array_;
        typedef mpl::int_<5>::type    const_array_;
        typedef mpl::int_<6>::type    char_ptr_;
        typedef mpl::int_<7>::type    const_char_ptr_;
        typedef mpl::int_<8>::type    wchar_t_ptr_;
        typedef mpl::int_<9>::type    const_wchar_t_ptr_;
        typedef mpl::int_<10>::type   iterator_;
        typedef mpl::int_<11>::type   string_;
        
        template< typename C >
        struct container_helper
        {
            static C* c;
            static C  ptr;

            BOOST_STATIC_CONSTANT( bool, is_pair_                = sizeof( ::boost::container_traits_detail::is_pair_impl( c ) ) == sizeof( yes_type ) );
            BOOST_STATIC_CONSTANT( bool, is_char_ptr_            = sizeof( ::boost::container_traits_detail::is_char_ptr_impl( ptr ) ) == sizeof( yes_type ) );
            BOOST_STATIC_CONSTANT( bool, is_const_char_ptr_      = sizeof( ::boost::container_traits_detail::is_const_char_ptr_impl( ptr ) ) == sizeof( yes_type ) );
            BOOST_STATIC_CONSTANT( bool, is_wchar_t_ptr_         = sizeof( ::boost::container_traits_detail::is_wchar_t_ptr_impl( ptr ) ) == sizeof( yes_type ) );
            BOOST_STATIC_CONSTANT( bool, is_const_wchar_t_ptr_   = sizeof( ::boost::container_traits_detail::is_const_wchar_t_ptr_impl( ptr ) ) == sizeof( yes_type ) );
            BOOST_STATIC_CONSTANT( bool, is_string_              = (::boost::type_traits::ice_or<is_const_char_ptr_, is_const_wchar_t_ptr_>::value ));
            BOOST_STATIC_CONSTANT( bool, is_array_               = ::boost::is_array<C>::value );
            BOOST_STATIC_CONSTANT( bool, is_iterator_            = sizeof( ::boost::container_traits_detail::is_iterator_impl( c ) ) == sizeof( yes_type ) );
            
        };
        
        template< typename C >
        class container
        {
            /*
            typedef BOOST_CT_DEDUCED_TYPENAME   ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_pair_,
                                                                    ::boost::container_traits_detail::std_pair_,
                                 BOOST_CT_DEDUCED_TYPENAME ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_char_ptr_,
                                                                      ::boost::container_traits_detail::char_ptr_,
                                            BOOST_CT_DEDUCED_TYPENAME ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_const_char_ptr_,
                                                                                 ::boost::container_traits_detail::const_char_ptr_,
                                                       BOOST_CT_DEDUCED_TYPENAME ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_wchar_t_ptr_,
                                                                                            ::boost::container_traits_detail::wchar_t_ptr_,
                                                                  BOOST_CT_DEDUCED_TYPENAME ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_const_wchar_t_ptr_,
                                                                                                       ::boost::container_traits_detail::const_wchar_t_ptr_,
                                                                             BOOST_CT_DEDUCED_TYPENAME ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_string_,
                                                                                                                  ::boost::container_traits_detail::string_,
                                                                                        BOOST_CT_DEDUCED_TYPENAME ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_array_,
                                                                                                                             ::boost::container_traits_detail::array_,
                                                                                                   BOOST_CT_DEDUCED_TYPENAME ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_iterator_,
                                                                                                                                        ::boost::container_traits_detail::iterator_,
                                                                                                                                        ::boost::container_traits_detail::std_container_
                                                                                                                                      >::type
                                                                                                                           >::type
                                                                                                                >::type
                                                                                                     >::type
                                                                                          >::type
                                                                               >::type
                                                                    >::type
                                                         >::type type;
                                    };
              */
                                  
            typedef BOOST_CT_DEDUCED_TYPENAME   ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_pair_,
                                                                    ::boost::container_traits_detail::std_pair_,
                                                                    void >::type pair_t;
            typedef BOOST_CT_DEDUCED_TYPENAME   ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_array_,
                                                                    ::boost::container_traits_detail::array_,
                                                                    pair_t >::type array_t;
            typedef BOOST_CT_DEDUCED_TYPENAME   ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_string_,
                                                                    ::boost::container_traits_detail::string_,
                                                                    array_t >::type string_t;
            typedef BOOST_CT_DEDUCED_TYPENAME   ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_const_char_ptr_,
                                                                    ::boost::container_traits_detail::const_char_ptr_,
                                                                    string_t >::type const_char_ptr_t;
            typedef BOOST_CT_DEDUCED_TYPENAME   ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_char_ptr_,
                                                                    ::boost::container_traits_detail::char_ptr_,
                                                                    const_char_ptr_t >::type char_ptr_t;
            typedef BOOST_CT_DEDUCED_TYPENAME   ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_const_wchar_t_ptr_,
                                                                    ::boost::container_traits_detail::const_wchar_t_ptr_,
                                                                    char_ptr_t >::type const_wchar_ptr_t;
            typedef BOOST_CT_DEDUCED_TYPENAME   ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_wchar_t_ptr_,
                                                                    ::boost::container_traits_detail::wchar_t_ptr_,
                                                                    const_wchar_ptr_t >::type wchar_ptr_t;
            typedef BOOST_CT_DEDUCED_TYPENAME   ::boost::mpl::if_c< ::boost::container_traits_detail::container_helper<C>::is_iterator_,
                                                                    ::boost::container_traits_detail::iterator_,
                                                                    wchar_ptr_t >::type iterator_t;  
        public:
            typedef BOOST_CT_DEDUCED_TYPENAME   ::boost::mpl::if_c< ::boost::is_void<iterator_t>::value,
                                                                    ::boost::container_traits_detail::std_container_, 
                                                                    iterator_t >::type type;
            
        }; // class 'container' 
    }
}
        
#endif
