/*=============================================================================
    Copyright (c) 1999-2003 Jaakko Jarvi
    Copyright (c) 1999-2003 Jeremiah Willcock
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_OUT_05042005_0120)
#define BOOST_OUT_05042005_0120

#include <ostream>

#include <boost/fusion/sequence/io/detail/out.hpp>
#include <boost/fusion/support/is_sequence.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/or.hpp>

namespace boost { namespace fusion
{
    template <typename Seq>
    inline std::ostream&
    out(std::ostream& os, Seq& seq)
    {
        detail::print_sequence(os, seq);
        return os;
    }
    
    namespace operators
    {
        template <typename Seq>
        inline typename
            enable_if<
                fusion::traits::is_sequence<Seq>
              , std::ostream&
            >::type
        operator<<(std::ostream& os, Seq const& seq)
        {
            return fusion::out(os, seq);
        }
    }
    using operators::operator<<;
}}

#endif
