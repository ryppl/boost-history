
#include <boost/local/fuction.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    void BOOST_LOCAL_FUNCTION_PARAMS(
            types(const double&, const std::string&) num) {
        std::cout << num << std::endl;
    } BOOST_LOCAL_FUNCTION_NAME(l)

    std::vector<double> v(3);
    v[0] = -1.0; v[1] = -2.0; v[2] = -3.0;
    std::for_each(v.begin(), v.end(), l);

    std::vector<std::string> s(3);
    s[0] = "aa"; s[1] = "bb"; s[2] = "cc";
    std::for_each(s.begin(), s.end(), l);

    return 0;
}

