///////////////////////////////////////////////////////////////////////////////
//  as_random_regression.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_AS_RANDOM_REGRESSION_HPP_ER_2009
#define BOOST_AS_RANDOM_REGRESSION_HPP_ER_2009

#include <boost/mpl/bool.hpp>
#include <boost/type_traits.hpp>
#include <boost/random_regression/base.hpp>
namespace boost{


    // Maps a RandomDistribution (or RandomRegression) to RandomRegression
	template<typename RandomDist>
	class as_random_regression : RandomDist{
	    typedef RandomDist super_t;
	public:

        typedef typename RandomDist::result_type result_type;
        typedef typename RandomDist::input_type input_type;

        as_random_regression(const super_t& g)
        :super_t(g){}

        template<typename Args>
        as_random_regression(const Args& args)
        :super_t(args){}

 //       template<typename Args>
 //       as_random_regression(Args& args)
 //       :super_t(args){
 //           //No known need, but for consistency w as_random_distribution
 //       }

		as_random_regression(const as_random_regression& that)
		:super_t(static_cast<const super_t&>(that)){}

		as_random_regression&
		operator=(const as_random_regression& that){
		    if(&that!=this){
                super_t::operator=(that);
            }
		    return *this;
		}

        template<typename U,typename X>
        typename super_t::result_type
        operator()(U& u,const X& x){
            typedef typename
                is_base_of<random_regression_base,super_t>::type is;
            return impl(is(),u,x);
        }

    private:

    template <typename U,typename X >
    typename super_t::result_type
    impl(mpl::bool_<true>,U& u,const X& x) {
        return super_t::operator()(u,x);
    }

    template <typename U,typename X >
    typename super_t::result_type
    impl(mpl::bool_<false>,U& u,const X& x) {
        return super_t::operator()(u);
    }


};

}//boost
#endif

