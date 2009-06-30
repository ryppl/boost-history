/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_VIEW_SINGLE_VIEW_SINGLE_VIEW_HPP
#define BOOST_FUSION_VIEW_SINGLE_VIEW_SINGLE_VIEW_HPP

//TODO as fusion element?!

#include <boost/fusion/support/ref.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/detail/as_fusion_element.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/fusion/view/single_view/single_view_iterator.hpp>
#include <boost/fusion/view/single_view/detail/begin_impl.hpp>
#include <boost/fusion/view/single_view/detail/end_impl.hpp>

namespace boost { namespace fusion
{
    struct single_view_tag;
    struct forward_traversal_tag;
    struct fusion_sequence_tag;

    template <typename T>
    struct single_view
      : sequence_base<single_view<T> >
    {
        typedef single_view_tag fusion_tag;
        typedef fusion_sequence_tag tag; // this gets picked up by MPL
        typedef forward_traversal_tag category;
        typedef mpl::true_ is_view;
        typedef mpl::int_<1> size;
        typedef T value_type;

        single_view()
          : val()
        {}

        single_view(single_view const& single_view)
          : val(single_view.val)
        {}

        //TODO !!!
        single_view(single_view& single_view)
          : val(single_view.val)
        {}

        single_view(single_view&& single_view)
          : val(single_view.val)
        {}

        template<typename OtherT>
        explicit single_view(BOOST_FUSION_R_ELSE_CLREF(OtherT) val)
          : val(BOOST_FUSION_FORWARD(OtherT,val))
        {}

        value_type val;
    };

    template <typename T>
    inline single_view<
        typename detail::as_fusion_element<BOOST_FUSION_R_ELSE_CLREF(T)>::type
    >
    make_single_view(BOOST_FUSION_R_ELSE_CLREF(T) v)
    {
        typedef
            single_view<
                typename detail::as_fusion_element<
                    BOOST_FUSION_R_ELSE_CLREF(T)
                >::type
            >
        view;

        return view(BOOST_FUSION_FORWARD(T,v));
    }
}}

#endif
