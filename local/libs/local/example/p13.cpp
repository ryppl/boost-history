
#include <iostream>
#include <vector>
#include <algorithm>

template<typename T1 = void, typename T2 = void, typename T3 = void>
struct types {};

template<typename F>
struct func {};

template<typename R, typename A1>
struct func<R (A1)> {
    typedef R (*call_ptr)(void*, A1);
    func(void* obj, call_ptr call): obj_(obj), call_(call) {}
    R operator()(A1 a1) { return call_(obj_, a1); }
private:
    void* obj_;
    call_ptr call_;
};

template<typename R, typename A1T1, typename A1T2>
struct func<R (types<A1T1, A1T2>)> {
    typedef R (*call_a1t1_ptr)(void*, A1T1);
    typedef R (*call_a1t2_ptr)(void*, A1T2);
    func(void* obj, call_a1t1_ptr call_a1t1, call_a1t2_ptr call_a1t2):
            obj_(obj), call_a1t1_(call_a1t1), call_a1t2_(call_a1t2) {}
    R operator()(A1T1 a1t1) { return call_a1t1_(obj_, a1t1); }
    R operator()(A1T2 a1t2) { return call_a1t2_(obj_, a1t2); }
private:
    void* obj_;
    call_a1t1_ptr call_a1t1_;
    call_a1t2_ptr call_a1t2_;
};

int main() {
//  void BOOST_LOCAL_FUNCTION(types(const double&, const std::string&) num,
//          const bind& sep, {
//      std::cout << num << std::endl;
//   }) BOOST_LOCAL_FUNCTION_NAME(l)

    char sep = '\n';

    struct local {
        local(const char& sep): sep_(sep) {}
        void operator()(double num) { body(sep_, num); }
        static void call(void* obj, double num)
            { (*static_cast<local*>(obj))(num); }

        void operator()(std::string num) { body(sep_, num); }
        static void call(void* obj, std::string num)
            { (*static_cast<local*>(obj))(num); }
    private:
        const char& sep_;
        void body(const char& sep, double num)
            { std::cout << num << std::endl; }
        void body(const char& sep, std::string num)
            { std::cout << num << std::endl; }
    } local_l(sep);
    func<void (types<double, std::string>)> l(&local_l,
            local_l.call, local_l.call);

    std::vector<double> v(3);
    v[0] = -1.0; v[1] = -2.0; v[2] = -3.0;
    std::for_each(v.begin(), v.end(), l);
    l(0);

    std::vector<std::string> s(3);
    s[0] = "aa"; s[1] = "bb"; s[2] = "cc";
    std::for_each(s.begin(), s.end(), l);
    l("zz");

    return 0;
}

