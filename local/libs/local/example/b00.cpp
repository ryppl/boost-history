
/*

const bind& x
--> bind& x
--> & x

const bind_type int& x
--> bind_type int& x
--> int& x --> int

const bind<int>& x
--> bind<int>& x
--> <int>& x
--> select<t11>::type<int>& x

*/

struct bind {
    template<typename T = void>
    struct type {};
};


int main() {
    bind::type<int> b;
    return 0;
}

