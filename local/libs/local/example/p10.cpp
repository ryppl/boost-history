
#include <iostream>
#include <vector>
#include <algorithm>

template<typename T>
struct func {
    func(void *obj, void (*call)(void*, T)): obj_(obj), call_(call) {}

};

int main() {
    struct fctor {
        void operator()(const double& num) const { std::cout << num << std::endl; }
        void operator()(const std::string& num) const { std::cout << num << std::endl; }
    };

    void BOOST_LOCAL_FUNCTION_PARAMS(
            types(const double&, const std::string&) num) {
        std::cout << num << std::endl;
    }

    std::vector<double> v(3);
    v[0] = -1.0; v[1] = -2.0; v[2] = -3.0;
    std::for_each(v.begin(), v.end(), f);

    std::vector<std::string> s(3);
    s[0] = "aa"; s[1] = "bb"; s[2] = "cc";
    std::for_each(s.begin(), s.end(), f);

    return 0;
}

