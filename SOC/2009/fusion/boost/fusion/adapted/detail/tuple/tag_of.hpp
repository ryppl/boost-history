/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/config.hpp>
#include <boost/fusion/support/tag_of_fwd.hpp>

#include <boost/mpl/tag.hpp>
#ifdef BOOST_FUSION_ADAPTED_STD_TUPLE
#   ifdef BOOST_NO_0X_HDR_TUPLE
#       include <boost/preprocessor/enum.hpp>

#       ifndef BOOST_FUSION_STD_TR1_TUPLE_UPPER_LIMIT
#           define BOOST_FUSION_STD_TR1_TUPLE_UPPER_LIMIT 10
#       endif
#   endif
#endif

namespace boost
{
    namespace fusion
    {
        struct std_tuple_tag;
        struct std_tuple_iterator_tag;
        struct boost_tuple_tag;
        struct boost_tuple_iterator_tag;
        struct fusion_sequence_tag;
        struct random_access_traversal_tag;

        namespace traits
        {
#ifdef BOOST_FUSION_ADAPTED_STD_TUPLE
#   ifndef BOOST_NO_0X_HDR_TUPLE
#       define BOOST_FUSION_TUPLE_TAG_OF_SPECIALIZATION(MODIFIER,_)\
            template<typename... Types>\
            struct tag_of<std::tuple<Types...> MODIFIER>\
            {\
                typedef std_tuple_tag type;\
            };
#   else
#       define BOOST_FUSION_TUPLE_TAG_OF_SPECIALIZATION(MODIFIER,_)\
            template<\
                BOOST_PP_ENUM(BOOST_FUSION_STD_TR1_TUPLE_UPPER_LIMIT,typename T)\
            >\
            struct tag_of<\
                std::tr1::tuple<\
                    BOOST_PP_ENUM(BOOST_FUSION_STD_TR1_TUPLE_UPPER_LIMIT,T)\
                > MODIFIER\
            >\
            {\
                typedef std_tuple_tag type;\
            };
#   endif
#else
#   ifdef BOOST_NO_PARTIAL_SPECIALIZATION_IMPLICIT_DEFAULT_ARGS
#       define BOOST_FUSION_TUPLE_TAG_OF_SPECIALIZATION(MODIFIER,_)\
            template <\
                class T0, class T1, class T2, class T3, class T4, \
                class T5, class T6, class T7, class T8, class T9\
            >\
            struct tag_of<\
                tuples::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> MODIFIER\
              , void\
            >\
            {\
                typedef boost_tuple_tag type;\
            };\
            \
            template <class Head, class Tail>\
            struct tag_of<tuples::cons<Head, Tail> MODIFIER,void>\
            {\
                typedef boost_tuple_tag type;\
            };\
            \
            template <>\
            struct tag_of<tuples::null_type MODIFIER,void>\
            {\
                typedef boost_tuple_tag type;\
            };
#   else
#       define BOOST_FUSION_TUPLE_TAG_OF_SPECIALIZATION(MODIFIER,_)\
            template <\
                class T0, class T1, class T2, class T3, class T4, \
                class T5, class T6, class T7, class T8, class T9\
            >\
            struct tag_of<\
                tuples::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> MODIFIER\
            >\
            {\
                typedef boost_tuple_tag type;\
            };\
            \
            template <class Head, class Tail>\
            struct tag_of<tuples::cons<Head, Tail> MODIFIER>\
            {\
                typedef boost_tuple_tag type;\
            };\
            \
            template <>\
            struct tag_of<tuples::null_type MODIFIER>\
            {\
                typedef boost_tuple_tag type;\
            };
#   endif
#endif

        BOOST_FUSION_ALL_CV_REF_NON_REF_COMBINATIONS(
                BOOST_FUSION_TUPLE_TAG_OF_SPECIALIZATION,_)

#undef BOOST_FUSION_TUPLE_TAG_OF_SPECIALIZATION
        }
    }

    namespace mpl
    {
        template<typename>
        struct sequence_tag;

#ifdef BOOST_FUSION_ADAPTED_STD_TUPLE
#   ifndef BOOST_NO_0X_HDR_TUPLE
        template<typename... Types>
        struct sequence_tag<std::tuple<Types...> >
        {
            typedef fusion::fusion_sequence_tag type;
        };

        template<typename... Types>
        struct sequence_tag<std::tuple<Types...> const>
        {
            typedef fusion::fusion_sequence_tag type;
        };
#   else
        template<
            BOOST_PP_ENUM(BOOST_FUSION_STD_TR1_TUPLE_UPPER_LIMIT,typename T)
        >
        struct sequence_tag<
            std::tr1::tuple<
                BOOST_PP_ENUM(BOOST_FUSION_STD_TR1_TUPLE_UPPER_LIMIT,T)
            >
        >
        {
            typedef fusion::fusion_sequence_tag type;
        };

        template<
            BOOST_PP_ENUM(BOOST_FUSION_STD_TR1_TUPLE_UPPER_LIMIT,typename T)
        >
        struct sequence_tag<
            std::tr1::tuple<
                BOOST_PP_ENUM(BOOST_FUSION_STD_TR1_TUPLE_UPPER_LIMIT,T)
            > const
        >
        {
            typedef fusion::fusion_sequence_tag type;
        };
#   endif
#else
        template <
            class T0, class T1, class T2, class T3, class T4,
            class T5, class T6, class T7, class T8, class T9
        >
        struct sequence_tag<
            tuples::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>
        >
        {
            typedef fusion::fusion_sequence_tag type;
        };

        template <
            class T0, class T1, class T2, class T3, class T4,
            class T5, class T6, class T7, class T8, class T9
        >
        struct sequence_tag<
            tuples::tuple<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> const
        >
        {
            typedef fusion::fusion_sequence_tag type;
        };

        template <class Head, class Tail>
        struct sequence_tag<tuples::cons<Head, Tail> >
        {
            typedef fusion::fusion_sequence_tag type;
        };

        template <class Head, class Tail>
        struct sequence_tag<tuples::cons<Head, Tail> const>
        {
            typedef fusion::fusion_sequence_tag type;
        };

        template <>
        struct sequence_tag<tuples::null_type>
        {
            typedef fusion::fusion_sequence_tag type;
        };

        template <>
        struct sequence_tag<tuples::null_type const>
        {
            typedef fusion::fusion_sequence_tag type;
        };
#endif
    }
}

