///////////////////////////////////////////////////////////////////////////////
// importance_sampling::example::sampler.cpp                                 //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <boost/range.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/format.hpp>
#include <boost/function.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/math/tools/precision.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/ref.hpp>
#include <boost/foreach.hpp>

#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/make_map.hpp>

#include <boost/math/tools/precision.hpp>

#include <boost/statistics/detail/accumulator/statistics/proportion_less_than.hpp>
#include <boost/statistics/detail/distribution_common/distributions/reference/wrapper.hpp>
#include <boost/statistics/detail/distribution_common/functor/log_unnormalized_pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/distributions/normal/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/map_pdf/ratio_pdf/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/test/detail/x_f.hpp>
#include <boost/statistics/detail/distribution_toolkit/test/random.hpp>

#include <boost/statistics/detail/fusion/at_key/functor.hpp>
#include <boost/statistics/detail/fusion/at_key/range.hpp>
#include <boost/statistics/detail/non_parametric/kolmogorov_smirnov/statistic.hpp>

#include <boost/statistics/detail/importance_sampling/weights/prepare_weights.hpp>
#include <boost/statistics/detail/importance_sampling/random/include.hpp>
#include <boost/statistics/detail/importance_sampling/statistics/percentage_effective_sample_size.hpp>

void example_sampler(std::ostream& os){
    os << "->example_sampler : \n";

    // Sample from N(x|mu,sigma^2)N(x|mu,sigma^2) = N(x|mu,sigma^2/2), 
    // using SIR with N(x|mu+sigma,sigma^2) as proposal density. 
    // The quality of the sample is assessed by a series of 
    // kolmorov-distances along the the sample size of the targets.    
    using namespace boost;
    namespace stat = boost::statistics::detail;
    namespace dist = stat::distribution;
    namespace tk = stat::distribution::toolkit;
    namespace is = stat::importance_sampling;
    namespace np = stat::non_parametric;

    typedef std::string                                 str_;
    typedef double                                      val_;
    typedef std::vector<val_>                           vals_;
    typedef range_iterator<vals_>::type                 vals_it_;
    typedef math::normal_distribution<val_>             dist_;
    typedef mt19937                                     urng_;
    typedef is::prepare_weights<val_>                   prepare_weights_;
    typedef stat::accumulator::tag::percentage_effective_sample_size tag_ess_;
    typedef stat::accumulator::tag::proportion_less_than tag_plt_; 
    typedef boost::accumulators::stats<tag_ess_,tag_plt_> stats_;
    typedef boost::accumulators::accumulator_set<val_,stats_> acc_;
    typedef std::size_t                                 size_;

    typedef boost::mpl::int_<0>                     	k0_;
    typedef boost::mpl::int_<1>                         k1_;
	typedef boost::fusion::result_of::make_map<k0_,k1_,val_,val_>::type data_;
	
    typedef np::kolmogorov_smirnov::statistic<val_,k0_,k1_> ks_stat_;
    typedef std::vector<data_>                          vec_data_;
    typedef range_iterator<vec_data_>::type             vec_data_it_;

    typedef stat::fusion::at_key::meta_range<
        vec_data_it_,
        k0_
    >::type range1_;
    typedef is::sampler<range1_,val_>                   	is_sampler_;
    typedef boost::variate_generator<urng_&,is_sampler_> 	vg_;

    // Constants
    const unsigned n_p          = 5e4;             
    const val_ max_log          = 100.0;           
    const val_ mu               = 0.0;
    const val_ sigma            = 1.0;
    const val_ t_mu             = mu + sigma;
    const val_ t_sigma          = sigma/sqrt(2.0);
    const val_ eps = boost::math::tools::epsilon<val_>();

    const unsigned n_loops  = 7;
    const unsigned n1       = 1e1;
    const unsigned n2       = 1e1;

    prepare_weights_ prepare_weights( max_log );
    dist_ p_d( mu, sigma );       // proposal
    dist_ t_d( t_mu, t_sigma );   // target

    urng_ urng;

    vec_data_ proposals(n_p); 
    {
        // Generate proposal values and their log_pdf
        // {(x,log_pdf(x)):i=1,...,n_p}
		BOOST_AUTO(vg,dist::make_random_generator(urng,p_d));        
        BOOST_FOREACH(data_& data,proposals){
        	val_ x = vg();
			boost::fusion::at_key<k0_>(data) = x;
            boost::fusion::at_key<k1_>(data) = log_unnormalized_pdf(p_d,x);
		}        
    }
    vals_ is_weights( n_p );
    {
        // is_weights <- log_pdf(t_d) - log_pdf(p_d)

        vals_it_ i = boost::begin( is_weights );
        BOOST_FOREACH(const data_& d, proposals)
        {
            *i = log_unnormalized_pdf( t_d, fusion::at_key<k0_>(d) ) 
                - fusion::at_key<k1_>(d);
            ++i;
        }

    }
    { 
        //is_weights <- exp(is_weights + c )
        prepare_weights(
            boost::begin(is_weights),
            boost::end(is_weights)
        );
        os << "weights : " << std::endl
            << prepare_weights_::header << " = "
            << prepare_weights << std::endl;
    }
    {
        acc_ acc = std::for_each(
            boost::begin(is_weights),
            boost::end(is_weights),
            acc_(( stat::accumulator::keyword::threshold = eps ))
        );
        val_ ess = boost::accumulators::extract_result<tag_ess_>(acc);
        val_ plt_eps = boost::accumulators::extract_result<tag_plt_>(acc);
        boost::format f("(ess,plt_eps) = (%1%,%2%)");
        f % ess % plt_eps;
        os << f.str() << std::endl;
    }
    {
        range1_ r1 = stat::fusion::at_key::make_range<k0_>(
            boost::begin(proposals),
            boost::end(proposals)
        );
        vg_ vg(
            urng,
            is_sampler_(
                is_weights,
                r1
            )
        );
        vec_data_ targets;
        {
            os << "proposal : " << description(p_d) << std::endl; 
            os << ks_stat_::description_header << std::endl;
			dist::toolkit::test::random(
            	t_d,
                vg,
                n_loops,
                n1,
                n2,
                os
            );
        }
    }

    os << "<-" << std::endl;

}


