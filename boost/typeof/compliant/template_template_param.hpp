// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TYPEOF_COMPLIANT_TEMPLATE_TEMPLATE_PARAM_HPP_INCLUDED
#define BOOST_TYPEOF_COMPLIANT_TEMPLATE_TEMPLATE_PARAM_HPP_INCLUDED

#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>

namespace boost
{
    namespace type_of 
    {
        namespace 
        {
            template<class V, class Type_Not_Registered_With_Typeof_System> struct encode_template_impl;
            template<class T, class Iter> struct decode_template_impl;
        }
        template<class V, class T> struct encode_template
            : encode_template_impl<V, T>
        {};

        template<class Iter> struct decode_template 
            :   decode_template_impl<typename mpl::deref<Iter>::type, typename mpl::next<Iter>::type>
        {};
    }
}

#define BOOST_TYPEOF_MAKE_OBJ_template(x)   BOOST_TYPEOF_TEMPLATE_PARAM(x)
#define BOOST_TYPEOF_TEMPLATE(X) template(X) BOOST_TYPEOF_EAT
#define BOOST_TYPEOF_template(X) (template(X))


#define BOOST_TYPEOF_TEMPLATE_PARAM(Type)\
    (TEMPLATE_PARAM)\
    (Type)

//Encode / decode this
#define BOOST_TYPEOF_ENCODE_TEMPLATE_PARAM(This, n)\
   typedef typename encode_template<BOOST_PP_CAT(V, n),\
       BOOST_PP_CAT(P, n)<BOOST_TYPEOF_SEQ_ENUM(BOOST_TYPEOF_PARAM_GETTYPE(This),BOOST_TYPEOF_PLACEHOLDER)>\
   >::type BOOST_PP_CAT(V, BOOST_PP_INC(n));

#define BOOST_TYPEOF_DECODE_TEMPLATE_PARAM(This, n)\
   typedef decode_template< BOOST_PP_CAT(iter, n) > BOOST_PP_CAT(d, n);\
   typedef typename BOOST_PP_CAT(d, n)::type BOOST_PP_CAT(P, n);\
   typedef typename BOOST_PP_CAT(d, n)::iter BOOST_PP_CAT(iter,BOOST_PP_INC(n));

//Expand argument
#define BOOST_TYPEOF_PARAM_EXPAND_TEMPLATE_PARAM(Param) \
    template <BOOST_TYPEOF_SEQ_ENUM(BOOST_TYPEOF_PARAM_GETTYPE(Param),BOOST_TYPEOF_REGISTER_TEMPLATE_PARAM_TYPE)> class

#define BOOST_TYPEOF_REGISTER_TEMPLATE_PARAM_TYPE(z,n,elem)\
    BOOST_TYPEOF_PARAM_EXPAND_TYPE(BOOST_TYPEOF_MAKE_OBJ(elem))

//Placeholder arguments to create a dummy template template.
#define BOOST_TYPEOF_PLACEHOLDER(z,n,elem)\
    BOOST_PP_CAT(BOOST_TYPEOF_PLACEHOLDER_, BOOST_PP_SEQ_ELEM(0, BOOST_TYPEOF_MAKE_OBJ(elem)))(BOOST_TYPEOF_MAKE_OBJ(elem))

#define BOOST_TYPEOF_PLACEHOLDER_TYPE_PARAM(Param) int
#define BOOST_TYPEOF_PLACEHOLDER_INTEGRAL_PARAM(Param) BOOST_TYPEOF_PARAM_GETTYPE(Param)(0)
#define BOOST_TYPEOF_PLACEHOLDER_TEMPLATE_PARAM(Param) Nested_Template_Template_Arguments_Not_Supported

//Template template registration
#define BOOST_TYPEOF_REGISTER_TYPE_FOR_TEMPLATE_TEMPLATE(Name,Params,ID)\
    template<class V\
        BOOST_TYPEOF_SEQ_ENUM_TRAILING(Params,BOOST_TYPEOF_REGISTER_TEMPLATE_PARAM_PAIR)\
    >\
    struct encode_template_impl<V,Name<\
        BOOST_PP_ENUM_PARAMS(\
        BOOST_PP_SEQ_SIZE(Params),\
        P)> >\
    :   BOOST_TYPEOF_PUSH_BACK<V, mpl::size_t<ID> >\
    {\
    };\
    template<class Iter> struct decode_template_impl<mpl::size_t<ID>, Iter>\
    {\
        BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(Params),BOOST_TYPEOF_TYPEDEF_INT_PN,_)\
        typedef Name<BOOST_TYPEOF_SEQ_ENUM(Params,BOOST_TYPEOF_PLACEHOLDER)> type;\
        typedef Iter iter;\
    };

#define BOOST_TYPEOF_TYPEDEF_INT_PN(z,n,Params) typedef int BOOST_PP_CAT(P,n);

#define BOOST_TYPEOF_REGISTER_NOTHING(Name,Params,ID)

