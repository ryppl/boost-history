///////////////////////////////////////////////////////////////////////////////
// dist_random::random::generate_n.hpp                                       //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_DIST_RANDOM_RANDOM_GENERATE_N_HPP_ER_2009
#define BOOST_DIST_RANDOM_RANDOM_GENERATE_N_HPP_ER_2009
#include <algorithm>
#include <boost/concept_check.hpp>
#include <boost/iterator/iterator_concepts.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/scalar_dist/fun_wrap/include.hpp>
#include <boost/scalar_dist/iterator/distribution_function.hpp>
#include <boost/dist_random/functional/make_random.hpp>

namespace boost{

    // Generates a sample from a distribution
    template<typename ItX,typename N,typename D,typename U> 
    typename enable_if<
        math::is_scalar_dist<D>,
        ItX 
    >::type
    generate_n(
        ItX b_x, N n,const D& dist, U& urng
    );

    // Same as above and a function associated with the distribution
    template<template<typename> class F,
        typename ItX,typename N,typename ItF,typename D,
        typename U> 
    typename enable_if<
        math::is_scalar_dist<D>,
        ItX 
    >::type
    generate_function_n(
        ItX b_x, ItF b_f, N n,const D& dist, U& urng
    );

    // F =  math::fun_wrap::log_unnormlized_pdf_ 
    template<typename ItX,typename N,typename ItL,typename D,
        typename U> 
    typename enable_if<
        math::is_scalar_dist<D>,
        ItX 
    >::type
    generate_n(
        ItX b_x, ItL b_lpdf, N n,const D& dist, U& urng
    ){
        return generate_function_n<math::fun_wrap::log_unnormalized_pdf_>(
            b_x,b_lpdf,n,dist,urng
        );
    }

    // Implementation //

    template<typename ItX,typename N,typename D,typename U> 
    typename enable_if<
        math::is_scalar_dist<D>,
        ItX 
    >::type
    generate_n(
        ItX b_x, N n,const D& dist, U& urng
    ){
        typedef typename dist_random<D>::type rand_;
        typedef boost::variate_generator<U&,rand_> vg_;
        vg_ vg(
            urng,
            make_random(dist)
        );
        return std::generate_n(
            b_x,
            n,
            vg
        );
    }
    
    template<
        template<typename> class F,
        typename ItX,
        typename N,
        typename ItF,
        typename D,
        typename U
    > 
    typename enable_if<
        math::is_scalar_dist<D>,
        ItX 
    >::type
    generate_function_n(ItX b_x, ItF b_f, N n,const D& dist, U& urng){
        BOOST_CONCEPT_ASSERT((boost_concepts::IncrementableIterator<ItX>));
        // TODO was there a reason std::back_inserter would not be allowed?
        // BOOST_CONCEPT_ASSERT((boost_concepts::WritableIterator<ItF>));
        ItX e_x = generate_n(
            b_x,
            n,
            dist,
            urng
        );

        std::copy(
            math::make_distribution_function_iterator<F>(
                dist,
                b_x
            ),
            math::make_distribution_function_iterator<F>(
                dist,
                e_x
            ),
            b_f
        );

        return e_x;
    }

}// boost

#endif