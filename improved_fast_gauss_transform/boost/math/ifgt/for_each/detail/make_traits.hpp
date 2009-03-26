//////////////////////////////////////////////////////////////////////////////
// make_traits.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_MAKE_TRAITS_HPP_ER_2009
#define BOOST_MATH_IFGT_MAKE_TRAITS_HPP_ER_2009
#include <boost/mpl/size_t.hpp>
namespace boost{
namespace math{
namespace ifgt{


template<typename T, std::size_t stride0, std::size_t stride1>
struct make_traits{
    typedef T type;
    typedef mpl::size_t<stride0> stride0_type;
    typedef mpl::size_t<stride1> stride1_type;
};


}
}
}
#endif
