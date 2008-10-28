/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
    Copyright 2008 Stjepan Rajko
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#ifndef BOOST__PARAMETER__AUX__DELAYED_CONSTRUCTOR_HPP
#   ifndef BOOST_PP_IS_ITERATING

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>

#include <boost/mpl/bool.hpp>

#define BOOST_PARAMETER_MAX_DELAYED_CONSTRUCTOR_ARITY 3

namespace boost { namespace parameter { namespace aux {
    
    struct void_;
    
    template<typename T>
    struct is_delayed_constructor
        : public mpl::false_
    {};

    template< typename T, BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_PP_INC(BOOST_PARAMETER_MAX_DELAYED_CONSTRUCTOR_ARITY), typename T, void_) >
    struct delayed_constructor;

    template< typename T, BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_DELAYED_CONSTRUCTOR_ARITY, typename T)>
    struct is_delayed_constructor<delayed_constructor<T, BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_DELAYED_CONSTRUCTOR_ARITY, T)> >
        : public mpl::true_
    {};

    template< typename T >
    struct value_type_of
    {};
    
    template< typename T, BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_DELAYED_CONSTRUCTOR_ARITY, typename T)>
    struct value_type_of<delayed_constructor<T, BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_DELAYED_CONSTRUCTOR_ARITY, T)> >
    {
        typedef typename delayed_constructor<T, BOOST_PP_ENUM_PARAMS(BOOST_PARAMETER_MAX_DELAYED_CONSTRUCTOR_ARITY, T)>::value_type type;
    };

#     define BOOST_PP_FILENAME_1 <boost/parameter/aux_/delayed_constructor.hpp>
#     define BOOST_PP_ITERATION_LIMITS (1,BOOST_PARAMETER_MAX_DELAYED_CONSTRUCTOR_ARITY)
#     include BOOST_PP_ITERATE()

} } }

#     define BOOST__PARAMETER__AUX__DELAYED_CONSTRUCTOR_HPP
#   else // defined(BOOST_PP_IS_ITERATING)
#     define N BOOST_PP_ITERATION()
    template< typename T, BOOST_PP_ENUM_PARAMS(N, typename T) >
    struct delayed_constructor< T, BOOST_PP_ENUM_PARAMS(N, T) >
    {
        typedef T value_type;
        
        delayed_constructor(const delayed_constructor &rhs)
#   define TEMP_INIT(z,n,text) t ## n (rhs.t ## n)
            : BOOST_PP_ENUM(N, TEMP_INIT, _)
#   undef TEMP_INIT
        {}
        
        delayed_constructor(BOOST_PP_ENUM_BINARY_PARAMS(N,const T,& a))
#   define TEMP_INIT(z,n,text) t ## n (a ## n)
            : BOOST_PP_ENUM(N, TEMP_INIT, _)
#   undef TEMP_INIT
        {}
        
        operator T() const
        {
            return T(BOOST_PP_ENUM_PARAMS(N, t));
        }
        
#   define TEMP_DECL(z,n,text) const T ## n & t ## n;
        BOOST_PP_REPEAT(N, TEMP_DECL, _)
#   undef TEMP_DECL
    };

#     undef N
#   endif // defined(BOOST_PP_IS_ITERATING)

#endif // include guard

