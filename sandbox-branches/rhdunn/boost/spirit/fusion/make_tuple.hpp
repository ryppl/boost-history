/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Järvi
    Copyright (c) 2001-2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_MAKE_TUPLE_HPP)
#define BOOST_FUSION_MAKE_TUPLE_HPP

#include <boost/spirit/fusion/tuple.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  The make_tuple, tie generators
    //
    //      Expands to:
    //
    //          template <typename T0, typename T1... typename TN>
    //          inline tuple<
    //              typename tuple_detail::as_tuple_element<T0>::type,
    //              typename tuple_detail::as_tuple_element<T1>::type,
    //              ...
    //              typename tuple_detail::as_tuple_element<TN>::type,
    //          make_tuple(T0 const& _0, T1 const& _1... TN const& _N)
    //          {
    //              return tuple<
    //                  typename tuple_detail::as_tuple_element<T0>::type,
    //                  typename tuple_detail::as_tuple_element<T1>::type,
    //                  ...
    //                  typename tuple_detail::as_tuple_element<TN>::type>(
    //                      _0, _1... _N);
    //          }
    //
    //          template <typename T0, typename T1... typename TN>
    //          inline tuple<T0&, T1&... TN&>
    //          tie(T0& _0, T1& _1... TN& _N)
    //          {
    //              return tuple<T0&, T1&... TN&>(_0, _1... _N);
    //          }
    //
    //      where N runs from 1 to BOOST_TUPLE_MAX_ARITY and as_tuple_element
    //      is a metafunction that converts a ref(T) to a reference to T, a
    //      cref(T) to a const reference to T and of course, a T left
    //      unconverted as-is to a T.
    //
    ///////////////////////////////////////////////////////////////////////////
    //  Bring in the generators
    #include <boost/spirit/fusion/detail/make_tuple.ipp>

    //  Swallows any assignment (by Doug Gregor)
    namespace tuple_detail
    {
        struct swallow_assign
        {
            template<typename T>
            swallow_assign const&
            operator=(const T&) const
            { return *this; }
        };
    }

    //  "ignore" allows tuple positions to be ignored when using "tie".
    tuple_detail::swallow_assign const ignore = tuple_detail::swallow_assign();

}} // end of namespace boost:;fusion

#endif // defined(BOOST_FUSION_MAKE_TUPLE_HPP)
