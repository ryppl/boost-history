/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman
    Copyright (c) 2007 Dan Marsden
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ADAPTED_STRUCT_ADAPT_ASSOC_STRUCT_HPP
#define BOOST_FUSION_ADAPTED_STRUCT_ADAPT_ASSOC_STRUCT_HPP

#include <boost/fusion/support/internal/ref.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <boost/fusion/adapted/detail/struct/extension.hpp>
#include <boost/fusion/adapted/detail/struct/adapt_base.hpp>
#include <boost/fusion/adapted/detail/struct/at_impl.hpp>
#include <boost/fusion/adapted/detail/struct/is_view_impl.hpp>
#include <boost/fusion/adapted/detail/struct/is_sequence_impl.hpp>
#include <boost/fusion/adapted/detail/struct/value_at_impl.hpp>
#include <boost/fusion/adapted/detail/struct/category_of_impl.hpp>
#include <boost/fusion/adapted/detail/struct/size_impl.hpp>
#include <boost/fusion/adapted/detail/struct/begin_impl.hpp>
#include <boost/fusion/adapted/detail/struct/end_impl.hpp>
#include <boost/fusion/adapted/detail/struct/value_of_impl.hpp>
#include <boost/fusion/adapted/detail/struct/deref_impl.hpp>
#include <boost/fusion/adapted/detail/struct/deref_data_impl.hpp>
#include <boost/fusion/adapted/detail/struct/key_of_impl.hpp>
#include <boost/fusion/adapted/detail/struct/value_of_data_impl.hpp>

#define BOOST_FUSION_ADAPT_ASSOC_STRUCT_FILLER_0(X, Y, Z)\
    ((X, Y, Z)) BOOST_FUSION_ADAPT_ASSOC_STRUCT_FILLER_1
#define BOOST_FUSION_ADAPT_ASSOC_STRUCT_FILLER_1(X, Y, Z)\
    ((X, Y, Z)) BOOST_FUSION_ADAPT_ASSOC_STRUCT_FILLER_0
#define BOOST_FUSION_ADAPT_ASSOC_STRUCT_FILLER_0_END
#define BOOST_FUSION_ADAPT_ASSOC_STRUCT_FILLER_1_END

#define BOOST_FUSION_ADAPT_ASSOC_STRUCT(NAME, SEQ)\
    BOOST_FUSION_ADAPT_STRUCT_BASE(\
        NAME,\
        assoc_struct_tag,\
        BOOST_PP_CAT(BOOST_FUSION_ADAPT_ASSOC_STRUCT_FILLER_0 SEQ,_END),\
        BOOST_FUSION_ADAPT_ASSOC_STRUCT_C)

#define BOOST_FUSION_ADAPT_ASSOC_STRUCT_C(R, NAME, I, XYZ)                      \
    BOOST_FUSION_ADAPT_STRUCT_C_BASE(NAME, I, XYZ, 3)                           \
                                                                                \
namespace boost { namespace fusion { namespace extension                        \
{                                                                               \
    template<>                                                                  \
    struct struct_assoc_key<NAME, I>                                            \
    {                                                                           \
        typedef BOOST_PP_TUPLE_ELEM(3, 2, XYZ) type;                            \
    };                                                                          \
}}}

#endif
