/////////////////////////////////////////////////////////////////////////////
// regression_coefficient.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_PARAMETER_DEPENDENT_REGRESSION_COEFFICIENT_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_PARAMETER_DEPENDENT_REGRESSION_COEFFICIENT_HPP_ER_2009
#include <vector>
#include <string>
#include <boost/range.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/utility/dont_care.hpp>
#include <boost/utility/remove_qualifier.hpp>
#include <boost/utility/assert_is_base_of.hpp>
#include <boost/utility/container_to_string.hpp>
#include <boost/shared_features/feature/keyword.hpp>
#include <boost/shared_features/parameter.hpp>
#include <boost/shared_features/depends_on.hpp>
#include <boost/conditionally_specified_distribution/parameter/detail/skip_position_operations.hpp>
#include <boost/conditionally_specified_distribution/result_of/include.hpp>
namespace boost{
namespace conditionally_specified_distribution{
namespace parameter{
namespace detail{

/// A regression_coefficient computes the inner product (excluding the
/// element at a specified position) for all datapoints.
///
/// {$\eta_{i,-j} = x_{i,0} \beta_0 + ... + x_{i,-j}\beta_{-j}$
/// $+x_{i,j+1} \beta_{j+1}+...+x_{i,J-1} \beta_{J}$,i=1,...,n}

namespace impl{
        template<typename Id,typename DataBase,typename CoeffsBase>
        class regression_coefficient_base{
            public:
            typedef std::size_t                      size_type;
            // operations defined in regression_coefficient

            struct identifier : mpl::identity<Id>{};
            struct result_of_position : mpl::identity<size_type>{};

            struct result_of_range_data
              :  conditionally_specified_distribution
                ::result_of::functor<DataBase>
            {};
            protected:
            struct data_unit
            : range_value<
                typename utility::remove_qualifier<
                    typename result_of_range_data::type
                >::type
             >
            {};
            struct result_of_range_covariate
                : conditionally_specified_distribution
                    ::result_of::range_covariate<
                    typename data_unit::type
                >
            {};

            public:
            struct result_of_range_coefficient
                :  conditionally_specified_distribution
                    ::result_of::functor<CoeffsBase>
            {};

            struct result_of_matching_covariate
                :  range_value<
                        typename utility::remove_qualifier<
                            typename result_of_range_covariate::type
                        >::type
                    >
            {};

            protected:

