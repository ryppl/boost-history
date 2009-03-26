/////////////////////////////////////////////////////////////////////////////
// normal.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_PARAMETER_NORMAL_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_PARAMETER_NORMAL_HPP_ER_2009
#include <string>
#include <boost/mpl/apply.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/format.hpp>
#include <boost/utility/assert_is_base_of.hpp>
#include <boost/utility/dont_care.hpp>
#include <boost/conditionally_specified_distribution/keyword/parameter.hpp>
#include <boost/conditionally_specified_distribution/result_of/include.hpp>
#include <boost/conditionally_specified_distribution/crtp/include.hpp>
#include <boost/shared_features/depends_on.hpp>

namespace boost {
namespace conditionally_specified_distribution {
namespace parameter {

namespace impl
{

    template<typename RealType,typename Id>
    class normal_base{

        public:
        struct identifier : mpl::identity<Id>{};
        typedef RealType value_type;

        // outside interface
        struct function_argument : mpl::identity<value_type>{};
        struct function_value : function_argument{};

        // inside interface
        struct result_of_mu : function_argument{};
        struct result_of_sigma : function_argument{};

    };

    template<typename RealType,typename Id = mpl::void_>
    class normal
        : public normal_base<RealType,Id>,
        public crtp::normal< normal<RealType,Id> >
    {
        typedef normal_base<RealType,Id>                    super_t;
        typedef parameter::kwd<Id>                          kwd;
        public:

        normal()
        : mu_((typename super_t::value_type)(0)),
        sigma_((typename super_t::value_type)(1)){}

        normal(
            typename super_t::value_type mu,
            typename super_t::value_type sigma
        )
            :   mu_(mu),sigma_(sigma){}

        template<typename Args>
        normal(const Args& args)
            //: //TODO pass normal directly as an option
            :mu_(args[kwd::mu|(typename super_t::value_type)(0)]),
            sigma_(args[kwd::sigma|(typename super_t::value_type)(1)])
        {
        }

        normal(const normal& that) : mu_(that.mu_),sigma_(that.sigma_){}

        normal& operator=(const normal& that){
            if(&that!=this){
                mu_ = that.mu_;
                sigma_ = that.sigma_;
            }
            return *this;
        }

        void update(utility::dont_care){}

        typename result_of::mu<super_t>::type
        mu() const {
            return this->mu_;
        }
        template<typename Args>
        typename result_of::mu<super_t>::type
        mu(const Args& args) const {
            return this->mu();
        }

        typename result_of::sigma<super_t>::type
        sigma() const {
            return this->sigma_;
        }
        template<typename Args>
        typename result_of::sigma<super_t>::type
        sigma(const Args& args) const { return this->sigma(); }

        std::string as_string()const{
            std::string str = "parameter::impl::normal : ";
            str += " mu = %1%";
            str += " sigma = %2%";
            format f(str); f%mu_%sigma_;
            return f.str();
        }

        std::string as_string(utility::dont_care)const{
            return this->as_string();
        }

        private:
        typename super_t::value_type mu_;
        typename super_t::value_type sigma_;
    };

} // namespace impl

namespace feature{
    template <typename RealType,typename Id>
    struct normal
      : shared_features::depends_on0
    {
      typedef impl::normal_base<RealType,Id>    traits;
      typedef impl::normal<RealType,Id>         impl;

    };
}


}//distribution
}//parameter
}//boost

#endif
