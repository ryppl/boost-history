
#include <boost/local/function.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    double sum = 0.0;
    int factor = 10;

    typedef void (*tag)(int factor); 
    typedef BOOST_TYPEOF(boost::scope_exit::aux::wrap(
            boost::scope_exit::aux::deref(factor, (tag)0))) wrapped_t;
    typedef wrapped_t::type capture_t;
    struct params_t {
        typedef capture_t param_t;
        boost::scope_exit::aux::member<param_t, tag> param;
    } params = {
          { boost::scope_exit::aux::deref(factor, (tag)0) } 
    }; 


    std::vector<double> v(1000000);
    std::cout << params.param.value << std::endl;


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

    std::fill(v.begin(), v.end(), 10);
    for (size_t i = 0; i < 10000; ++i) {
        for (size_t j = 0; j < v.size(); ++j) {
            //add(v[j]);
            add(10);
        }
    }

    std::cout << sum << std::endl;
    return 0;
}

