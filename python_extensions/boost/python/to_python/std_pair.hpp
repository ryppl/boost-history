// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_TO_PYTHON_STD_PAIR_HPP
#define BOOST_PYTHON_TO_PYTHON_STD_PAIR_HPP

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/python/to_python/boost_fusion.hpp>

namespace boost {
namespace python {

/**
 *  @brief A to-python converter for std::pair that yields a two-element Python tuple.
 */
template <typename T1, typename T2>
struct std_pair_to_python 
    : public boost_fusion_to_python< std::pair<T1,T2> > 
{

    std_pair_to_python() {}

};

} // namespace boost::python
} // namespace boost

#endif // !BOOST_PYTHON_TO_PYTHON_STD_PAIR_HPP