            typedef typename
                range_value<
                    typename utility::remove_qualifier<
                        typename result_of_range_coefficient::type
                    >::type
                >::type                                  value_type;
            typedef std::vector<value_type>             container_type;
            public:
            struct result_of_range_skip_position_covariate_dot_coefficient
             : mpl::identity<const container_type&>
            {};
            // outside interface
            struct function_value    : mpl::identity<value_type>{};
            struct function_argument : mpl::identity<value_type>{};
        };
    template<typename Id, typename Dataset,typename Coefficients>
    class regression_coefficient
    : public regression_coefficient_base<
        Id, typename Dataset::traits, typename Coefficients::traits
    >{
        typedef regression_coefficient_base<
            Id,
            typename Dataset::traits,
            typename Coefficients::traits
        >                                                           super_t;
        typedef Dataset                                             data_t;
        typedef Coefficients                                        coeffs_t;
        typedef parameter::detail::skip_position_operations         impl_t;
        public:
        regression_coefficient(typename super_t::size_type pos)
        :position_(pos){
                //Warning : state_ default initializes
        }
        template<typename Args>
        regression_coefficient(const Args& args)
        :position_(args[shared_features::kwd<Id>::position|0]){
            //TODO less clumsy
            typedef mpl::bool_<false> f;
            alert_if(args[shared_features::kwd_do_initialize|f()],
                args[shared_features::kwd<Id>::position|-1]
            );
            this->update_if(
                args[shared_features::kwd_do_initialize|f()],
                args
            );
        }

        regression_coefficient(const regression_coefficient& that)
        :position_(that.position_)
        {
            this->set(that.state_);
        }

        regression_coefficient&
        operator=(const regression_coefficient& that){
            if(&that!=this){
                (this->position_) = that.position_;
                (this->state_) = that.state_;
            }
            return *this;
        }

        typename conditionally_specified_distribution
            ::result_of::position<super_t>::type
        position()const{ return this->position_; }

        typename conditionally_specified_distribution
            ::result_of::position<super_t>::type
        position(utility::dont_care)const{
            return (this->position());
        }

        template<typename Args>
        typename conditionally_specified_distribution
            ::result_of::range_data<super_t>::type
        range_data(const Args& args)const{
            return this->get_data(args[shared_features::kwd_set])();
        }

        template<typename Args>
        typename conditionally_specified_distribution
            ::result_of::range_coefficient<super_t>::type
        range_coefficient(const Args& args)const{
            return this->get_coeffs(args[shared_features::kwd_set])();
        }

        typename conditionally_specified_distribution
            ::result_of::matching_covariate<super_t>::type
        matching_covariate(
            typename super_t::data_unit::type const & u
        )const{
            impl_t impl(this->position());
            return impl.matching_element(u.range_covariate());
        }

        typename conditionally_specified_distribution
            ::result_of::matching_covariate<super_t>::type
        matching_covariate(
            utility::dont_care,
            typename super_t::data_unit::type const & u
        )const{
            return this->matching_covariate(u);
        }

        //TODO range_matching_covariate

        // {<x[-i],b[j]> : i =1,...,n}
        typename conditionally_specified_distribution::result_of
            ::range_skip_position_covariate_dot_coefficient<
            super_t
        >::type
        range_skip_position_covariate_dot_coefficient()const{
            return this->state_;
        }
        typename conditionally_specified_distribution::result_of
            ::range_skip_position_covariate_dot_coefficient<super_t>::type
        range_skip_position_covariate_dot_coefficient(
            utility::dont_care
        )const{
            return this->range_skip_position_covariate_dot_coefficient();
        }

        template<typename Args>
        void update(const Args& args){
            typedef typename utility::remove_qualifier<
                typename conditionally_specified_distribution
                ::result_of::range_coefficient<super_t>::type
            >::type                                      coeffs_type;
            typedef op<coeffs_type> op_t;
            op_t the_op(this->position(),this->range_coefficient(args));
            state_.clear();
            std::transform(
                begin(range_data(args)),
                end(range_data(args)),
                back_inserter(
                    (this->state_)
                ),
                the_op
            );
        }

        template<typename Args>
        std::string
        as_string(const Args& args)const{
            typedef utility::container_to_string to_str_t;
            std::string str = "[position = %1%\n";
            format f(str);
            f%(this->position(args));
            str = f.str();
            str += "range_coefficient =";
            str += to_str_t::get_indexed(this->range_coefficient(args));
            str += "\nrange_skip_position_covariate_dot_coefficient = ";
            str += to_str_t::get_indexed(
                this->range_skip_position_covariate_dot_coefficient(args)
            );
            str +="]";
            return str;
        }
        private:
        void alert_if(mpl::bool_<true>,int i)const{
            BOOST_ASSERT(i>-1);
        }
        void alert_if(mpl::bool_<false>,int i)const{}
        template<typename Args>
        void update_if(mpl::bool_<true>,const Args& args){
            this->update(args);
        }
        void update_if(mpl::bool_<false>,utility::dont_care){}

        template<typename R>
        void set(const R& r){
            state_.clear();
            copy(
                begin(r),
                end(r),
                back_inserter(
                   (this -> state_)
                )
            );
        }
        template<typename R>
        class op{
            public:
            typedef typename range_value<R>::type result_type;

            op(typename super_t::size_type pos, const R& coeffs)
            :impl_(pos),coeffs_(coeffs){}

            op(const op& that)
            :impl_(that.impl_),coeffs_(that.coeffs_){}

            template<typename DataUnit>
            result_type
            operator()(const DataUnit& u)const{
                return impl_.inner_product_skip_position(
                    u.covariate(),
                    coeffs_
                );
            }

            private:
            op& operator=(const op&);
            impl_t      impl_;
            const R&    coeffs_;
        };
        template<typename Set>
        typename Set::template result_of_extract<coeffs_t>::type const&
        get_coeffs(const Set& set)const{
            return set.template extract<coeffs_t>();
        }
        template<typename Set>
        typename Set::template result_of_extract<data_t>::type const&
        get_data(const Set& set)const{
            return set.template extract<data_t>();
        }
        typename super_t::size_type       position_;
        typename super_t::container_type  state_;
    };
} //impl

namespace feature{
    // Provided for testing purposes only
    // Don't inherit from this feature.
    template<typename Id, typename Dataset,typename Coefficients>
    struct regression_coefficient : shared_features::depends_on<
        mpl::vector<
            Dataset,
            Coefficients
        >
    >{
        typedef impl::regression_coefficient_base<
            Id,
            typename Dataset::traits,
            typename Coefficients::traits
        >                                                            traits;
        typedef impl::regression_coefficient<Id,Dataset,Coefficients> impl;
    };
}

}//detail
}//parameter
}//csd
}//boost

#endif
