
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

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

