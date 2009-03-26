/////////////////////////////////////////////////////////////////////////////
// normal_given_normal.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_PARAMETER_NORMAL_GIVEN_NORMAL_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_PARAMETER_NORMAL_GIVEN_NORMAL_HPP_ER_2009
#include <cmath>
#include <string>
#include <boost/format.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/utility/dont_care.hpp>
#include <boost/shared_features/parameter.hpp>
#include <boost/shared_features/depends_on.hpp>
#include <boost/shared_features/set.hpp>

#include <boost/shared_features/feature/scalar.hpp>
#include <boost/conditionally_specified_distribution/crtp/normal.hpp>
#include <boost/conditionally_specified_distribution/parameter/normal.hpp>
#include <boost/conditionally_specified_distribution/keyword/parameter.hpp>
#include <boost/conditionally_specified_distribution/result_of/include.hpp>

namespace boost {
namespace conditionally_specified_distribution{
namespace parameter{

namespace impl
{
    template<typename ParY>
    class normal_given_normal_base{
        typedef normal_given_normal_base<ParY> super_t;

        public:
        struct identifier
            : conditionally_specified_distribution::identifier<ParY>{};
        typedef typename function_argument<ParY>::type
                                                            value_type;
        // inside interface

        struct result : function_argument<ParY>{};
        struct result_of_mu : result{};
        struct result_of_sigma : result{};
        struct result_of_beta : result{};

        // outside interface
        struct function_argument : result{};
        struct function_value : result{};

    };

    template<
        typename ParY,
        typename StateX,
        typename ParX,
        typename CorrXY
    >
    class normal_given_normal
        : public normal_given_normal_base<typename ParY::traits>,
        public crtp::normal<
            normal_given_normal<
                ParY,
                StateX,
                ParX,
                CorrXY
            >
        >
    {
        typedef normal_given_normal_base<typename ParY::traits>  super_t;
        public:
        typedef ParX                                    par_x;
        typedef ParY                                    par_y;
        typedef StateX                                  x;
        typedef CorrXY                                  rho;

        normal_given_normal()
        :   mu_((typename super_t::value_type)(0)),
            sigma_((typename super_t::value_type)(0)),
            beta_((typename super_t::value_type)(0))
        {}

        template<typename Args>
        normal_given_normal(const Args& args)
        :   mu_((typename super_t::value_type)(0)),
            sigma_(sigma(args[::boost::shared_features::kwd_set])),
            beta_(beta(args[::boost::shared_features::kwd_set]))
        {
                this->update(args);
        }


        normal_given_normal(const normal_given_normal& that)
        : mu_(that.mu_),sigma_(that.sigma_),beta_(that.beta_){}

        normal_given_normal&
        operator=(const normal_given_normal& that){
            if(&that!=this){
                mu_     = that.mu_;
                sigma_  = that.sigma_;
                beta_   = that.beta_;
            }
            return *this;
        }
        template<typename Args>
        void update(Args& args){
            this->update_impl(args[::boost::shared_features::kwd_set]);
        }
        typename result_of::mu<super_t>::type
        mu()const{ return mu_; }
        typename result_of::sigma<super_t>::type
        sigma()const{ return sigma_; }
        // TODO If the need arises, replace result_of_beta<this_type>
        // not urgent as beta() is most likely internal only
        typename super_t::result_of_beta::type
        beta()const{ return beta_; }


        typename result_of::mu<super_t>::type
        mu(utility::dont_care)const{ return this->mu(); }
        typename result_of::sigma<super_t>::type
        sigma(utility::dont_care)const{ return this->sigma(); }
        typename super_t::result_of_beta::type
        beta(utility::dont_care)const{ return this->beta(); }

        template<typename Args>
        std::string
        as_string(const Args& args)const{
            return this->as_string_impl(
                args[::boost::shared_features::kwd_set]);
        }

        private:
        typename super_t::value_type mu_;
        typename super_t::value_type sigma_;
        typename super_t::value_type beta_;

        template<typename Set>
        std::string
        as_string_impl(const Set& set)const{
            std::string str = "parameter::normal_given_normal (y|x)";
            str+= "mu = %1%, sigma = %2%";
            str+= "x= %3% ";
            format f(str);
            f%(this->mu())%(this->sigma())%((this->get_x(set))());
            return f.str();
        }
        template<typename Set>
        typename super_t::value_type sigma(const Set& set){
            typename super_t::value_type sigma_y = get_par_y(set).sigma();
            typename super_t::value_type r = get_rho(set)();
            return sqrt(1.0-r*r)*sigma_y;
        }
        template<typename Set>
        typename super_t::value_type beta(const Set& set){
            typename super_t::value_type sigma_x = get_par_x(set).sigma();
            typename super_t::value_type sigma_y = get_par_y(set).sigma();
            typename super_t::value_type r = get_rho(set)();
            return r*(sigma_y/sigma_x);
        }

        public://TODO private
        template<typename Set>
        void update_impl(Set& set){
            typename super_t::value_type x_val = get_x(set)();
            typename super_t::value_type mu_x = get_par_x(set).mu();
            typename super_t::value_type mu_y = get_par_y(set).mu();
            (this->mu_) = mu_y + this->beta()*(x_val-mu_x);
        }

        template<typename Set>
        typename Set::template result_of_extract<par_y>::type const&
        get_par_y(const Set& set)const{
            return set.template extract<par_y>();
        }
        template<typename Set>
        typename Set::template result_of_extract<par_x>::type const&
        get_par_x(const Set& set)const{
            return set.template extract<par_x>();
        }
        template<typename Set>
        typename Set::template result_of_extract<rho>::type const&
        get_rho(const Set& set)const{
            //std::cout << "calling rho" << std::endl;
            return set.template extract<rho>();
        }
        template<typename Set>
        typename Set::template result_of_extract<x>::type const&
        get_x(const Set& set)const{
            return set.template extract<x>();
        }
    }; //normal_given_normal

} // namespace impl
namespace feature{
    template <
        typename ParY,
        typename StateX,
        typename ParX,
        typename CorrXY
    >
    struct normal_given_normal
      : shared_features::depends_on<
        mpl::vector<
            ParY,
            StateX,
            ParX,
            CorrXY
        >
      >
    {
        /// INTERNAL ONLY
        typedef impl::normal_given_normal_base<typename ParY::traits> traits;
        typedef impl::normal_given_normal<
            ParY,
            StateX,
            ParX,
            CorrXY
        > impl;
    };
}//feature

}//distribution
}//parameter
}//boost

#endif
