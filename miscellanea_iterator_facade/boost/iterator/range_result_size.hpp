//////////////////////////////////////////////////////////////////////////////
// range_result_size.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ITERATOR_RANGE_RESULT_SIZE_HPP_ER_2009
#define BOOST_ITERATOR_RANGE_RESULT_SIZE_HPP_ER_2009
namespace boost{

    template<typename T,std::size_t input_size>
    struct range_result_size{
        typedef typename T::template result_size<input_size>::type type;
    };

}
#endif
