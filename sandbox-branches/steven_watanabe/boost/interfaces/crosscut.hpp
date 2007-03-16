// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#error Currently broken: null pointer is no longer in util.

#ifndef BOOST_IDL_CROSSCUT_HPP_INCLUDED
#define BOOST_IDL_CROSSCUT_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp> // BOOST_MSVC
#include <boost/interfaces/access.hpp>
#include <boost/interfaces/advice.hpp>
#include <boost/interfaces/advice_categories.hpp>
#include <boost/interfaces/aspect.hpp>
#include <boost/interfaces/detail/constants.hpp>
#include <boost/interfaces/detail/empty_base.hpp>
#include <boost/interfaces/detail/generate.hpp>
#include <boost/interfaces/detail/preprocessor/member_data.hpp>
#include <boost/interfaces/detail/util.hpp> // null pointer, disable_if_same.
#include <boost/mpl/aux_/na.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/limits/list.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace interfaces {          

namespace detail {

template<typename Aspects>
struct crosscut_impl {
    template< typename Derived, 
              typename Flags = flags::default_, 
              typename Base = detail::empty_base >
    struct generator_impl {
        struct merge {
            template<typename Aspect>
            struct apply {
                typedef typename Aspect::pointcut_type      pointcut_type;
                typedef typename
                        merge_advice<
                            typename Aspect::advice_type,
                            Base
                        >::type                             advice_type;
                typedef typename pointcut_type::template
                        generator<
                            Derived, 
                            Flags, 
                            advice_type
                        >                                   type;
            };
        };
        typedef typename 
                mpl::transform_view<
                    Aspects, merge
                >::type                         generators; 
        typedef typename 
                mpl::fold<
                    generators,
                    empty_base,
                    generate
                >::type                         type;
    };
      
    template< typename Derived, 
              typename Flags = flags::default_, 
              typename Base = detail::empty_base >
    struct generator 
        : generator_impl<Derived, Flags, Base>::type 
    { };
};

template<typename Aspects, typename Derived>
struct crosscut_traits {
    typedef typename 
            crosscut_impl<Aspects>::template 
            generator<Derived> 
    type;
};

} // End namespace detail.

//------------------Definition of class subinterface--------------------------//

#define BOOST_IDL_CROSSCUT_TEMPLATE_PARAMS() \
    typename Root, \
    BOOST_PP_ENUM_BINARY_PARAMS( BOOST_MPL_LIMIT_LIST_SIZE, \
                                 typename Aspect, \
                                 = mpl::na BOOST_PP_INTERCEPT ), \
                                 typename Dummy = mpl::na \
    /**/

#define BOOST_IDL_CROSSCUT_TEMPLATE_ARGS() \
    Root, BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_LIST_SIZE, Aspect), Dummy \
    /**/

#define BOOST_IDL_CROSSCUT_ASPECTS() \
    mpl::list<BOOST_PP_ENUM_PARAMS(BOOST_MPL_LIMIT_LIST_SIZE, Aspect)> \
    /**/

template<BOOST_IDL_CROSSCUT_TEMPLATE_PARAMS()>
class crosscut_interface
    : public detail::crosscut_traits<
                 BOOST_IDL_CROSSCUT_ASPECTS(),
                 crosscut_interface<BOOST_IDL_CROSSCUT_TEMPLATE_ARGS()>
             >::type        
{
public:
    typedef BOOST_IDL_CROSSCUT_ASPECTS() aspects;
    typedef typename 
            detail::crosscut_traits<
                 BOOST_IDL_CROSSCUT_ASPECTS(),
                 crosscut_interface<BOOST_IDL_CROSSCUT_TEMPLATE_ARGS()>
             >::type base_type;
    typedef typename base_type::interface_table      interface_table; 
    template<typename T>
    struct interface_functions
        : base_type::template interface_functions<T>,
          detail::table_holder<interface_table, T>
        { 
            using detail::table_holder<interface_table, T>::BOOST_IDL_MEMBER_TABLE;
        };
    template< typename Derived, 
              typename Flags = flags::default_, 
              typename Base = detail::empty_base >
    struct generator 
        : detail::crosscut_impl<aspects>::template 
              generator<Derived, Flags, Base>
        { };
    crosscut_interface() 
        : BOOST_IDL_MEMBER_PTR(0), 
          BOOST_IDL_MEMBER_TABLE(0) 
        { }
    template<typename T>
    crosscut_interface(T& t)
    {
        BOOST_IDL_MEMBER_PTR = &t;
        BOOST_IDL_MEMBER_TABLE = 
            &interface_functions<T>::BOOST_IDL_MEMBER_TABLE;
    }
    crosscut_interface& operator=(const crosscut_interface& rhs) 
    { 
        BOOST_IDL_MEMBER_PTR = rhs.BOOST_IDL_MEMBER_PTR;
        BOOST_IDL_MEMBER_TABLE = rhs.BOOST_IDL_MEMBER_TABLE; 
        return *this; 
    }
    crosscut_interface& operator=(detail::null_pointer_constant) 
    { reset(); return *this; }
    void reset() 
    { 
        BOOST_IDL_MEMBER_PTR = 0;
        BOOST_IDL_MEMBER_TABLE = 0;
    }
    operator const void*() const { return BOOST_IDL_MEMBER_TABLE; }
BOOST_IDL_PRIVATE:
    BOOST_IDL_MEMBER_DATA()
};

} } // End namespaces interfaces, boost.

#endif // #ifndef BOOST_IDL_CROSSCUT_HPP_INCLUDED
