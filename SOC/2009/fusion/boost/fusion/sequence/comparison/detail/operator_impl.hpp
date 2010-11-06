/*==============================================================================
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/fusion/support/internal/base.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/support/internal/constexpr.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template<typename Seq1Ref, typename Seq2Ref>
        struct BOOST_FUSION_OPERATOR_ABREV
        {
            typedef typename result_of::end<Seq1Ref>::type end1_type;

            template<typename It1, typename It2>
            static BOOST_FUSION_CONSTEXPR
            bool
            call(It1 const&, It2 const&, mpl::true_)
            {
#ifdef BOOST_FUSION_COMPARE_ALL
                return true;
#else
                return false;
#endif
            }

            template<typename It1, typename It2>
            static bool
            call(It1 const& it1, It2 const& it2, mpl::false_)
            {
                return fusion::deref(it1) BOOST_FUSION_OPERATOR
                        fusion::deref(it2)
#ifdef BOOST_FUSION_COMPARE_ALL
                        &&
#else
                        ||
#endif
                    call(fusion::next(it1), fusion::next(it2));
            }

            template<typename It1, typename It2>
            static bool
            call(It1 const& it1, It2 const& it2)
            {
                return call(
                    it1,
                    it2,
                    typename result_of::equal_to<
                        It1
                      , end1_type
                    >::type());
            }
        };
#ifndef __GNUC__
    }
#endif

        template<typename Seq1, typename Seq2>
        typename boost::enable_if<
           mpl::and_<
                traits::is_sequence<Seq1 const&>
              , traits::is_sequence<Seq2 const&>
              , mpl::equal_to<
                    result_of::size<Seq1 const&>
                  , result_of::size<Seq2 const&>
                >
            >
          , bool
        >::type
        operator BOOST_FUSION_OPERATOR(Seq1 const& seq1, Seq2 const& seq2)
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq1 const&>))
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq2 const&>))

            return detail::BOOST_FUSION_OPERATOR_ABREV<
                Seq1 const&
              , Seq2 const&
            >::call(fusion::begin(seq1), fusion::begin(seq2));
        }
#ifdef __GNUC__
    }

    //cschmidt: workaround conflict with the iterator operator== in gcc
    using detail::operator BOOST_FUSION_OPERATOR;
#endif


    template<typename Seq1, typename Seq2>
    bool
    BOOST_FUSION_OPERATOR_ABREV(Seq1 const& seq1, Seq2 const& seq2)
    {
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq1 const&>))
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq1 const&>))
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq2 const&>))
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq2 const&>))
        BOOST_FUSION_MPL_ASSERT((
            mpl::equal_to<
                result_of::size<Seq1 const&>
              , result_of::size<Seq2 const&>
            >))

        return detail::BOOST_FUSION_OPERATOR_ABREV<
            Seq1 const&
          , Seq2 const&
        >::call(fusion::begin(seq1), fusion::begin(seq2));
    }
}}
