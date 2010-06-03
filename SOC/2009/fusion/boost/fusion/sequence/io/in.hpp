/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 1999-2003 Jeremiah Willcock
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SEQUENCE_IO_IN_HPP
#define BOOST_FUSION_SEQUENCE_IO_IN_HPP

#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/support/internal/assert.hpp>

#include <istream>

#include <boost/fusion/sequence/io/detail/in.hpp>

namespace boost { namespace fusion
{
    template<typename Seq>
    inline std::istream&
    in(std::istream& is, Seq& seq)
    {
        BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
        BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

        detail::read_sequence(is, seq);
        return is;
    }

    namespace operators
    {
        template<typename Seq>
        inline typename
            enable_if<
               fusion::traits::is_sequence<Seq>
             , std::istream&
            >::type
        operator>>(std::istream& is, Seq& seq)
        {
            BOOST_FUSION_MPL_ASSERT((traits::is_sequence<Seq>));
            BOOST_FUSION_MPL_ASSERT((traits::is_forward<Seq>));

            return fusion::in(is, seq);
        }
    }

    using operators::operator>>;
}}

#endif
