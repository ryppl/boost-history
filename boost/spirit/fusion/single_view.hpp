/*=============================================================================
    Copyright (c) 2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_SINGLE_VIEW_HPP)
#define BOOST_FUSION_SINGLE_VIEW_HPP

#include <boost/spirit/fusion/iterator.hpp>
#include <boost/spirit/fusion/detail/helpers.hpp>
#include <boost/spirit/fusion/detail/make_tuple.ipp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/add_reference.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  single_iterator
    //
    //      Makes a single value T act like a single element iterator.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct single_iterator_end : iterator<single_iterator_end<T> >
    {
        typedef void_t deref_type;
        typedef void_t next_type;
    };

    template <typename T>
    struct single_iterator : iterator<single_iterator<T> >
    {
        typedef add_reference<typename add_const<T>::type> deref_type;
        typedef mpl::identity<single_iterator_end<T> > next_type;

        single_iterator()
            : val() {}

        explicit single_iterator(typename tuple_detail::call_param<T>::type val)
            : val(val) {}

        template <typename IterT>
        static single_iterator_end<T>
        next(IterT const&)
        {
            return single_iterator_end<T>();
        }

        template <typename IterT>
        static typename add_reference<typename add_const<T>::type>::type
        deref(IterT const& t)
        {
            return t.val;
        }

        T val;
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  single_view
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct single_view
    {
        typedef single_iterator<T> begin_type;
        typedef single_iterator_end<T> end_type;

        single_view()
            : val() {}

        explicit single_view(typename tuple_detail::call_param<T>::type val)
            : val(val) {}

        begin_type
        begin() const
        {
            return begin_type(val);
        }

        end_type
        end() const
        {
            return end_type();
        }

        T val;
    };

    template <typename T>
    struct make_single_view
    {
        typedef
            single_view<typename tuple_detail::as_tuple_element<T>::type>
        type;
    };

}} // namespace boost::fusion

#endif


