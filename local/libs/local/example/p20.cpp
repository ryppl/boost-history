
#include <string>

std::string A(size_t arity, size_t type) {
    return std::string("A") + 
}

R operator()(A1T1);
R operator()(A1T2);
R operator()(A1T3);

R operator()(A1T1, A2T1);
R operator()(A1T1, A2T2);
R operator()(A1T1, A2T3);
//
R operator()(A1T2, A2T1);
R operator()(A1T2, A2T2);
R operator()(A1T2, A2T3);
//
R operator()(A1T3, A2T1);
R operator()(A1T3, A2T2);
R operator()(A1T3, A2T3);

template<R, typename A0T0, typename A0T1, typename A1T0, typename A1T1, typename A2T0> {
} class function<
        R (types<A0T0, A0T1>, types<A1T0, A1T1>, A2T0), 1
> {
    typedef void* object_ptr;
    typedef R (*call_ptr)(object_ptr
            , typename ::boost::call_tratis<A0T0>::param_type
            , typename ::boost::call_tratis<A1T0>::param_type
            , typename ::boost::call_tratis<A2T0>::param_type);
    typedef R (*call_ptr)(object_ptr
            , typename ::boost::call_tratis<A0T1>::param_type
            , typename ::boost::call_tratis<A1T0>::param_type
            , typename ::boost::call_tratis<A2T0>::param_type);
};


int main() {
    size_t arity = 5;
    size_t types = 3;
    size_t defaults = 0;

    for (a = 0; a < arity; ++a) {
        for (t = 0; t < types; ++t) {
            for (d = 0; d < defaults; ++d) {
                std::cout <<
"R operator()(A)

            }
        }
    }

    return 0;
}

