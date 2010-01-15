//////////////////////////////////////////////////////////////////////////////
// non_parametric::kolmogorov_smirnov::frequency_adaptor.hpp                //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_NON_PARAMETRIC_KOLMOGOROV_SMIRNOV_FREQUENCY_ADAPTOR_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_NON_PARAMETRIC_KOLMOGOROV_SMIRNOV_FREQUENCY_ADAPTOR_HPP_ER_2009
#include <utility>

#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range.hpp>

#include <boost/statistics/detail/non_parametric/empirical_distribution/ordered_sample.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/framework/extractor.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace non_parametric{
	
    // Maps the result of empirical_distribution::ordered_sample to the input
    // required by kolmogorov_smirnov::statistic.
    template<
    	typename Kx,	
        typename Kcdf,
        typename Kn,
        typename Int = int,		
        typename T = double		// Int x; T cdf = cdf(dist,x)
    >
	struct frequency_adaptor{

		typedef Int int_;

    	typedef boost::fusion::map<
        	boost::fusion::pair<Kx,Int>,
            boost::fusion::pair<Kn,T>,
            boost::fusion::pair<Kcdf,T>
        > elem_;

		typedef elem_ type;

        template<typename D>
        struct fun{

        	fun(int_ n,const D& dist) 
            	: n_(n),dist_(dist){}
			fun(const fun& that) 
            	: n_(that.n_),dist_(that.dist_){} 
        
        	typedef elem_ result_type;
        
        	template<typename Pair>
        	result_type operator()(const Pair& pair)const{

				int_ x = pair.first;
                int_ i = pair.second;
            	T the_cdf = cdf(this->dist_,x);
            
            	return result_type(
                	boost::fusion::make_pair<Kx>(x),
                    boost::fusion::make_pair<Kn>(i),
					boost::fusion::make_pair<Kcdf>(the_cdf)                    
                );
        	}
            
            int_ n_;
            D dist_;

        };

		template<typename Map,typename D>
        struct apply1{

			typedef typename boost::range_iterator<const Map>::type it_;
			typedef fun<D> fun_;

			typedef boost::transform_iterator<fun_,it_> t_it_;
            typedef boost::iterator_range<t_it_> type;

			template<typename N>
			static type call(const Map& map, N n,const D& dist){
                fun_ f(n,dist);

				return type(
                    t_it_(
                    	boost::begin(map),
                        f
                    ),
                    t_it_(
                    	boost::end(map),
                        f
                    )
                );
            }
        };

		template<typename Acc,typename D>
        struct apply2{
            typedef boost::statistics::detail
                	::empirical_distribution::tag::ordered_sample tag_os_;
            typedef boost::accumulators::tag::count tag_n_;
		
			typedef typename boost::accumulators::detail
            	::extractor_result<Acc, tag_os_>::type cref_map_;
            typedef typename boost::remove_cv<
            	typename boost::remove_reference<
                	cref_map_
                >::type
            >::type map_;

			typedef apply1<map_,D> apply1_;
        	typedef typename apply1_::type type;
        
			template<typename AccSet>
			static type call(const AccSet& acc,const D& dist){
				return apply1_::call(
					boost::accumulators::extract_result<tag_os_>(acc),                		
					boost::accumulators::extract_result<tag_n_>(acc),
                    dist                		
                );
            }
    	};

		template<typename Acc,typename D>
    	typename apply2<Acc,D>::type
		static call(const Acc& acc,const D& dist){
        	typedef apply2<Acc,D> apply2_;
        	return apply2_::call(acc,dist);
        }

	};

    template<
    	typename Kx,	
        typename Kn,
        typename Kcdf,
        typename Acc,
        typename D
    >
    typename frequency_adaptor<Kx,Kn,Kcdf>::template apply2<Acc,D>::type
	frequency_adapt(const Acc& acc,const D& dist){
    	typedef frequency_adaptor<Kx,Kn,Kcdf> adaptor_;
        return adaptor_::call(acc,dist);
    }

}// non_parametric
}// detail
}// statistics
}// boost

#endif
