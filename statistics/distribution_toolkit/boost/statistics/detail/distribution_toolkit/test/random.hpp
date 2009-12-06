///////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::test::random.hpp                                   //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_TEST_RANDOM_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_TEST_RANDOM_HPP_ER_2009
#include <iterator>
#include <boost/utility/result_of.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/range.hpp>
#include <boost/function.hpp>
#include <boost/static_assert.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/statistics/detail/distribution_toolkit/test/detail/x_f.hpp>
#include <boost/statistics/detail/non_parametric/kolmogorov_smirnov/statistic.hpp>
#include <boost/statistics/detail/distribution_common/functor/cdf.hpp>
#include <boost/statistics/detail/distribution_common/meta/random/generator.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace toolkit{
namespace test{

template<typename D,typename R,typename N>
void random(
    const D& dist,
    R& random_generator,
    N n_loops,  // 6
    N n1,       // 1e1
    N n2,       // 1e1
    std::ostream& os
)
{
    // For each distribution, dist, and the corresponding random engine returned
    // by make_random(dist), this example outputs a series of kolmogorov-
    // smirnov statistics. Convergence to zero is evidence that 
    // the sample agrees wit the cdf of dist
    
    typedef typename distribution::meta::value<D>::type val_;
    typedef distribution::functor::cdf_<D>     		    fun_;
	typedef detail::x_f<val_>							x_cdf_;
    typedef typename x_cdf_::input						k1_;
    typedef typename x_cdf_::output1					k2_;
    typedef non_parametric::kolmogorov_smirnov::statistic<val_,k1_,k2_> ks_;
    typedef typename x_cdf_::template functor<D> 		make_x_cdf_;
    
    typedef typename x_cdf_::template meta_generator<D>	meta_gen_data_;
	typedef typename meta_gen_data_::type 				gen_data_;
    typedef typename gen_data_::result_type 			data_;
    typedef std::vector<data_> dataset_;
    dataset_ dataset;
    ks_ ks;
    {

		gen_data_ gen_data = meta_gen_data_::call(dist,random_generator);
        os << dist << std::endl; 
        unsigned i = 0;
        unsigned n_draws = n1;
        dataset.clear();
        while(i<n_loops){
            dataset.reserve(dataset.size()+n_draws);
            std::generate_n(
                std::back_inserter(dataset),
                n_draws,
                gen_data
            );
            ks(boost::begin(dataset),boost::end(dataset));
            os << ks.description() << std::endl;
            ++i;
            n_draws *= n2;
        }
    }
}

template<typename D,typename U,typename N>
void random2(
    const D& dist,
    U& urng,
    N n_loops,  
    N n1,       
    N n2,       
    std::ostream& os
)
{
	BOOST_AUTO(
    	vg,
        make_random_generator(urng,dist)
    );

	return test::random(
    	dist,
		vg,
        n_loops,
		n1,
        n2,
        os
	);
}


}// test
}// toolkit
}// distribution
}// detail
}// statistics
}// boost

#endif