/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Järvi
    Copyright (c) 2001-2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_TUPLE_OPERATOR_HPP)
#define BOOST_FUSION_TUPLE_OPERATOR_HPP

#include <boost/spirit/fusion/tuple.hpp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    //  Relational operators
    //
    //      Expands to:
    //
    //          template <
    //              typename T0, typename T1... typename TN,
    //              typename TB0, typename TB1... typename TBN,
    //              typename ID>
    //          inline bool operator OP(
    //              tuple<T0, T1... TN, ID> const& a
    //              tuple<TB0, TB1... TBN, ID> const& b)
    //          { return a.data OP b.data; }
    //
    //      where N = BOOST_FUSION_MAX_ARITY and with one function
    //      for each OP, where OP is: ==, !=, <, >, <=, and >=.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <
        BOOST_FUSION_TEMPLATE_PARAMS_ALL(T),
        BOOST_FUSION_TEMPLATE_PARAMS_ALL(TB), typename ID>
    inline bool operator==(
        BOOST_FUSION_TUPLE(T) const& a,
        BOOST_FUSION_TUPLE(TB) const& b)
    {
        return a.data == b.data;
    }

    template <
        BOOST_FUSION_TEMPLATE_PARAMS_ALL(T),
        BOOST_FUSION_TEMPLATE_PARAMS_ALL(TB), typename ID>
    inline bool operator!=(
        BOOST_FUSION_TUPLE(T) const& a,
        BOOST_FUSION_TUPLE(TB) const& b)
    {
        return a.data != b.data;
    }

    template <
        BOOST_FUSION_TEMPLATE_PARAMS_ALL(T),
        BOOST_FUSION_TEMPLATE_PARAMS_ALL(TB), typename ID>
    inline bool operator<(
        BOOST_FUSION_TUPLE(T) const& a,
        BOOST_FUSION_TUPLE(TB) const& b)
    {
        return a.data < b.data;
    }

    template <
        BOOST_FUSION_TEMPLATE_PARAMS_ALL(T),
        BOOST_FUSION_TEMPLATE_PARAMS_ALL(TB), typename ID>
    inline bool operator>(
        BOOST_FUSION_TUPLE(T) const& a,
        BOOST_FUSION_TUPLE(TB) const& b)
    {
        return a.data > b.data;
    }

    template <
        BOOST_FUSION_TEMPLATE_PARAMS_ALL(T),
        BOOST_FUSION_TEMPLATE_PARAMS_ALL(TB), typename ID>
    inline bool operator<=(
        BOOST_FUSION_TUPLE(T) const& a,
        BOOST_FUSION_TUPLE(TB) const& b)
    {
        return a.data <= b.data;
    }

    template <
        BOOST_FUSION_TEMPLATE_PARAMS_ALL(T),
        BOOST_FUSION_TEMPLATE_PARAMS_ALL(TB), typename ID>
    inline bool operator>=(
        BOOST_FUSION_TUPLE(T) const& a,
        BOOST_FUSION_TUPLE(TB) const& b)
    {
        return a.data >= b.data;
    }

#if defined __GNUC__ && __GNUC__ <= 2

    ///////////////////////////////////////////////////////////////////////////
    //
    //  Relational operators for tuples of the same type. The following
    //  relational ops are workarounds over the ambiguity with the relops
    //  caused by g++ 2.9x
    //
    //      Expands to:
    //
    //          template <typename T0, typename T1... typename TN, typename ID>
    //          inline bool operator OP(
    //              tuple<T0, T1... TN, ID> const& a
    //              tuple<T0, T1... TN, ID> const& b)
    //          { return a.data OP b.data; }
    //
    //      where N = BOOST_FUSION_MAX_ARITY and with one function
    //      for each OP, where OP is: ==, !=, <, >, <=, and >=.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(T), typename ID>
    inline bool operator==(
        BOOST_FUSION_TUPLE(T) const& a,
        BOOST_FUSION_TUPLE(T) const& b)
    {
        return a.data == b.data;
    }

    template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(T), typename ID>
    inline bool operator!=(
        BOOST_FUSION_TUPLE(T) const& a,
        BOOST_FUSION_TUPLE(T) const& b)
    {
        return a.data != b.data;
    }

    template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(T), typename ID>
    inline bool operator<(
        BOOST_FUSION_TUPLE(T) const& a,
        BOOST_FUSION_TUPLE(T) const& b)
    {
        return a.data < b.data;
    }

    template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(T), typename ID>
    inline bool operator>(
        BOOST_FUSION_TUPLE(T) const& a,
        BOOST_FUSION_TUPLE(T) const& b)
    {
        return a.data > b.data;
    }

    template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(T), typename ID>
    inline bool operator<=(
        BOOST_FUSION_TUPLE(T) const& a,
        BOOST_FUSION_TUPLE(T) const& b)
    {
        return a.data <= b.data;
    }

    template <BOOST_FUSION_TEMPLATE_PARAMS_ALL(T), typename ID>
    inline bool operator>=(
        BOOST_FUSION_TUPLE(T) const& a,
        BOOST_FUSION_TUPLE(T) const& b)
    {
        return a.data >= b.data;
    }

#endif

}} // end of namespace boost::fusion

#endif // defined(BOOST_FUSION_TUPLE_OPERATOR_HPP)
