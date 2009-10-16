///////////////////////////////////////////////////////////////////////////////
// vector_space::example::difference.cpp                                     //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <boost/bind.hpp>
#include <boost/range.hpp>
#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/math/tools/precision.hpp>
#include <boost/vector_space/data/lazy_difference.hpp>

void example_difference(std::ostream& out){
    std::cout << "-> example_difference : ";
    using namespace boost;
    namespace vs = statistics::detail::vector_space;
    typedef double                          val_;
    typedef std::vector<val_>               vec_;
    typedef vs::lazy_difference<vec_,vec_>  range_diff_;

    const val_ eps = math::tools::epsilon<val_>();
    const val_ delta = eps * static_cast<val_>(2);

    vec_ vec;
    {
        using namespace assign;
        vec += 0.0, 1.1, 2.2, 3.3;
    }
    vec_ vec1(vec);
    std::transform(
        boost::begin(vec),
        boost::end(vec),
        begin(vec1),
        boost::bind<val_>(
            std::minus<val_>(),
            _1,
            delta
        )
    );

    range_diff_ range_diff(vec,vec1);
    vec_ copy_diff(size(vec));
    std::copy(
        const_begin(range_diff),
        const_end(range_diff),
        begin(copy_diff)
    );

   BOOST_FOREACH(const val_& z, copy_diff){
        BOOST_ASSERT(fabs(z-delta)<eps);
    }

    out << "<- " << std::endl;
}
