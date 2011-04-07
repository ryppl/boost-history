
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#include <boost/local/function.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    double sum = 0.0;
    int factor = 10;

    std::vector<double> v(1000000);
    std::fill(v.begin(), v.end(), 10);

void (*ERROR_missing_result_type_before_the_local_function_parameter_macro_id20)(); typedef void (*boost_local_auxXdeduce_result_tag20)( int ERROR_missing_result_type_before_the_local_function_parameter_macro_id20); typedef __typeof__(boost::type_of::ensure_obj(boost::scope_exit::aux::wrap( boost::scope_exit::aux::deref( ERROR_missing_result_type_before_the_local_function_parameter_macro_id20, (boost_local_auxXdeduce_result_tag20)0)))) boost_local_auxXdeduce_result_wrap20; typedef boost_local_auxXdeduce_result_wrap20::type boost_local_auxXdeduce_result_capture20; struct boost_local_auxXdeduce_result_params20 { typedef boost_local_auxXdeduce_result_capture20 function_ptr_type; }; typedef boost::remove_pointer< boost_local_auxXdeduce_result_params20::function_ptr_type >::type boost_local_auxXdeduce_result_function_type20; typedef boost::function_traits< boost_local_auxXdeduce_result_function_type20>::result_type boost_local_auxXresult_type20; typedef void (*boost_se_tag_0_20)(int factor ); typedef void (*boost_se_tag_1_20)(int & sum ); typedef __typeof__(boost::type_of::ensure_obj(boost::scope_exit::aux::wrap( boost::scope_exit::aux::deref(factor, (boost_se_tag_0_20)0)))) boost_se_wrapped_t_0_20; typedef boost_se_wrapped_t_0_20::type boost_se_capture_t_0_20; typedef __typeof__(boost::type_of::ensure_obj(boost::scope_exit::aux::wrap( boost::scope_exit::aux::deref(& sum, (boost_se_tag_1_20)0)))) boost_se_wrapped_t_1_20; typedef boost_se_wrapped_t_1_20::type boost_se_capture_t_1_20; struct boost_se_params_t_20 { typedef boost_se_capture_t_0_20 boost_se_param_t_0_20; typedef boost_se_capture_t_1_20 boost_se_param_t_1_20; boost::scope_exit::aux::member< boost_se_param_t_0_20, boost_se_tag_0_20 > boost_se_param_0_20; boost::scope_exit::aux::member< boost_se_param_t_1_20, boost_se_tag_1_20 > boost_se_param_1_20; } boost_local_auxXparams20 = { { boost::scope_exit::aux::deref(factor, (boost_se_tag_0_20)0) } , { boost::scope_exit::aux::deref(& sum, (boost_se_tag_1_20)0) } }; boost::scope_exit::aux::declared< boost::scope_exit::aux::resolve< sizeof(boost_local_auxXargs) >::cmp1<0>::cmp2 > boost_local_auxXargs; boost_local_auxXargs.value = &boost_local_auxXparams20; 


    class boost_local_auxXfunctor20
        //: public ::boost::local::aux::abstract_function< boost_local_auxXresult_type20 ( double num ), 0 > 
    { 
        //typedef boost_local_auxXresult_type20 (boost_local_auxXfunction_type) ( double num ); 
    public:
        explicit boost_local_auxXfunctor20(void* binding_data, double& _sum, int _factor)
            : boost_local_auxXbinds(static_cast< boost_se_params_t_20*>(binding_data)) , sum_(_sum), factor_(_factor)
        { 
            /*boost_local_auxXinit_recursion();*/
//            int f = 0;
//            sum_ = boost_local_auxXbinds-> boost_se_param_1_20.value;
            factor_ = boost_local_auxXbinds-> boost_se_param_0_20.value;
            std::cout << factor_ << std::endl;
        } 
        
        void operator()( /*::boost::function_traits< boost_local_auxXfunction_type>::arg1_type*/double arg1 ) 
        { return boost_local_auxXbody( 
                //boost_local_auxXbinds-> boost_se_param_0_20.value
                10//factor_
                , 
                //boost_local_auxXbinds-> boost_se_param_1_20.value
                sum_
                , arg1 ); 
        } 
    private: 
//        typedef ::boost::local::function<boost_local_auxXfunction_type, 0 > boost_local_auxXfunctor_type; 
//        typedef ::boost::add_const< boost_se_params_t_20:: boost_se_param_t_0_20 >::type factorXboost_local_auxXtypeof_type ; 
//        typedef boost_se_params_t_20:: boost_se_param_t_1_20 & sumXboost_local_auxXtypeof_type ; 
        boost_se_params_t_20* boost_local_auxXbinds; 
        double& sum_;
        int factor_;
//        boost::scope_exit::aux::undeclared boost_local_auxXargs; 
        
        void boost_local_auxXbody( 
                /*::boost::add_const< boost_se_params_t_20:: boost_se_param_t_0_20 >::type*/const int factor 
                , /*boost_se_params_t_20:: boost_se_param_t_1_20*/double & sum , double num ) const {
        sum += factor * num;
    } 
//    public: 
//        boost_local_auxXfunctor_type add; 
//    private: 
//        void boost_local_auxXinit_recursion() { 
//            //add = *this; 
//        } 
    } boost_local_auxXfunctorXadd(boost_local_auxXargs.value, sum, factor);
   // __typeof__(boost::type_of::ensure_obj(boost_local_auxXfunctorXadd.add)) const add(boost_local_auxXfunctorXadd);
    
    for(int i = 0; i < 10000; ++i) {
        for(size_t j = 0; j < v.size(); ++j) {
            boost_local_auxXfunctorXadd(v[j]);
        }
    }

    std::cout << sum << std::endl;
    return 0;
}

