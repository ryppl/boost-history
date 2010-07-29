/*=============================================================================
    Copyright (c) 2010 Stefan Strasser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef BOOST_ADAPTED_STRUCT_ADAPT_DERIVED_HPP
#define BOOST_ADAPTED_STRUCT_ADAPT_DERIVED_HPP

#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/int.hpp>

namespace boost{
namespace fusion{
namespace extension{
namespace detail{

//this is done while compiling, instead of preprocessing, because struct_size<Base>::value
//is not known to the preprocessor
template<int I,typename B0=void,typename B1=void,typename B2=void,typename B3=void,typename B4=void>
struct struct_base_member{
    typedef typename mpl::eval_if_c<
        I < struct_size<B0>::value,
        mpl::pair<B0,mpl::int_<I> >,
        struct_base_member<
            I - struct_size<B0>::value,
            B1,B2,B3,B4
        >
    >::type type;
};

}
}
}
}


#define BOOST_FUSION_ADAPT_DERIVED(name, dseq, bseq)                            \
    BOOST_FUSION_ADAPT_DERIVED_I(                                               \
        name, dseq, BOOST_PP_CAT(BOOST_FUSION_ADAPT_STRUCT_X bseq, 0))          \
    /***/


#define BOOST_FUSION_ADAPT_DERIVED_I(name, dseq, seq)                           \
    namespace boost { namespace fusion {                                        \
        BOOST_FUSION_ADAPT_STRUCT_H(                                            \
            name,                                                               \
            BOOST_FUSION_ADAPT_BASE_SIZE_PLUS(dseq) BOOST_PP_SEQ_SIZE(seq))     \
        namespace extension{                                                    \
            template <int I>                                                    \
            struct struct_member<name, I>                                       \
            {                                                                   \
                typedef typename detail::struct_base_member<                    \
                    I,BOOST_PP_SEQ_ENUM(dseq)                                   \
                >::type pair;                                                   \
                typedef typename pair::first Base;                              \
                typedef struct_member<Base,pair::second::value> base_member;    \
                typedef typename base_member::type type;                        \
                static type& call(name& struct_)                                \
                {                                                               \
                    Base &b=fusion::access::upcast<Base>(struct_);              \
                    return base_member::call(b);                                \
                };                                                              \
            };                                                                  \
        }                                                                       \
        BOOST_PP_SEQ_FOR_EACH_I(                                                \
            BOOST_FUSION_ADAPT_DERIVED_M,                                       \
            (name,BOOST_FUSION_ADAPT_BASE_SIZE_PLUS(dseq) 0), seq)              \
    }}                                                                          \
    /***/

#define BOOST_FUSION_ADAPT_BASE_SIZE_PLUS(seq)                                  \
    BOOST_PP_SEQ_FOR_EACH(BOOST_FUSION_ADAPT_BASE_SIZE_E,0,seq)                 \
    /***/

#define BOOST_FUSION_ADAPT_BASE_SIZE_E(r,data,elem)                             \
    extension::struct_size<elem>::value +                                       \
    /***/

#define BOOST_FUSION_ADAPT_DERIVED_M(r, tuple, i, xy)                           \
    BOOST_FUSION_ADAPT_STRUCT_C(r,BOOST_PP_TUPLE_ELEM(2,0,tuple),                      \
        i + BOOST_PP_TUPLE_ELEM(2,1,tuple),xy)                                  \
    /***/


#endif

