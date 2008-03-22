// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#include <boost/mpl/if.hpp>
#include <boost/mpl/list.hpp>

#include <boost/explore.hpp>

// for streaming
std::ostream& basic_lg_stream_format(std::ostream& ostr)
{
    using namespace explore;
    return ostr << start("<=") << separator("#") << explore::end("=>")
                << assoc_start("<=") << assoc_separator("#") << assoc_end("=>");
}
