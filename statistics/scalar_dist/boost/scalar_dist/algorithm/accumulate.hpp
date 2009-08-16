//////////////////////////////////////////////////////////////////////////////
// scalar_dist::algorithm::accumulate.hpp                                   //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SCALAR_DIST_ALGORITHM_ACCUMULATE_HPP_ER_2009
#define BOOST_SCALAR_DIST_ALGORITHM_ACCUMULATE_HPP_ER_2009
#include <numeric>
#include <functional>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/function_traits.hpp>
#include <boost/scalar_dist/meta/has_pdf.hpp>

namespace boost{
namespace math{

    // Usage
    // sum_log_pdf = accumulate<std::plus,log_unnormalized_pdf_>(
    //    dist,begin(vec_x),end(vec_x))
    template<
        template<typename> class G, //binary
        template<typename> class F, //in include.hpp
        typename D,
        typename InIt
    >
    typename enable_if<
        has_pdf<D>,
        typename D::value_type
    >::type
    accumulate(
        const D& dist,
        InIt b,
        InIt e,
        typename D::value_type init
    );

    // Implementation //

    template<
        template<typename> class G, //binary
        template<typename> class F, //in include.hpp
        typename D,
        typename InIt
    >
    typename enable_if<
        has_pdf<D>,
        typename D::value_type
    >::type
    accumulate(
        const D& dist,
        InIt b,
        InIt e,
        typename D::value_type init
    ){
        typedef typename D::value_type val_;
        return std::accumulate(
            b,
            e,
            init,
            boost::lambda::bind<val_>(
                G<val_>(),
                boost::lambda::_1,
                boost::lambda::bind<val_>(
                    F<D>::instance,
                    dist,
                    boost::lambda::_2
                )
            )
        );
    }

}
}

#endif 