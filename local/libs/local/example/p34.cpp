
#include <boost/local/function.hpp>
#include <boost/function.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

namespace boost { namespace local { namespace function {

template<typename F0, typename F1, typename F2 = void, typename F3 = void>
struct overloaded {};

// Specialization for 2 functions.

template<typename F0, typename F1>
struct overloaded<F0, F1>: F0, F1 {
    overloaded(F0 const& f0, F1 const& f1):
            F0(f0), F1(f1) {}
    using F0::operator(); using F1::operator();
};

template<typename F0, typename F1>
overloaded<F0, F1> overload(F0 const& f0, F1 const& f1) {
    return overloaded< F0, F1 >(f0, f1);
}

template<typename F0, typename F1>
overloaded< boost::function<F0>, boost::function<F1> >
overload(
      boost::local::aux::function<F0> const& f0
    , boost::local::aux::function<F1> const& f1
) {
    return overloaded< boost::function<F0>, boost::function<F1> >(f0, f1);
}

}}} // namespace boost::local::function

// More specializations to overload also with F2, F3, etc.

int main() {
    char sep = '\n';

    void BOOST_LOCAL_FUNCTION_PARAMS(const double& item, int x, default 1, const bind& sep) {
        std::cout << item << sep;
    } BOOST_LOCAL_FUNCTION_NAME(print_double)

    void BOOST_LOCAL_FUNCTION_PARAMS(const std::string& item, const bind& sep) {
        std::cout << item << sep;
    } BOOST_LOCAL_FUNCTION_NAME(print_string)

    boost::local::function::overloaded<
          boost::function< void (const double&) >
        , boost::function< void (const std::string&) >
    > print = boost::local::function::overload(
          boost::function< void (const double&) >(print_double)
        , boost::function< void (const std::string&) >(print_string)
    );

    std::vector<double> d(2);
    d[0] = 1.2; d[1] = 3.4;
    std::for_each(d.begin(), d.end(), print);

    std::vector<std::string> s(3);
    s[0] = "ab"; s[1] = "cd"; s[2] = "ef";
    std::for_each(s.begin(), s.end(), print);

    return 0;
}

