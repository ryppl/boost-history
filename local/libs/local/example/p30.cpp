
#include <boost/local/function.hpp>
#include <boost/function.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

template<typename F0, typename F1, typename F2 = void, typename F3 = void>
struct overload {
};

template<typename F0R, typename F0A0, typename F1R, typename F1A0>
struct overload<F0R (F0A0), F1R (F1A0)> {
    overload(boost::function<F0R (F0A0)> f0, boost::function<F1R (F1A0)> f1):
            f0_(f0), f1_(f1) {}
    F0R operator()(F0A0 a0) const { return f0_(a0); }
    F1R operator()(F1A0 a0) const { return f1_(a0); }
private:
    boost::function<F0R (F0A0)> f0_;
    boost::function<F1R (F1A0)> f1_;
};

// More specializations to overload also with F2, F3, etc.

int main() {
    char sep = '\n';

    void BOOST_LOCAL_FUNCTION_PARAMS(const double& num, const bind& sep) {
        std::cout << num << sep;
    } BOOST_LOCAL_FUNCTION_NAME(print_d)

    void BOOST_LOCAL_FUNCTION_PARAMS(const std::string& num, const bind& sep) {
        std::cout << num << sep;
    } BOOST_LOCAL_FUNCTION_NAME(print_s)

    overload<void (const double&), void (const std::string&)> print(
            print_d, print_s);

    std::vector<double> d(2);
    d[0] = 1.2; d[1] = 3.4;
    std::for_each(d.begin(), d.end(), print);

    std::vector<std::string> s(3);
    s[0] = "ab"; s[1] = "cd"; s[2] = "ef";
    std::for_each(s.begin(), s.end(), print);

    return 0;
}

