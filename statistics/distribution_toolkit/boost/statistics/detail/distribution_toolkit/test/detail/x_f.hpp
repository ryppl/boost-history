///////////////////////////////////////////////////////////////////////////////
// distribution::toolkit::test::x_f.hpp                               	 	 //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_TEST_DETAIL_X_F_HPP_ER_2009
#define BOOST_STATISTICS_DETAIL_DISTRIBUTION_TOOLKIT_TEST_DETAIL_X_F_HPP_ER_2009
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/statistics/detail/distribution_common/functor/cdf.hpp>
#include <boost/statistics/detail/distribution_common/distributions/reference/include.hpp>
#include <boost/statistics/detail/distribution_common/meta/value.hpp>
#include <boost/statistics/detail/distribution_common/meta/random/generator.hpp>

namespace boost{
namespace statistics{
namespace detail{
namespace distribution{
namespace toolkit{
namespace test{
namespace detail{

	template<typename T>
	class x_f{

		public:
		struct input;
		struct output1;
        typedef boost::fusion::map<
            boost::fusion::pair<input,T>,
            boost::fusion::pair<output1,T>
        > data_type;

		template<
        	typename D,
        	template<typename> class F = distribution::functor::cdf_
        >
		class functor{
			typedef distribution::functor::cdf_<D> funct_;				
			public:

			typedef data_type result_type;

			functor(const D& dist) : f(dist){}
        	functor(const functor& that) : f(that.f){}

			typedef typename distribution::meta::value<D>::type value_type;
        
			template<typename X>
        	result_type operator()(const X& x)const
        	{
        		return result_type(
            		boost::fusion::make_pair<input>(x),
            		boost::fusion::make_pair<output1>(
                    	this->f(x)
                	)
            	);
        	}

			private:
        	funct_ f;
        };

        typedef boost::function<data_type()> generator_type;

        template<
        	typename D,
            template<typename> class F = distribution::functor::cdf_
        >
        struct meta_generator{
        	typedef functor<D,F> functor_;
            typedef typename functor_::result_type res_;
        	typedef generator_type type;

			template<typename G>            
    		static type call(const D& dist,G& random_generator)
            {
        		return type(
                    boost::lambda::bind<res_>(
            			functor_(dist),
            			boost::lambda::bind(
                        	boost::ref(
                            	random_generator
                            )
                        )
                    )
                );
			}
		};        
	};

}// detail
}// test
}// toolkit
}// distribution
}// detail
}// statistics
}// boost

#endif
