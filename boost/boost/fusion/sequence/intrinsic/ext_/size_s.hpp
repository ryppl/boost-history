/*=============================================================================
    Copyright (c) 2006 Eric Niebler

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_SIZE_S_08112006_1141)
#define FUSION_SIZE_S_08112006_1141

#include <boost/mpl/plus.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/fusion/algorithm/iteration/fold.hpp>
#include <boost/fusion/support/ext_/is_segmented.hpp>
#include <boost/fusion/sequence/intrinsic/ext_/segments.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    // calculates the size of any segmented data structure.
    template<typename Sequence, bool IsSegmented = traits::is_segmented<Sequence>::value>
    struct segmented_size;

    namespace detail
    {
        struct size_plus
        {
            template<typename Seq, typename State>
            struct result
              : mpl::plus<
                    segmented_size<typename remove_reference<Seq>::type>
                  , State
                >
            {};
        };
    }

    ///////////////////////////////////////////////////////////////////////////
    template<typename Sequence, bool IsSegmented>
    struct segmented_size
      : result_of::fold<
            typename result_of::segments<Sequence>::type
          , mpl::size_t<0>
          , detail::size_plus
        >::type
    {};

    template<typename Sequence>
    struct segmented_size<Sequence, false>
      : result_of::size<Sequence>
    {};
}}

#endif
