/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2006 Dan Marsden

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_ZIP_VIEW_ZIP_VIEW_HPP
#define BOOST_FUSION_VIEW_ZIP_VIEW_ZIP_VIEW_HPP

#include <boost/fusion/sequence/intrinsic/end.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/algorithm/query/find_if.hpp>
#include <boost/fusion/iterator/equal_to.hpp>
#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/algorithm/transformation/remove.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/unused.hpp>
#include <boost/fusion/support/ref.hpp>

#include <boost/mpl/not.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/eval_if.hpp>

#include <boost/integer_traits.hpp>
#include <boost/type_traits/is_reference.hpp>

#include <boost/fusion/view/detail/strictest_traversal.hpp>
#include <boost/fusion/view/zip_view/detail/zip_view_fwd.hpp>
#include <boost/fusion/view/zip_view/detail/size_impl.hpp>
#include <boost/fusion/view/zip_view/detail/at_impl.hpp>
#include <boost/fusion/view/zip_view/detail/value_at_impl.hpp>
#include <boost/fusion/view/zip_view/detail/zip_view_iterator.hpp>

#include <boost/fusion/view/zip_view/detail/begin_impl.hpp>
#include <boost/fusion/view/zip_view/detail/end_impl.hpp>
#include <boost/fusion/view/zip_view/detail/deref_impl.hpp>
#include <boost/fusion/view/zip_view/detail/next_impl.hpp>
#include <boost/fusion/view/zip_view/detail/prior_impl.hpp>
#include <boost/fusion/view/zip_view/detail/advance_impl.hpp>
#include <boost/fusion/view/zip_view/detail/distance_impl.hpp>
#include <boost/fusion/view/zip_view/detail/value_of_impl.hpp>
#include <boost/fusion/view/zip_view/detail/equal_to_impl.hpp>

namespace boost { namespace fusion {

    namespace detail
    {
        template<typename Seqs>
        struct all_references
          : fusion::result_of::equal_to<
                typename fusion::result_of::find_if<
                    Seqs, mpl::not_<is_lrref<mpl::_> >
                >::type
              , typename fusion::result_of::end<Seqs>::type
            >
        {};

        struct seq_size
        {
            template<typename Params>
            struct result;

            template<typename Self,typename Seq>
            struct result<Self(Seq)>
            {
                typedef typename
                    mpl::eval_if<
                        traits::is_forward<Seq>
                      , result_of::size<Seq>
                      , mpl::identity<
                            mpl::int_<integer_traits<int>::const_max>
                        >
                    >::type
                type;
            };
        };

        struct poly_min
        {
            template<typename T>
            struct result;

            template<typename Self,typename MinSize, typename SeqSize>
            struct result<Self(MinSize, SeqSize)>
              : mpl::min<
                    typename detail::remove_reference<MinSize>::type,
                    typename detail::remove_reference<SeqSize>::type
                >
            {
            };
        };

        template<typename Seqs>
        struct min_size
        {
            typedef typename
                result_of::transform<Seqs, detail::seq_size>::type
            sizes;

            typedef typename
                result_of::fold<
                    sizes
                  , typename result_of::front<sizes>::type
                  , detail::poly_min
                >::type
            type;
        };
    }

    struct zip_view_tag;
    struct fusion_sequence_tag;

    template<typename Seqs>
    struct zip_view
      : sequence_base< zip_view<Seqs> >
    {
        //TODO cschmidt: replace view& with view
        typedef typename
            result_of::remove<Seqs, unused_type const&>::type
        real_seqs;
        BOOST_MPL_ASSERT((detail::all_references<Seqs>));

        typedef typename
            detail::strictest_traversal<real_seqs>::type
        category;
        typedef zip_view_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef mpl::true_ is_view;
        typedef typename
            mpl::eval_if<
                result_of::size<Seqs>
              , detail::min_size<real_seqs>
              , mpl::identity<mpl::int_<0> >
            >::type
        size;

        typedef typename
            fusion::result_of::as_vector<Seqs>::type
        seqs_type;

#define ZIP_VIEW_CTOR(COMBINATION,_)\
        template<typename OtherSeqs>\
        zip_view(zip_view<OtherSeqs> COMBINATION view)\
          : seqs(sequence_assign(BOOST_FUSION_FORWARD(\
                zip_view<OtherSeqs> COMBINATION,view).seqs))\
        {}

        BOOST_FUSION_ALL_CTOR_COMBINATIONS(ZIP_VIEW_CTOR,_)

#undef ZIP_VIEW_CTOR

        template<typename OtherSeqs>
        explicit zip_view(BOOST_FUSION_R_ELSE_CLREF(OtherSeqs) seqs)
          : seqs(fusion::sequence_assign(
              BOOST_FUSION_FORWARD(OtherSeqs,seqs)))
        {}

        template<typename OtherZipView>
        zip_view&
        operator=(BOOST_FUSION_R_ELSE_CLREF(OtherZipView) other_zip_view)
        {
            seqs=BOOST_FUSION_FORWARD(OtherZipView,other_zip_view).seqs;
            return *this;
        }

        seqs_type seqs;
    };
}}

#endif
