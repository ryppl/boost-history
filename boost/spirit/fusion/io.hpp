/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Järvi
    Copyright (c) 1999-2003 Jeremiah Willcock
    Copyright (c) 2001-2003 Joel de Guzman

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.
==============================================================================*/
#if !defined(BOOST_FUSION_TUPLE_IO_HPP)
#define BOOST_FUSION_TUPLE_IO_HPP

#include <iostream>
#include <boost/spirit/fusion/tuple.hpp>
#include <boost/spirit/fusion/detail/manip.ipp>
#include <boost/spirit/fusion/detail/io.ipp>

namespace boost { namespace fusion
{
    ///////////////////////////////////////////////////////////////////////////
    //
    // Tuple I/O (<< and >> operators)
    //
    ///////////////////////////////////////////////////////////////////////////
    template <
        class CharType, class CharTrait
      , BOOST_FUSION_TEMPLATE_PARAMS_ALL(T)
      , typename ID
    >
    inline std::basic_ostream<CharType, CharTrait>&
    operator<<(
        std::basic_ostream<CharType, CharTrait>& os
      , const BOOST_FUSION_TUPLE(T)& t)
    {
	    tuple_detail::print_tuple_functor<
	        std::basic_ostream<CharType, CharTrait> > f(os);
	    tuple_detail::printread_tuple(f,t);
	    return os;
    }

    template <
        class CharType
      , class CharTrait
      , BOOST_FUSION_TEMPLATE_PARAMS_ALL(T)
      , typename ID>
    inline std::basic_istream<CharType, CharTrait>&
    operator>>(
        std::basic_istream<CharType, CharTrait>& is
      , BOOST_FUSION_TUPLE(T)& t)
    {
        tuple_detail::read_tuple_functor<
	        std::basic_istream<CharType, CharTrait> > f(is);
        tuple_detail::printread_tuple(f,t);
        return is;
    }

}} // namespace boost::fusion

#endif // defined(BOOST_FUSION_TUPLE_IO_HPP)
