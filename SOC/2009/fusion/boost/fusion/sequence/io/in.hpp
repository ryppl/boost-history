/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 1999-2003 Jeremiah Willcock
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_IN_05042005_0120)
#define BOOST_IN_05042005_0120

#include <istream>

#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/sequence/io/detail/in.hpp>

namespace boost { namespace fusion
{
    template <typename Seq>
    inline std::istream&
    in(std::istream& is, Seq& seq)
    {
        detail::read_sequence(is, seq);
        return is;
    }

    namespace operators
    {
        template <typename Seq>
        inline typename
            enable_if<
               fusion::traits::is_sequence<Seq>
              , std::istream&
            >::type
        operator>>(std::istream& is, Seq& seq)
        {
            return fusion::in(is, seq);
        }
    }

    using operators::operator>>;
}}

#endif
