///////////////////////////////////////////////////////////////////////////////
// joint_dist::random::generate_n.hpp                                        //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_JOINT_DIST_RANDOM_GENERATE_N_ER_2009
#define BOOST_JOINT_DIST_RANDOM_GENERATE_N_ER_2009
#include <algorithm>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/concept_check.hpp>
#include <boost/iterator/iterator_concepts.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/range.hpp>
#include <boost/mpl/nested_type.hpp>
#include <boost/dist_random/random/generate_n.hpp>
#include <boost/dist_random/random/sample.hpp>
#include <boost/joint_dist/fun_wrap/include.hpp>
#include <boost/joint_dist/meta/is_joint_dist.hpp>

namespace boost{

    // To be consistent with dist_random, generate_n is in namespace boost
    // not namespace boost::joint_dist. enable_if is used instead.

    // Requirements:
    // size(*it_rx) == jd.dimension()
    template<typename ItRx,typename N,typename D,typename U>
    typename enable_if<
        joint_dist::is_joint_dist<D>,
        ItRx
    >::type
    generate_n(ItRx b_rx,N n,const D& jd,U& urng);

    // F is defined in joint_dist/fun_wrap
    template<template<typename,typename> class F,
        typename ItRx,typename ItF,typename N,typename D,typename U>
    typename enable_if<
        joint_dist::is_joint_dist<D>,
        ItRx
    >::type
    generate_function_n(ItRx b_rx,ItF b_f,N n,const D& jd,U& urng);

    // Same a previous, but F = fun_wrap::log_unnormalized_pdf_
    template<typename ItRx,typename ItF,typename N,typename D,typename U>
    typename enable_if<
        joint_dist::is_joint_dist<D>,
        ItRx
    >::type
    generate_n(ItRx b_rx,ItF b_f,N n,const D& jd,U& urng){
        return generate_function_n<joint_dist::fun_wrap::log_unnormalized_pdf_>(
            b_rx,b_f,n,jd,urng
        );
    } 
           
    // Implementation //

    template<typename ItRx,typename N,typename D,typename U>
    typename enable_if<
        joint_dist::is_joint_dist<D>,
        ItRx
    >::type
    generate_n(ItRx b_rx,N n,const D& jd,U& urng){
        typedef typename iterator_value<ItRx>::type row_;

        // std::back_inserter cannot work
        BOOST_CONCEPT_ASSERT((boost_concepts::IncrementableIterator<ItRx>));
        BOOST_CONCEPT_ASSERT((boost_concepts::WritableIterator<ItRx>));

        typedef N size_;
        size_ i = 0;
        while(i<n){
            row_& row = *b_rx;
            sample(
                boost::begin( row ),
                jd,
                urng
            );
            ++b_rx;
            ++i;
        }
        return b_rx;
    }

    template<template<typename,typename> class F,
        typename ItRx,typename ItF,typename N,typename D,typename U>
    typename enable_if<
        joint_dist::is_joint_dist<D>,
        ItRx
    >::type
    generate_function_n(ItRx b_rx,ItF b_f,N n,const D& jd,U& urng){
        ItRx e_rx = generate_n(b_rx,n,jd,urng);

        typedef typename iterator_value<ItRx>::type r_;
        typedef F<D,r_> mf_;
        typename mf_::type f = mf_::instance;

        std::transform(
            b_rx,
            e_rx,
            b_f,
            boost::lambda::bind(f,jd,boost::lambda::_1)
        );
        return e_rx;
    }


//} // intentional
}//boost


#endif