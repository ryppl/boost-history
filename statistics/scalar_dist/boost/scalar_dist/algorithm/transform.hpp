//////////////////////////////////////////////////////////////////////////////
// scalar_dist::algorithm::transform.hpp                                    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_SCALAR_DIST_ALGORITHM_TRANSFORM_HPP_ER_2009
#define BOOST_SCALAR_DIST_ALGORITHM_TRANSFORM_HPP_ER_2009
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/type_traits/function_traits.hpp>
#include <boost/scalar_dist/meta/pointer.hpp>
#include <boost/scalar_dist/meta/has_pdf.hpp>

namespace boost{
namespace math{

    // Applies a function (F) to a set of values, whose definition is looked up
    // in the namespace of the distribution (D)
    //
    // Example: F = fun_wrap::log_unnormalized_pdf_
    template<
        template<typename> class F,
        typename D,
        typename InIt,
        typename OutIt
    >
    typename enable_if<
        has_pdf<D>,
        OutIt
    >::type
    transform(
        const D& dist,
        InIt b,
        InIt e,
        OutIt i
    );

    //Example: G = std::plus
    template<
        template<typename> class G,
        template<typename> class F,
        typename D,
        typename InIt,
        typename InIt1,
        typename OutIt
    >
    typename enable_if<
        has_pdf<D>,
        OutIt
    >::type
    transform(
        const D& dist,
        InIt b,
        InIt e,
        InIt1 b1,
        OutIt i
    );

    // Implementation //
    
    template<
        template<typename> class F,
        typename D,
        typename InIt,
        typename OutIt
    >
    typename enable_if<
        has_pdf<D>,
        OutIt
    >::type
    transform(
        const D& dist,
        InIt b,
        InIt e,
        OutIt i
    ){
        typedef pointer<D> fp_;
        typedef typename signature<D>::type sig_;
        typedef function_traits<sig_> traits_;
        typedef typename traits_::result_type res_;

        return std::transform(
            b,
            e,
            i,
            bind<res_>(
                fp_::template make<F>(),
                dist,
                _1
            )
        );
    }

    template<
        template<typename> class G,
        template<typename> class F,
        typename D,
        typename InIt,
        typename InIt1,
        typename OutIt
    >
    typename enable_if<
        has_pdf<D>,
        OutIt
    >::type
    transform(
        const D& dist,
        InIt b,
        InIt e,
        InIt1 b1,
        OutIt i
    ){
        typedef pointer<D> fp_;
        typedef typename signature<D>::type sig_;
        typedef function_traits<sig_> traits_;
        typedef typename traits_::result_type res_;

        return std::transform(
            b,  //1
            e,
            b1, //2
            i,
            bind<res_>(
                G<res_>(),
                _2,
                bind<res_>(
                    fp_::template make<F>(),
                    dist,
                    _1
                )
            )
        );
    }

}
}

#endif
