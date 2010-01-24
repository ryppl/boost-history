/*=============================================================================
    Copyright (c) 2006 Eric Niebler
    Copyright (c) 2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_EXT_SEGMENTED_ITERATOR_HPP
#define BOOST_FUSION_VIEW_EXT_SEGMENTED_ITERATOR_HPP

#include <boost/fusion/sequence/intrinsic/ext_/segments.hpp>
#include <boost/fusion/sequence/intrinsic/empty.hpp>
#include <boost/fusion/container/list/cons.hpp>
#include <boost/fusion/container/generation/make_cons.hpp>
#include <boost/fusion/iterator/distance.hpp>
#include <boost/fusion/view/filter_view.hpp>
#include <boost/fusion/support/tag_of.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/support/ext_/is_segmented.hpp>
#include <boost/fusion/support/internal/ref.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/placeholders.hpp>

namespace boost { namespace fusion
{
    struct fusion_sequence_tag;
    struct segmented_range_tag;

    namespace detail
    {
        template<typename Sequence, typename Iterator, bool IsSegmented>
        struct segmented_range
          : sequence_base<segmented_range<Sequence, Iterator, IsSegmented> >
        {
            typedef mpl::bool_<IsSegmented> is_segmented;
            // If this is a range of segments, skip over the empty ones
            typedef typename
                mpl::if_c<
                    IsSegmented
                  , filter_view<Sequence, mpl::not_<result_of::empty<mpl::_1> > >
                  , Sequence
                >::type
            sequence_non_ref_type;

            typedef segmented_range_tag fusion_tag;
            typedef fusion_sequence_tag tag;
            typedef mpl::true_ is_view;
            typedef Iterator iterator_type;

            typedef typename mpl::if_<
                traits::is_view<sequence_non_ref_type>
              , sequence_non_ref_type
              , sequence_non_ref_type &
            >::type sequence_type;

            typedef typename traits::category_of<sequence_non_ref_type>::type category;

            explicit segmented_range(Sequence &sequence_)
              : sequence(sequence_type(sequence_))
              , where_(fusion::begin(sequence))
            {}

            segmented_range(sequence_type sequence_, iterator_type const &wh)
              : sequence(sequence_)
              , where_(wh)
            {}

            sequence_type sequence;
            iterator_type where_;
        };
    }

    namespace extension
    {
        template<>
        struct is_segmented_impl<segmented_range_tag>
        {
            template<typename Sequence>
            struct apply
              : Sequence::is_segmented
            {};
        };

        template<>
        struct size_impl<segmented_range_tag>
        {
            template<typename Sequence>
            struct apply
            {
                  typedef typename detail::remove_reference<Sequence>::type seq;

                  typedef
                      result_of::distance<
                          typename seq::iterator_type
                        , typename result_of::end<typename seq::sequence_non_ref_type>::type
                      >
                  type;
            };
        };

        template<>
        struct segments_impl<segmented_range_tag>
        {
            template<typename Sequence>
            struct apply
            {
                typedef Sequence &type;
                static type call(Sequence &seq)
                {
                    return seq;
                }
            };
        };

        template<>
        struct begin_impl<segmented_range_tag>
        {
            template<typename Sequence>
            struct apply
            {
                typedef typename detail::remove_reference<Sequence>::type seq;

                typedef typename seq::iterator_type type;

                static type call(Sequence seq)
                {
                    return seq.where_;
                }
            };
        };

        template<>
        struct end_impl<segmented_range_tag>
        {
            template<typename Sequence>
            struct apply
            {
                typedef typename detail::remove_reference<Sequence>::type seq;

                typedef typename seq::sequence_non_ref_type sequence;
                typedef typename result_of::end<sequence>::type type;

                static type call(Sequence seq)
                {
                    return fusion::end(seq.sequence);
                }
            };
        };
    }

    namespace detail
    {
        ///////////////////////////////////////////////////////////////////////
        template<typename Range>
        struct range_next;

        template<typename Sequence, typename Iterator, bool IsSegmented>
        struct range_next<segmented_range<Sequence, Iterator, IsSegmented> >
        {
            typedef typename result_of::next<Iterator>::type iterator_type;
            typedef segmented_range<Sequence, iterator_type, IsSegmented> type;

            static type call(segmented_range<Sequence, Iterator, IsSegmented> const &rng)
            {
                return type(rng.sequence, fusion::next(rng.where_));
            }
        };

        ///////////////////////////////////////////////////////////////////////
        template<typename Cons>
        struct is_range_next_empty
          : result_of::empty<typename range_next<typename Cons::car_type>::type>
        {};

        template<>
        struct is_range_next_empty<nil>
          : mpl::true_
        {};

        ///////////////////////////////////////////////////////////////////////
        template<typename Sequence, bool IsSegmented = traits::is_segmented<Sequence>::value>
        struct as_segmented_range
        {
            typedef typename result_of::segments<Sequence>::type segments;
            typedef typename remove_reference<segments>::type sequence;
            typedef typename result_of::begin<filter_view<sequence, mpl::not_<result_of::empty<mpl::_1> > > >::type begin;
            typedef segmented_range<sequence, begin, true> type;

            static type call(Sequence &seq)
            {
                segments segs(fusion::segments(seq));
                return type(segs);
            }
        };

        template<typename Sequence>
        struct as_segmented_range<Sequence, false>
        {
            typedef typename remove_reference<Sequence>::type sequence;
            typedef typename result_of::begin<sequence>::type begin;
            typedef segmented_range<sequence, begin, false> type;

            static type call(Sequence &seq)
            {
                return type(seq);
            }
        };

        template<typename Sequence, typename Iterator, bool IsSegmented>
        struct as_segmented_range<segmented_range<Sequence, Iterator, IsSegmented>, IsSegmented>
        {
            typedef segmented_range<Sequence, Iterator, IsSegmented> type;
            static type &call(type &seq)
            {
                return seq;
            }
        };

        ///////////////////////////////////////////////////////////////////////
        template<
            typename Sequence
          , typename State = nil
          , bool IsSegmented = traits::is_segmented<Sequence>::value
        >
        struct push_segments;

        template<typename Sequence, typename State>
        struct push_segments<Sequence, State, true>
        {
            typedef typename as_segmented_range<Sequence>::type range;
            typedef typename result_of::begin<range>::type begin;
            typedef typename result_of::deref<begin>::type next_ref;
            typedef typename remove_reference<next_ref>::type next;
            typedef push_segments<next, cons<range, State> > push;
            typedef typename push::type type;

            static type call(Sequence &seq, State const &state)
            {
                range rng(as_segmented_range<Sequence>::call(seq));
                next_ref nxt(*fusion::begin(rng));
                return push::call(nxt, fusion::make_cons(rng, state));
            }
        };

        template<typename Sequence, typename State>
        struct push_segments<Sequence, State, false>
        {
            typedef typename as_segmented_range<Sequence>::type range;
            typedef cons<range, State> type;

            static type call(Sequence &seq, State const &state)
            {
                range rng(as_segmented_range<Sequence>::call(seq));
                return fusion::make_cons(rng, state);
            }
        };

        ///////////////////////////////////////////////////////////////////////
        template<typename State, bool IsEmpty = is_range_next_empty<State>::value>
        struct pop_segments
        {
            typedef range_next<typename State::car_type> next;
            typedef push_segments<typename next::type, typename State::cdr_type> push;
            typedef typename push::type type;

            static type call(State const &state)
            {
                typename next::type rng(next::call(state.car));
                return push::call(rng, state.cdr);
            }
        };

        template<typename State>
        struct pop_segments<State, true>
        {
            typedef pop_segments<typename State::cdr_type> pop;
            typedef typename pop::type type;

            static type call(State const &state)
            {
                return pop::call(state.cdr);
            }
        };

        template<>
        struct pop_segments<nil, true>
        {
            typedef nil type;

            static type call(nil const &)
            {
                return nil();
            }
        };
    } // namespace detail

    struct segmented_iterator_tag;

    ////////////////////////////////////////////////////////////////////////////
    template<typename Cons>
    struct segmented_iterator
      : fusion::iterator_base<segmented_iterator<Cons> >
    {
        typedef segmented_iterator_tag fusion_tag;
        typedef fusion::forward_traversal_tag category;

        typedef Cons cons_type;
        typedef typename Cons::car_type car_type;
        typedef typename Cons::cdr_type cdr_type;

        explicit segmented_iterator(Cons const &c)
          : cons_(c)
        {}

        cons_type const &cons() const { return this->cons_; };
        car_type const &car() const { return this->cons_.car; };
        cdr_type const &cdr() const { return this->cons_.cdr; };

    private:
        Cons cons_;
    };

    ///////////////////////////////////////////////////////////////////////////
    template<typename Sequence>
    struct segmented_begin
    {
        typedef typename detail::push_segments<Sequence> push;
        typedef segmented_iterator<typename push::type> type;

        static type call(Sequence &seq)
        {
            return type(push::call(seq, nil()));
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    template<typename Sequence>
    struct segmented_end
    {
        typedef segmented_iterator<nil> type;

        static type call(Sequence &)
        {
            return type(nil());
        }
    };

    namespace extension
    {
        template<>
        struct equal_to_impl<segmented_iterator_tag>
        {
            template<typename It1, typename It2>
            struct apply
              : is_same<
                    typename detail::remove_reference<It1>::type::cons_type
                  , typename detail::remove_reference<It2>::type::cons_type
                >
            {};
        };

        template<>
        struct value_of_impl<segmented_iterator_tag>
        {
            template<typename Iterator>
            struct apply
            {
                typedef typename detail::remove_reference<Iterator>::type it;

                typedef typename result_of::begin<typename it::car_type>::type begin;
                typedef typename result_of::value_of<begin>::type type;
            };
        };

        template<>
        struct deref_impl<segmented_iterator_tag>
        {
            template<typename Iterator>
            struct apply
            {
                typedef typename detail::remove_reference<Iterator>::type it;
                typedef typename result_of::begin<typename it::car_type>::type begin;
                typedef typename result_of::deref<begin>::type type;

                static type call(Iterator it)
                {
                    return *fusion::begin(it.car());
                }
            };
        };

        // discards the old head, expands the right child of the new head
        // and pushes the result to the head of the list.

        template<>
        struct next_impl<segmented_iterator_tag>
        {
            template<
                typename Iterator
              , bool IsSegmentDone = detail::is_range_next_empty<typename detail::remove_reference<Iterator>::type>::value
            >
            struct apply
            {
                typedef typename detail::remove_reference<Iterator>::type it;
                typedef typename it::cdr_type cdr_type;
                typedef detail::range_next<typename it::car_type> next;
                typedef segmented_iterator<cons<typename next::type, cdr_type> > type;

                static type call(Iterator it)
                {
                    return type(fusion::make_cons(next::call(it.car()), it.cdr()));
                }
            };

            template<typename Iterator>
            struct apply<Iterator, true> // segment done, move to next segment
            {
                typedef typename detail::remove_reference<Iterator>::type it;
                typedef typename it::cdr_type cdr_type;
                typedef typename detail::pop_segments<cdr_type> pop;
                typedef segmented_iterator<typename pop::type> type;

                static type call(Iterator it)
                {
                    return type(pop::call(it.cdr()));
                }
            };
        };
    }
}} // namespace boost::fusion

#endif
