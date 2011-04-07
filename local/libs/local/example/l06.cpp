
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

    typedef void (*boost_se_tag_0_20)(int factor); 
    typedef BOOST_TYPEOF(boost::scope_exit::aux::wrap(
            boost::scope_exit::aux::deref(factor, (boost_se_tag_0_20)0)))
            boost_se_wrapped_t_0_20;
    typedef boost_se_wrapped_t_0_20::type boost_se_capture_t_0_20;
    
    struct boost_se_params_t_20 {
        typedef boost_se_capture_t_0_20 boost_se_param_t_0_20;

        boost::scope_exit::aux::member<boost_se_param_t_0_20,
                boost_se_tag_0_20> boost_se_param_0_20;
    } boost_local_auxXparams20 = {
          { boost::scope_exit::aux::deref(factor, (boost_se_tag_0_20)0) } 
    }; 
    boost::scope_exit::aux::declared<boost::scope_exit::aux::resolve<
            sizeof(boost_local_auxXargs)>::cmp1<0>::cmp2> boost_local_auxXargs;
    boost_local_auxXargs.value = &boost_local_auxXparams20;

    std::cout << boost_local_auxXparams20.boost_se_param_0_20.value << std::endl;

    class local_add { 
    public:
        explicit local_add(double& _sum, int _factor):
                sum_(_sum), factor_(_factor) {
        } 
        void operator()(double num) {
            return body(factor_, sum_, num);
        } 
    private: 
        double& sum_;
        int factor_;
        
        void body(const int factor, double& sum, double num)
                const {
            sum += factor * num;
        }
    } add(sum, factor);
    
    for (size_t i = 0; i < 10000; ++i) {
        for (size_t j = 0; j < v.size(); ++j) {
            add(v[j]);
        }
    }

    std::cout << sum << std::endl;
    return 0;
}

