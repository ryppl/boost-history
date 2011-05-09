
#include <boost/local/function.hpp>
#include <boost/function.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

template<typename F, typename G>
struct overloader {
    overloader(F f, G g): f_(f), g_(g) {}
    template<typename A0>
    void operator()(A0 a0) const { return f_(a0); }
    template<typename A0>
    void operator()(A0 a0) const { return g_(a0); }
private:
    F f_;
    G g_;
};

template<typename F, typename G>
overloader<F, G> overload(F f, G g) { return overloader<F, G>(f, g); }

int main() {
    char sep = '\n';
    void BOOST_LOCAL_FUNCTION_PARAMS(const double& num, const bind& sep) {
        std::cout << num << sep;
    } BOOST_LOCAL_FUNCTION_NAME(print_d)
    void BOOST_LOCAL_FUNCTION_PARAMS(const std::string& num, const bind& sep) {
        std::cout << num << sep;
    } BOOST_LOCAL_FUNCTION_NAME(print_s)
//    overloader<void (const double&), void (const std::string&)> print_ds(
//            print_d, print_s);
    __typeof__(overload(print_d, print_s)) print = overload(print_d, print_s);


    std::vector<double> d(2);
    d[0] = 1.2; d[1] = 3.4;
    std::for_each(d.begin(), d.end(), print);

    std::vector<std::string> s(3);
    s[0] = "ab"; s[1] = "cd"; s[2] = "ef";
    std::for_each(s.begin(), s.end(), print);

    return 0;
}

