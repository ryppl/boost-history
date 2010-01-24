/*=============================================================================
    Copyright (c) 2006 Eric Niebler
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_ALGORITHM_QUERY_EXT_FIND_IF_S_HPP
#define BOOST_FUSION_ALGORITHM_QUERY_EXT_FIND_IF_S_HPP

#include <boost/fusion/sequence/intrinsic/ext_/segments.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/container/list/cons.hpp>
#include <boost/fusion/algorithm/query/find_if.hpp>
#include <boost/fusion/view/ext_/segmented_iterator.hpp>
#include <boost/fusion/view/ext_/segmented_iterator_range.hpp>
#include <boost/fusion/support/ext_/is_segmented.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost { namespace fusion
{
    namespace detail
    {
        template<
            typename Seq
          , typename Where
          , bool IsSegmented = traits::is_segmented<Seq>::value
        >
        struct is_found;

        template<typename Seq,typename Where>
        struct is_found<Seq, Where,false>
          : mpl::not_<
                result_of::equal_to<
                    Where
                  , typename result_of::end<Seq>::type
                >
            >
        {};

        template<typename Seq, typename Cons>
        struct is_found<Seq, Cons, true>
          : mpl::not_<is_same<nil, Cons> >
        {};

        template<
            typename SegmentedRange
          , typename Where
          , typename Seq =
                typename detail::remove_reference<
                    typename result_of::deref<
                        typename SegmentedRange::iterator_type
                    >::type
                >::type
          , bool IsSegmented = traits::is_segmented<Seq>::value
        >
        struct as_segmented_cons;

        template<typename SegmentedRange, typename Where, typename Seq>
        struct as_segmented_cons<SegmentedRange, Where, Seq, false>
        {
            typedef
                cons<
                    SegmentedRange
                  , cons<segmented_range<Seq, Where, false> >
                >
            type;

            static type
            call(SegmentedRange const &range, Where const &where)
            {
                return fusion::make_cons(
                    range,
                    fusion::make_cons(
                        segmented_range<Seq, Where, false>(
                            fusion::deref(fusion::begin(range)),
                            where)
                    ));
            }
        };

        template<typename SegmentedRange, typename Where, typename Seq>
        struct as_segmented_cons<SegmentedRange, Where, Seq, true>
        {
            typedef cons<Seq, Where> type;

            static type
            call(Seq const& range, Where const& where)
            {
                return fusion::make_cons(range, where);
            }
        };

        template<
            typename Seq,
            typename Pred,
            bool IsSegmented = traits::is_segmented<Seq>::value
        >
        struct static_find_if_s_recurse;

        template<
            typename SegmentedRange
          , typename Pred
          , bool IsEmpty = is_empty<SegmentedRange>::value
        >
        struct static_find_if_s_seg
        {
            typedef typename
                detail::remove_reference<
                    typename result_of::deref<
                        typename SegmentedRange::iterator_type
                    >::type
                >::type
            segment;
            typedef static_find_if_s_recurse<segment, Pred> where;
            typedef is_found<segment, typename where::type> is_found_;
            typedef
                as_segmented_cons<SegmentedRange, typename where::type>
            found;
            typedef
                static_find_if_s_seg<
                    typename range_next<SegmentedRange>::type
                  , Pred
                >
            not_found;

            typedef typename
                mpl::eval_if<is_found_, found, not_found>::type
            type;

            static type
            call(SegmentedRange const& range)
            {
                return call_(range, is_found_());
            }

        private:
            static type call_(SegmentedRange const& range, mpl::true_)
            {
                return found::call(range, where::call(*range.where_));
            }

            static type call_(SegmentedRange const& range, mpl::false_)
            {
                return not_found::call(range_next<SegmentedRange>::call(range));
            }
        };

        template<
            typename SegmentedRange
          , typename Pred
        >
        struct static_find_if_s_seg<SegmentedRange, Pred, true>
        {
            typedef nil type;

            static type call(SegmentedRange const&)
            {
                return nil();
            }
        };

        template<typename Seq, typename Pred>
        struct static_find_if_s_recurse<Seq, Pred, true>
        {
            typedef
                static_find_if_s_seg<
                    typename as_segmented_range<Seq>::type,
                    Pred
                >
            find_if;

            typedef typename find_if::type type;

            static type
            call(Seq& seq)
            {
                return find_if::call(range(fusion::segments(seq)));
            }
        };

        template<typename Seq, typename Pred>
        struct static_find_if_s_recurse<Seq, Pred, false>
        {
            typedef typename result_of::find_if<Seq, Pred>::type type;

            static type
            call(Seq& seq)
            {
                return fusion::find_if<Pred>(seq);
            }
        };

        template<typename Sequence, typename Pred, bool IsSegmented = traits::is_segmented<Sequence>::value>
        struct static_find_if_s;

        template<typename Seq, typename Pred>
        struct static_find_if_s<Seq,Pred,false>
          : static_find_if_s_recurse<Seq, Pred, false>
        {};

        template<typename Seq, typename Pred>
        struct static_find_if_s<Seq, Pred, true>
        {
            typedef typename as_segmented_range<Seq>::type range;
            typedef static_find_if_s_recurse<Seq, Pred> find_if;
            typedef typename find_if::type found;

            typedef segmented_iterator<typename reverse_cons<found>::type> type;

            static type call(Seq& seq)
            {
                return type(reverse_cons<found>::call(find_if::call(seq)));
            }
        };
    }
    
    namespace result_of
    {
        template <typename Seq, typename Pred>
        struct find_if_s
          : detail::static_find_if_s<Seq, Pred>
        {};
    }

    template<typename Pred, typename Sequence>
    typename lazy_disable_if<
        is_const<Sequence>
      , result_of::find_if_s<Sequence, Pred>
    >::type
    find_if_s(Sequence& seq)
    {
        return detail::static_find_if_s<Sequence, Pred>::call(seq);
    }

    template <typename Pred, typename Sequence>
    typename result_of::find_if_s<Sequence const, Pred>::type
    find_if_s(Sequence const& seq)
    {
        return detail::static_find_if_s<Sequence const, Pred>::call(seq);
    }
}}

#endif
