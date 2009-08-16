///////////////////////////////////////////////////////////////////////////////
// ars::test::standard_distribution.hpp                                      //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ARS_TEST_STANDARD_DISTRIBUTION_HPP_ER_2009
#define BOOST_ARS_TEST_STANDARD_DISTRIBUTION_HPP_ER_2009

#include <iostream>
#include <list>
#include <string>
#include <iterator>
#include <vector>
#include <algorithm>

#include <boost/assert.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/format.hpp>
#include <boost/range.hpp>
#include <boost/numeric/conversion/converter.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/assign/std/vector.hpp>

//#include <boost/math/distributions/unnormalized_pdf/include.hpp>
//#include <boost/math/distributions/algorithm/transform.hpp>

#include <boost/non_param/algorithm/kolmogorov_smirnov_distance.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/ref_distribution.hpp>

#include <boost/ars/functional/standard_distribution.hpp>
#include <boost/ars/constant.hpp>
#include <boost/ars/proposal_sampler.hpp>
#include <boost/ars/sampler.hpp>

namespace boost{
namespace ars{
namespace test{

// This function draws repeat_n times by increments of draw_n
// and ouputs the corresponding kolmogorov_distance for distribution D
// and the number of rejections. D is a standard distribution
// such as math::normal_distribution
template<typename D,typename U>
void standard_distribution(
    const D& mdist, //e.g. D == math::normal_distribution
    typename D::value_type x_min,
    typename D::value_type x_max,
    typename D::value_type init_0,
    typename D::value_type init_1,
    U& urng,
    unsigned n_max_reject,
    unsigned draw_n,
    unsigned repeat_n,
    std::ostream& out
){
    using namespace boost;
    using namespace math;
    using namespace assign;
    typedef std::string                                     str_;
    typedef std::runtime_error                              err_;
    typedef typename D::value_type                          val_;
    typedef std::vector<val_>                               vals_;
    typedef std::vector<unsigned>                           ints_;

    typedef ars::functional::standard_distribution<D>       fun_t;
    typedef ars::proposal_sampler<val_,std::vector>         ps_;
    typedef ars::sampler<ps_>                               s_;
    typedef random::ref_distribution<s_&>                   ref_s_;
    typedef variate_generator<U&,ref_s_>                    vg_;
    //typedef boost::random::kolmogorov_smirnov<D>            check_dist_t;

    s_ s;
    s.set_function(x_min, x_max, fun_t(mdist));

    vals_ draws; draws.reserve( repeat_n * draw_n );
    vals_ kss; kss.reserve(repeat_n);
    ints_ n_rejects; n_rejects.reserve(repeat_n);
    try{
        for(unsigned i = 0; i<repeat_n; i++){
            try{
                s.initialize(init_0,init_1);
                vg_ vg(urng,ref_s_(s)); 
                std::generate_n(std::back_inserter(draws),draw_n,vg);
                // Without ref_s_, n would be reset to 0
                unsigned n = (
                    (vg.distribution()).distribution()
                ).n_reject();
                val_ e = non_param::kolmogorov_smirnov_distance(
                    mdist,
                    boost::begin(draws),
                    boost::end(draws)
                );
                kss.push_back(e);
                n_rejects.push_back(n);
            }catch(std::exception& e){
                format f("at i = %1% : %2%"); f % i % e.what();
                throw std::runtime_error(f.str());
            }
        }
    }catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    format f("init_0 = %1%, init_1 = %2%, by increments of n = %3%, ");
    f % init_0 % init_1 % draw_n;
    out << f.str() << std::endl;
    out << "KS distance : ";
    copy(
        begin(kss),
        end(kss),
        std::ostream_iterator<val_>(out," ")
    );
    out << std::endl << "n_reject : ";
    copy(
        begin(n_rejects),
        end(n_rejects),
        std::ostream_iterator<val_>(out," ")
    );
    out << std::endl;
}

}//test
}//ars
}//boost

#endif
