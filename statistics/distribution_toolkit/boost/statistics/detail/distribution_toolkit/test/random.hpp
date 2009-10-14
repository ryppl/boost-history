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
#include <ext/algorithm>
#include <boost/range.hpp>
#include <boost/static_assert.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/map_fwd.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/include/at_key.hpp>

#include <boost/statistics/detail/fusion/functor/map_identity_f.hpp>
#include <boost/statistics/detail/non_parametric/kolmogorov_smirnov/statistic.hpp>

#include <boost/statistics/detail/distribution_toolkit/meta/bind_delegate.hpp>
#include <boost/statistics/detail/distribution_toolkit/fun_wrap/cdf.hpp>

#include <boost/statistics/detail/distribution_toolkit/random/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/data/include.hpp>
#include <boost/statistics/detail/distribution_toolkit/unscope/os.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace toolkit{
namespace test{

template<typename D,typename N>
void random(
    const D& dist,
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

    using namespace boost;
    
    // Types 
    typedef typename common::meta::value<D>::type     val_;
    typedef mt19937                                   urng_;

    typedef typename meta::bind_delegate<D>::type     fun_;
//    typedef function<val_(const val_&)>               fun_;

    typedef detail::fusion::functor::map_identity_f<
        mpl::int_<0>,
        mpl::int_<1>,
        fun_
    > functor_;

   typedef typename functor_::identity::key_ key1_;
   typedef typename functor_::function::key_ key2_;


    typedef non_parametric::kolmogorov_smirnov::statistic<
        val_,
        key1_,
        key2_
    > kolmogorov_smirnov_stat_;
    typedef typename boost::result_of<functor_(const val_&)>::type   data_;
    typedef std::vector<data_>                              dataset_;

    // Constants
    
    urng_ urng;

    typedef boost::function<data_()> gen_data_;
    
    dataset_ dataset;
    kolmogorov_smirnov_stat_ kolmogorov_smirnov_stat;

    {
        typedef math::chi_squared_distribution<val_> dist_;
        const val_ df = 10;
        dist_ dist( df );

        functor_ functor(
            make_bind_delegate<fun_wrap::cdf_>(dist)
        );

        BOOST_AUTO(
            vg,
            make_random_generator(urng,dist)
        );

        gen_data_ gen_data = boost::lambda::bind<data_>(
            functor,
            boost::lambda::bind(
                boost::ref(
                    vg
                )
            )
        );

        os << description(dist) << std::endl; 
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
            
            kolmogorov_smirnov_stat(boost::begin(dataset),boost::end(dataset));
            os << kolmogorov_smirnov_stat.description() << std::endl;
            ++i;
            n_draws *= n2;
        }
    }
}


}// test
}// toolkit
}// distribution
}// detail
}// statistics
}// boost


#endif