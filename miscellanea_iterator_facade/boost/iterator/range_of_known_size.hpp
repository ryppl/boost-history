//////////////////////////////////////////////////////////////////////////////
// range_of_known_size.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ITERATOR_RANGE_OF_KNOW_SIZE_HPP_ER_2009
#define BOOST_ITERATOR_RANGE_OF_KNOW_SIZE_HPP_ER_2009
#include <boost/mpl/size_t.hpp>

namespace boost{

    // Result of subseting a range
    // Serves as implementation to other classes
    template<std::size_t size>
    struct range_of_known_size{
        template<std::size_t in_size>
        struct result_size{
            typedef mpl::size_t<size> type;
        };
    };
}
#endif
