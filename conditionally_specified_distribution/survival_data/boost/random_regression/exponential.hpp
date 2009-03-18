///////////////////////////////////////////////////////////////////////////////
// exponential.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_RANDOM_REGRESSION_EXPONENTIAL_HPP_ER_2009
#define BOOST_RANDOM_REGRESSION_EXPONENTIAL_HPP_ER_2009
#include <functional>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <cmath>
#include <boost/assert.hpp>
#include <boost/format.hpp>
#include <boost/mpl/void.hpp>
#include <boost/range.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/exponential_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random_regression/keyword.hpp>
#include <boost/random_regression/base.hpp>
namespace boost{
namespace random_regression{

	/// \class exponential
    /// \brief Simulates \f$ \mathrm{Exponential}(\lambda) \f$, with
    /// \f$ \lambda = exp(\langle x, coefficients \rangle) \f$
	template<
        typename Expon = boost::exponential_distribution<double>,
        template<typename> class Unif01 = boost::uniform_real,
        typename Id = mpl::void_
    >
	class exponential : random_regression_base{
    public:
        typedef typename Expon::result_type                 result_type;
        typedef typename Unif01<result_type>::input_type    input_type;
    private:
        typedef std::vector<result_type>			          coeffs_type;
        typedef exponential<Expon,Unif01>                   this_type;
        typedef Unif01<result_type>                         unif01_type;
	public:
//        template<typename R>
//        exponential(const R& coefficients)
//        {
//            set(coefficients);
//        }

        template<typename ArgumentPack>
        exponential(const ArgumentPack& args)
        {
            set(args[kwd<Id>::coefficients|args]);
            //in the absence of kwd, args has to be a Range
        }

        exponential(const exponential& that)
        {
            set(that.coefficients_);
        }

        exponential&
        operator=(const exponential& that){
            if(&that!=this){
                set(that.coefficients_);
            }
            return *this;
        }

        //U models UniformRandomNumberGenerator
        template<typename U,typename R>
        result_type
        operator()(U& urng, const R& covariate){
            if(size(covariate)!=size(coefficients_)){
                std::string str = "random_regression::";
                str+= "exponential(urng,covariate)";
                str+= " size(covariate) = %1% !=";
                str+= " size(coefficients) = %2%";
                format f(str);
                f % size(covariate) % size(coefficients_);
                throw std::runtime_error(f.str());
            }else{
                result_type log_lambda
                = std::inner_product(
                    begin(coefficients_),
                    end(coefficients_),
                    begin(covariate),
                    (result_type)(0),
                    std::plus<result_type>(),
                    std::multiplies<result_type>()
                );
                typedef boost::variate_generator<U&,unif01_type> gen_t;
                gen_t gen(urng,unif01_type());
                result_type num = expon1_(gen);
                //std::cout << "num/exp(log_lambda)="
                //<< num/exp(log_lambda) << std::endl;
                return num/exp(log_lambda);
            }
        }

        template<typename R>
        void set(const R& coefficients){
            copy(
                begin(coefficients),
                end(coefficients),
                back_inserter(coefficients_)
            );
        }

	private:
		coeffs_type 	coefficients_;
		Expon	        expon1_;
	};
}}


#endif
