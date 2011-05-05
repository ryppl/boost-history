
#include <boost/spirit/include/phoenix.hpp>
#include <iostream>
#include <vector>

int main() {
    using boost::phoenix::arg_names::_1;

    boost::phoenix::actor<boost::phoenix::composite<boost::phoenix::shift_left_eval, boost::fusion::vector<boost::phoenix::composite<boost::phoenix::shift_left_eval, boost::fusion::vector<boost::phoenix::reference<std::basic_ostream<char, std::char_traits<char> > >, boost::phoenix::argument<0>, boost::fusion::void_, boost::fusion::void_, boost::fusion::void_, boost::fusion::void_, boost::fusion::void_, boost::fusion::void_, boost::fusion::void_, boost::fusion::void_> >, boost::phoenix::value<const char*>, boost::fusion::void_, boost::fusion::void_, boost::fusion::void_, boost::fusion::void_, boost::fusion::void_, boost::fusion::void_, boost::fusion::void_, boost::fusion::void_> > >
        f = std::cout << _1 << "\n";

    std::vector<double> v(3);
    v[0] = -1.0; v[1] = -2.0; v[2] = -3.0;
    std::for_each(v.begin(), v.end(), f);

    std::vector<std::string> s(3);
    s[0] = "aa"; s[1] = "bb"; s[2] = "cc";
    std::for_each(s.begin(), s.end(), f);

    return 0;
}

