//////////////////////////////////////////////////////////////////////////////
// non_parametric::kolmogorov_smirnov::check_convergence.hpp                //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_NON_PARAMETRIC_KOLMOGOROV_SMIRNOV_CHECK_CONVERGENCE_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_NON_PARAMETRIC_KOLMOGOROV_SMIRNOV_CHECK_CONVERGENCE_HPP_ER_2009

#include <boost/mpl/copy.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/accumulators/framework/extractor.hpp>
#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>
#include <boost/accumulators/framework/parameters/accumulator.hpp>
#include <boost/accumulators/framework/depends_on.hpp>
#include <boost/accumulators/framework/accumulator_set.hpp>
#include <boost/accumulators/statistics/stats.hpp>

#include <boost/statistics/detail/non_parametric/empirical_distribution/ordered_sample.hpp>
#include <boost/statistics/detail/non_parametric/kolmogorov_smirnov/statistic.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace empirical_distribution{

    // Usage:
    //     check_convergence<> check;
    //     check(n_loops,n,n_factor,distribution,random_generator,os);
    // Generates random samples of size { k = n^p : p=1,...,n_loops} and for
    // each computes the kolmogorov-smirnov statistic. The results are passed
    // to os.
    template<
    	typename T1 = double,
    	typename MoreFeatures = boost::accumulators::stats<>
    >
    struct check_convergence
    {
    
    	typedef T1 value_type;
        typedef kolmogorov_smirnov::tag::kolmogorov_smirnov tag_;
    	typedef boost::accumulators::stats<tag_> stats_;
    	typedef boost::mpl::push_back<boost::mpl::_,boost::mpl::_> op_;
    
       	typedef typename boost::mpl::copy<
        	MoreFeatures,boost::mpl::back_inserter< stats_ >
    	>::type mpl_features;
    
    	public:
    
    	check_convergence(){}

        template<typename D,typename G>
        struct traits{
            typedef typename D::value_type val_;
            typedef typename G::distribution_type random_;
            typedef typename random_::result_type sample_type; 

            typedef boost::accumulators::accumulator_set<
            	sample_type,
                typename mpl_features::type
            > acc_;
        };
    
        template<typename D,typename G>
        typename traits<D,G>::acc_
        operator()(
            long n_loops,
            long n, 			// n *= n_factor at each loop
            long n_factor, 
            const D& dist,	
            G& gen,
            std::ostream& os
        )const{
            return (*this)(
            	boost::mpl::void_(),
            	n_loops, 
                n, 
                n_factor, 
                dist, 
                gen, 
                default_fun<D>(),
                os
            );
        }

        template<
            typename Args,
            typename D,
            typename G,
            typename F,
            typename Out
        >
        typename traits<D,G>::acc_
        operator()(
            const Args& args,
            long n_loops,
            long n, // n *= n_factor at each loop
            long n_factor, 
            const D& dist,
            G& gen,
            const F& fun,
            Out& out
        )const{
            typedef traits<D,G> traits_;
            typedef typename traits_::val_ val_;
            typedef typename traits_::random_ random_;
            typedef typename traits_::sample_type sample_type;
            typedef typename traits_::acc_ acc_;
            
            acc_ acc(args);
        
            for(long i1 = 0; i1<n_loops; i1++){
                for(long i2 = 0; i2< n; i2++){
                    sample_type x = gen();
                    acc(x);
                } // grows sample by n
				
                fun(acc,dist,out);                
                n *= n_factor;
            }
            
            return acc;
        }

        template<typename D>
    	struct default_fun{

            typedef boost::numeric::bounds<value_type> bounds_;

            default_fun() : ks0( bounds_::highest() ){}
		
            template<typename AccSet>
            void operator()(const AccSet& acc,const D& d,std::ostream& os)const{
                namespace ns = kolmogorov_smirnov;
            	value_type ks1 = ns::statistic<value_type>( acc, d );
                bool ok = ks1 - ks0 < bounds_::smallest(); 
                os 
                    << '('
                    << boost::accumulators::extract::count(acc) 
                    << ','
                    << ks1
                    << ','
                    << ok
                    << ')'
                    << std::endl;	    
            }

            private:
            mutable value_type ks0;

    	};
    
    };
    
}// empirical_distribution
}// detail
}// statistics
}// boost

#endif
