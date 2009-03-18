////////////////////////////////////////////////////////////////////////////
// sampler.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_ADAPTIVE_REJECTION_SAMPLING_SAMPLER_HPP_ER_2009
#define BOOST_ADAPTIVE_REJECTION_SAMPLING_SAMPLER_HPP_ER_2009
#include <vector>
#include <stdexcept>
#include <boost/assert.hpp>
#include <stdexcept>
#include <boost/adaptive_rejection_sampling/approximation.hpp>
namespace boost{
namespace adaptive_rejection_sampling{
    // Interface:
    // Constructor (forwards to approximation)
    //    sampler(
    //        const DistFun& f,
    //        result_type x0_init,
    //        result_type x1_init,
    //        size_type max_recursion
    //    )
    //    sampler(
    //        const DistFun& f,
    //        typename approx_t::size_type max_recursion
    //    )
    // Modifier:
    // initialize(x1,x2) // defined in approximation
    // result_type operator()(Unif01& e)
    //
    template<
        typename DistFun,
        template<typename,typename> class Cont = std::vector,
        template<typename> class Alloc = std::allocator
    >
    class sampler : public approximation<
        DistFun,
        Cont,
        Alloc
    >
    {

        typedef approximation<
            DistFun,
            Cont,
            Alloc
        >                                                 approx_t;
        public:
        typedef typename approx_t::value_type 		        result_type;

        sampler(
            const DistFun& f,
            typename approx_t::size_type max_recursion
        ):approx_t(f,max_recursion),
        is_postponed_(false),
        postponed_(0.0)
        {
            //warning not initialized
        }
        sampler(
            const DistFun& f,
            result_type x0_init,
            result_type x1_init,
            typename approx_t::size_type max_recursion
        ):approx_t(f,x0_init,x1_init,max_recursion)
        ,is_postponed_(false),postponed_(0.0)
        {}

        sampler(const sampler& that)
        :approx_t(that),
        is_postponed_(that.is_postponed_),
        postponed_(that.postponed_){}

		sampler&
		operator=(const sampler& that){
		    if(&that!=this){
                approx_t::operator=(that);
                is_postponed_ = that.is_postponed_;
                postponed_ = that.postponed_;
		    }
		    return *this;
		}
		// e() samples from (0,1) or [0,1) or (0,1] or [0,1]
        template<typename Unif01>
        result_type operator()(Unif01& e){ //not a const because
            // each call may update the approximation to dist_fun_
            if(is_postponed_){
                this->update(postponed_);
                is_postponed_ = false;
            }
            bool reject = true ;
            result_type u, x, thresh;
            while(reject){
                u = gen_excl_endpoints(e);
                //std::cout << "u1=" << std::setprecision(10) << u;
                x = approx_t::inverse_cdf(u);
                //std::cout << "x=" << std::setprecision(10) << x;
                thresh = exp_lower_minus_upper(x);
                u = gen_excl_endpoints(e);
                //std::cout << "u2=" << std::setprecision(10) << u;
                reject = !( u <= thresh);

                if( reject ){
                    thresh = exp_y_minus_upper(x);
                    u = gen_excl_endpoints(e);
                    //std::cout << "u3=" << std::setprecision(10) << u;
                    reject = !(u<=thresh);
                    if(reject){
                        approx_t::update(x);
                    }else{
                        postpone_update(x);
                    }
                }
            }
            return x;
        }

		private:
		bool is_postponed_;
		result_type postponed_;
        result_type
        exp_lower_minus_upper(result_type x)const{
            result_type a = approx_t::lower(x); // - inf?!
            a -= approx_t::upper(x);
            return math_limit::safer_exp(a);
        }
        result_type
        exp_y_minus_upper(result_type x)const{
            result_type a = approx_t::get_y(x);
            a -= approx_t::upper(x);
            return math_limit::safer_exp(a);
        }
		void postpone_update(result_type x){
            is_postponed_ = true;
            postponed_  = x;
		}
		template<typename Unif01>
		result_type gen_excl_endpoints(Unif01& e)const{
            result_type tmp = e();
            bool is_endpoint = ((tmp==0.0)||(tmp==1.0));
            BOOST_ASSERT(((tmp>0.0)&&(tmp<1.0)) || is_endpoint);
            if(is_endpoint){
                tmp = gen_excl_endpoints(e);
            }//Rejection sampling
            return tmp;
        }
    };
}
}

#endif // SAMPLER_HPP_INCLUDED