//Template template param decoding
#define BOOST_TYPEOF_TYPEDEF_DECODED_TEMPLATE_TEMPLATE_TYPE(Name,Params)\
    template<BOOST_TYPEOF_SEQ_ENUM(Params,BOOST_TYPEOF_REGISTER_DECLARE_DECODER_TYPE_PARAM_PAIR)>\
    struct decode_params;\
    template<BOOST_TYPEOF_SEQ_ENUM(Params,BOOST_TYPEOF_REGISTER_DECODER_TYPE_PARAM_PAIR)>\
    struct decode_params<BOOST_TYPEOF_SEQ_ENUM(Params,BOOST_TYPEOF_PLACEHOLDER_TYPES)>\
    {\
        typedef Name<BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(Params),T)> type;\
    };\
    typedef typename decode_params<BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(Params),P)>::type type;

//'template<class,int> class' is reduced to 'class'
#define BOOST_TYPEOF_DECLARATION_TYPE(elem)\
    BOOST_PP_CAT(BOOST_TYPEOF_DECLARATION_TYPE_, BOOST_PP_SEQ_ELEM(0, elem))(elem)
#define BOOST_TYPEOF_DECLARATION_TYPE_TYPE_PARAM(Param) BOOST_TYPEOF_PARAM_GETTYPE(Param)
#define BOOST_TYPEOF_DECLARATION_TYPE_INTEGRAL_PARAM(Param) BOOST_TYPEOF_PARAM_GETTYPE(Param)
#define BOOST_TYPEOF_DECLARATION_TYPE_TEMPLATE_PARAM(Param) class

#define BOOST_TYPEOF_REGISTER_DECLARE_DECODER_TYPE_PARAM_PAIR(z,n,elem) \
    BOOST_TYPEOF_DECLARATION_TYPE(BOOST_TYPEOF_MAKE_OBJ(elem)) BOOST_PP_CAT(T, n)

#define BOOST_TYPEOF_PLACEHOLDER_TYPES(z,n,elem)\
    BOOST_PP_CAT(BOOST_TYPEOF_PLACEHOLDER_TYPES_, BOOST_PP_SEQ_ELEM(0, BOOST_TYPEOF_MAKE_OBJ(elem)))(n,BOOST_TYPEOF_MAKE_OBJ(elem))

#define BOOST_TYPEOF_PLACEHOLDER_TYPES_TYPE_PARAM(n,Param) BOOST_PP_CAT(T,n)
#define BOOST_TYPEOF_PLACEHOLDER_TYPES_INTEGRAL_PARAM(n,Param) BOOST_PP_CAT(T,n)
#define BOOST_TYPEOF_PLACEHOLDER_TYPES_TEMPLATE_PARAM(n,Param) BOOST_PP_CAT(T,n)<BOOST_TYPEOF_SEQ_ENUM(BOOST_TYPEOF_PARAM_GETTYPE(Param),BOOST_TYPEOF_PLACEHOLDER)>


//Category: Can this parameter be used in a template template class?
//1 -> Simple type or integral constant
//2 -> Template template type (can not be used to create template template template types :)

#define BOOST_TYPEOF_PARAM_TT_CATEGORY(Param) \
    BOOST_PP_CAT(BOOST_TYPEOF_TT_CATEGORY_,BOOST_PP_SEQ_ELEM(0, Param))
#define BOOST_TYPEOF_TT_CATEGORY_INTEGRAL_PARAM 1
#define BOOST_TYPEOF_TT_CATEGORY_TYPE_PARAM 1
#define BOOST_TYPEOF_TT_CATEGORY_EXPLICIT_INTEGRAL_PARAM 0
#define BOOST_TYPEOF_TT_CATEGORY_TEMPLATE_PARAM 2

#define BOOST_TYPEOF_CHECK_TT_CATEGORY(s, data, elem)\
    BOOST_PP_EQUAL(BOOST_TYPEOF_PARAM_TT_CATEGORY(BOOST_TYPEOF_MAKE_OBJ(elem)), data)

//Check if one of the arguments is a template template argument.
#define BOOST_TYPEOF_HAS_TEMPLATE_TEMPLATE_ARGUMENTS(Params)\
    BOOST_PP_NOT_EQUAL(\
        BOOST_PP_SEQ_SIZE(\
            (_)BOOST_PP_SEQ_FILTER(BOOST_TYPEOF_CHECK_TT_CATEGORY,2,Params)\
        ),\
        1\
    )

//Define template template arguments
#define BOOST_TYPEOF_REGISTER_TEMPLATE_TEMPLATE_IMPL(Name,Params,ID)\
    BOOST_PP_IF(BOOST_TYPEOF_HAS_TEMPLATE_TEMPLATE_ARGUMENTS(Params),\
        BOOST_TYPEOF_REGISTER_NOTHING,\
        BOOST_TYPEOF_REGISTER_TYPE_FOR_TEMPLATE_TEMPLATE)(Name,Params,ID)

#endif //BOOST_TYPEOF_COMPLIANT_TEMPLATE_TEMPLATE_PARAM_HPP_INCLUDED
