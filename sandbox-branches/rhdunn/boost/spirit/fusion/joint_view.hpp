/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_JOINT_VIEW_HPP)
#define BOOST_FUSION_JOINT_VIEW_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  joint_iterator
    //
    //      Concatenates an iterator range and an iterator.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename FirstT, typename LastT, typename ConcatT>
    struct joint_iterator;

    namespace tuple_detail
    {
        template <typename FirstT, typename LastT, typename ConcatT>
        struct next_joint_iterator
        {
            typedef typename next_iterator<FirstT>::type next;

            typedef typename
                mpl::if_<
                    is_same<next, LastT>
                  , ConcatT
                  , joint_iterator<next, LastT, ConcatT>
                >::type
            type;
        };

        template <typename RT, typename IterT>
        static RT
        get_next_joint_iterator(IterT const& i, mpl::true_)
        {
            return RT(i.concat);
        }

        template <typename RT, typename IterT>
        static RT
        get_next_joint_iterator(IterT const& i, mpl::false_)
        {
            return RT(next(i.first), i.concat);
        }

        template <typename FirstT, typename LastT, typename ConcatT>
        struct joint_iterator_impl
            : iterator<joint_iterator<FirstT, LastT, ConcatT> >
        {
            typedef FirstT first_type;
            typedef LastT last_type;
            typedef ConcatT concat_type;

            typedef typename FirstT::deref_type deref_type;
            typedef next_joint_iterator<FirstT, LastT, ConcatT> next_type;

            joint_iterator_impl(FirstT const& first, ConcatT const& concat)
                : first(first), concat(concat) {}

            template <typename IterT>
            static typename next_iterator<IterT>::type
            next(IterT const& i)
            {
                typedef typename IterT::first_type first_type;
                typedef is_same<
                    typename next_iterator<first_type>::type, LastT>
                is_last;

                return get_next_joint_iterator
                    <typename next_iterator<IterT>::type>(i, is_last());
            }

            template <typename IterT>
            static typename deref_iterator<IterT>::type
            deref(IterT const& i)
            {
                typedef typename IterT::first_type first_type;
                return first_type::deref(i.first);
            }

            FirstT first;
            ConcatT concat;
        };

        template <typename ConcatT>
        struct joint_iterator_concat_impl : ConcatT
        {
            template <typename FirstT>
            joint_iterator_concat_impl(FirstT const&, ConcatT const& concat)
                : ConcatT(concat) {}
        };
    }

    template <typename FirstT, typename LastT, typename ConcatT>
    struct joint_iterator
        : mpl::if_<
            is_same<FirstT, LastT>
          , tuple_detail::joint_iterator_concat_impl<ConcatT>
          , tuple_detail::joint_iterator_impl<FirstT, LastT, ConcatT>
        >::type
    {
        typedef typename
            mpl::if_<
                is_same<FirstT, LastT>
              , tuple_detail::joint_iterator_concat_impl<ConcatT>
              , tuple_detail::joint_iterator_impl<FirstT, LastT, ConcatT>
            >::type
        base_type;

        joint_iterator(FirstT const& first, ConcatT const& concat)
            : base_type(first, concat) {}
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  joint_view
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename View1, typename View2>
    struct joint_view
    {
        typedef typename View1::begin_type begin_1;
        typedef typename View1::end_type end_1;
        typedef typename View2::begin_type begin_2;
        typedef typename View2::end_type end_2;

        typedef joint_iterator<begin_1, end_1, begin_2> begin_type;
        typedef end_2 end_type;

        joint_view(View1 const& view1, View2 const& view2)
            : view1(view1), view2(view2) {}

        begin_type
        begin() const
        {
            return begin_type(view1.begin(), view2.begin());
        }

        end_type
        end() const
        {
            return view2.end();
        }

        View1 view1;
        View2 view2;
    };

}} // namespace boost::fusion

#endif


