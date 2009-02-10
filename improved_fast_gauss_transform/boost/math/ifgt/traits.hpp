//////////////////////////////////////////////////////////////////////////////
// ifgt/traits.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_TRAITS_HPP_ER_2009
#define BOOST_MATH_IFGT_TRAITS_HPP_ER_2009
#include <boost/mpl/size_t.hpp>
namespace boost{namespace math{namespace ifgt{

template<typename T>
struct stride_traits{
    typedef mpl::size_t<T::source_size> stride0_type;
    typedef mpl::size_t<T::weight_size> stride1_type;
};

///These traits are for use by for_each_over_vector2matrix
template<typename T>
struct traits : public stride_traits<T>{
    typedef T type;
};

template<typename T>
struct call_traits : T::stride_traits{
    typedef T type;
};
}}}
#endif
