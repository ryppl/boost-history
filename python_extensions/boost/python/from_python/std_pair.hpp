// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_FROM_PYTHON_STD_PAIR_HPP
#define BOOST_PYTHON_FROM_PYTHON_STD_PAIR_HPP

#include <boost/python/from_python/boost_fusion.hpp>
#include <boost/mpl/adapted/std_pair.hpp> // distributed with python_extensions, not MPL
#include <boost/fusion/include/std_pair.hpp>

namespace boost {
namespace python {

/**
 *  @brief An rvalue from-python converter that converts a two-element Python iterable into an std::pair.
 */
template <typename T1, typename T2>
struct std_pair_from_python : public boost_fusion_from_python< std::pair<T1,T2> > {

    std_pair_from_python() : boost_fusion_from_python< std::pair<T1,T2> >() {}

};

} // namespace boost::python
} // namespace boost

#endif // !BOOST_PYTHON_FROM_PYTHON_BOOST_FUSION_HPP
