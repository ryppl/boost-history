/////////////////////////////////////////////////////////////////////////////
// pair_independent.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_PAIR_INDEPENDENT_HPP_ER_2009
#define BOOST_CONDITIONALLY_SPECIFIED_DISTRIBUTION_PAIR_INDEPENDENT_HPP_ER_2009
#include <string>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/parameter/keyword.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/assert.hpp>
//#include <boost/utility/assert_is_base_of.hpp>
#include <boost/utility/dont_care.hpp>
#include <boost/conditionally_specified_distribution/keyword/parameter.hpp>
#include <boost/conditionally_specified_distribution/result_of/include.hpp>
#include <boost/conditionally_specified_distribution/crtp/pair_independent.hpp>
#include <boost/shared_features/depends_on.hpp>
#include <boost/shared_features/result_of_extract.hpp>

namespace boost {
namespace conditionally_specified_distribution {
namespace parameter {

namespace impl
{

    template<typename T0,typename T1>
    class pair_independent_base{
        template<typename Args,typename T>
        struct result_of_T
          : add_reference<
                typename add_const<
                    typename
                        shared_features::result_of_extract<Args,T>::type
                >::type
            >
        {};
        public:
        struct identifier
        : conditionally_specified_distribution::identifier<T0>{
                //TODO chek same as T1
        };

        template<typename S>
        struct result_of_first{};
        template<typename F,typename Args>
        struct result_of_first<F(Args)> : result_of_T<Args,T0>
        {};
        template<typename S>
        struct result_of_second{};
        template<typename F,typename Args>
        struct result_of_second<F(Args)> : result_of_T<Args,T1>
        {};


        // outside interface
        struct function_argument
        : conditionally_specified_distribution::function_argument<T0>{};
        struct function_value
        : conditionally_specified_distribution::function_value<T0>{};

    };

    template<typename T0,typename T1>
    class pair_independent
        : public pair_independent_base<
            typename T0::traits,
            typename T1::traits
        >,
        public crtp::pair_independent< pair_independent<T0,T1> >
    {
        typedef pair_independent_base<T0,T1>                super_t;
        public:
        pair_independent(){}

        pair_independent(utility::dont_care){}

        pair_independent(const pair_independent& that){}

        pair_independent& operator=(const pair_independent& that){
            if(&that!=this){}
            return *this;
        }

        template<typename Args>
        void update(const Args& args){
            this->update_impl(args[shared_features::kwd_set],args);
        }

        template<typename Args>
        typename result_of::first<super_t(const Args&)>::type
        first(const Args& args) const {
            return first_impl(args[shared_features::kwd_set],args);
        }

        template<typename Args>
        typename result_of::second<super_t(const Args&)>::type
        second(const Args& args) const {
            return second_impl(args[shared_features::kwd_set],args);
        }

        template<typename Args>
        std::string as_string(const Args& args)const{
            std::string str = "parameter::pair_independent :";
            str+= "\n first : ";
            str+= first_impl(
                args[shared_features::kwd_set],args).as_string(args);
            str+= "\n second : ";
            str+= second_impl(
                args[shared_features::kwd_set],args).as_string(args);
            return str;
        }

        private:
        template<typename Set,typename Args>
        typename result_of::first<super_t(const Args&)>::type
        first_impl(const Set& set,const Args& args) const {
            check_first(set,args,set.template extract<T0>());
            return set.template extract<T0>();
        }

        template<typename Set,typename Args,typename First>
        void check_first(const Set& set,const Args& args,const First&) const {
            typedef typename
                result_of::first<super_t(const Args&)>::type cref_t;
            BOOST_ASSERT((
                is_same<
                    typename utility::remove_qualifier<cref_t>::type,
                    First
                >::value
            ));
            BOOST_ASSERT((
                is_same<
                    cref_t,
                    const First&
                >::value
            ));
        }
        template<typename Set,typename Args,typename Second>
        void check_second(const Set& set,const Args& args,
                                            const Second&) const {
            typedef typename
                result_of::second<super_t(const Args&)>::type cref_t;
            BOOST_ASSERT((
                is_same<
                    typename utility::remove_qualifier<cref_t>::type,
                    Second
                >::value
            ));
            BOOST_ASSERT((
                is_same<
                    cref_t,
                    const Second&
                >::value
            ));
        }
        template<typename Set,typename Args>
        typename result_of::second<super_t(const Args&)>::type
        second_impl(const Set& set,const Args& args) const {
            return set.template extract<T1>();
        }

        template<typename Set,typename Args>
        void update_impl(Set& set,const Args& args){
            (set.template extract<T0>()).update(args);
            (set.template extract<T1>()).update(args);
        }
    };

} // namespace impl

namespace feature{
    template <typename T0,typename T1>
    struct pair_independent
      : shared_features::depends_on<
        mpl::vector<T0,T1>
      >
    {
      typedef impl::pair_independent_base<
        typename T0::traits,
        typename T1::traits
      >                                                traits;
      typedef impl::pair_independent<T0,T1>             impl;
    };
}


}//distribution
}//parameter
}//boost

#endif
