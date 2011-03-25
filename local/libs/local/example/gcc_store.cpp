
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[gcc_store_cpp
#include <boost/local/function.hpp>

void intermediate(boost::local::function<void (int, int)> store_func,
        int size) {
    store_func(size - 1, -1);
}

void hack(int* array, int size) {
    void BOOST_LOCAL_FUNCTION_PARAMS( (int index) (int value) (bind array) ) {
        array[index] = value;
    } BOOST_LOCAL_FUNCTION_NAME(store)

    intermediate(store, size);
}

int main() {
    return 0;
}
//]

