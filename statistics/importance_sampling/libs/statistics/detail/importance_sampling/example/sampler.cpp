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

//#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/map.hpp>

#include <boost/statistics/detail/distribution_toolkit/distributions/normal/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/fwd_math/cdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/meta/bind_delegate.hpp>
#include <boost/statistics/detail/distribution_toolkit/map_pdf/ratio_pdf/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/fun_wrap/log_unnormalized_pdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/fun_wrap/cdf.hpp>
#include <boost/statistics/detail/distribution_toolkit/random/generator.hpp>

#include <boost/statistics/detail/fusion/at_key/functor.hpp>
#include <boost/statistics/detail/fusion/at_key/range.hpp>
#include <boost/statistics/detail/non_parametric/kolmogorov_smirnov/statistic.hpp>

#include <boost/statistics/detail/importance_sampling/weights/prepare_weights.hpp>
#include <boost/statistics/detail/importance_sampling/random/include.hpp>

void example_sampler(std::ostream& os){
    os << "->example_sampler : \n";

    // We sample from N(x|mu,sigma^2)N(x|mu,sigma^2) = N(x|mu,sigma^2/2), 
    // using SIR with N(x|mu+sigma,sigma^2) as proposal density. 
    // The quality of the sample is assessed by a series of 
    // kolmorov-distances along the the sample size of the targets.    
    using namespace boost;
    namespace stat = boost::statistics::detail;
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
    typedef std::size_t                                 size_;

    typedef mpl::int_<0>                                key1_;
    typedef mpl::int_<1>                                key2_;
    typedef boost::fusion::pair<key1_,val_>             p1_;
    typedef boost::fusion::pair<key2_,val_>             p2_;
    typedef fusion::map<p1_,p2_>                        data_;

    typedef tk::meta::bind_delegate<dist_>::type        fun_;
    typedef np::kolmogorov_smirnov::statistic<
        val_,
        key1_,
        key2_
    > ks_stat_;

    typedef std::vector<data_>                          dataset_;
    typedef range_iterator<dataset_>::type              dataset_it_;
    typedef boost::function<data_()>                    gen_data_;
    typedef stat::fusion::at_key::meta_range<
        dataset_it_,
        key1_
    >::type range1_;
    typedef is::sampler<range1_,val_>                   is_sampler_;
    typedef boost::variate_generator<
        urng_&,
        is_sampler_
    > vg_;

    // Constants
    const unsigned p_n          = 5e4;             
    const val_ max_log          = 100.0;           
    const val_ mu               = 0.0;
    const val_ sigma            = 1.0;
    const val_ t_mu             = mu + sigma;
    const val_ t_sigma          = sigma/sqrt(2.0);

    const unsigned n_loops  = 7;
    const unsigned n1       = 1e1;
    const unsigned n2       = 1e1;

    prepare_weights_ prepare_weights( max_log );
    dist_ p_d( mu, sigma );       // proposal
    dist_ t_d( t_mu, t_sigma );   // target

    urng_ urng;

    dataset_ proposals(p_n); 
    {
        // Generate proposal values and their log_pdf
        // {(x,log_pdf(x)):i=1,...,p_n}
        BOOST_AUTO(
            vg,
            make_random_generator(
                urng,
                p_d
            )
        );

        BOOST_FOREACH(data_& d,proposals)
        {
            val_ x = vg();
            d = data_(
                x,
                stat::log_unnormalized_pdf(p_d,x)
            );
            
        }

    }
    vals_ is_weights( p_n );
    {
        // is_weights <- log_pdf(t_d) - log_pdf(p_d)

        BOOST_AUTO(
            r,
            tk::make_ratio_pdf(t_d,p_d)
        );

        vals_it_ i = boost::begin( is_weights );
        BOOST_FOREACH(const data_& d, proposals)
        {

            *i = stat::log_unnormalized_pdf( t_d, fusion::at_key<key1_>(d) ) 
                - fusion::at_key<key2_>(d);
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
            << prepare_weights_::header << std::endl
            << prepare_weights << std::endl;
    }
    {
    
        range1_ r1 = stat::fusion::at_key::make_range<key1_>(
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

        dataset_ targets;
        ks_stat_ ks_stat;
        {
            os << "proposal : " << tk::description(p_d) << std::endl; 
            os << "target : " << tk::description(t_d) << std::endl; 
            unsigned i = 0;
            unsigned n_draws = n1;
            targets.clear();
            while(i<n_loops){
                targets.reserve(targets.size()+n_draws);
                for(unsigned j = 0; j<n_draws; j++)
                {
                    BOOST_AUTO(x,vg());
                
                    targets.push_back( 
                        data_(
                            fusion::make_pair<key1_>( x ),
                            fusion::make_pair<key2_>( 
                                stat::cdf(t_d,x)
                            )
                        )
                    );
                }
                ks_stat(
                    boost::begin(targets),
                    boost::end(targets)
                );
                os << ks_stat.description() << std::endl;
                ++i;
                n_draws *= n2;
            }
        }
        
    }

    os << "<-" << std::endl;

}


