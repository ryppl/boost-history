
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[gcc_access_cpp
#include <boost/local/function.hpp>
#include <iostream>

void bar(int* array, int offset, int size) {
    int BOOST_LOCAL_FUNCTION_PARAMS( (int* array) (int index)
            (const bind offset) ) {
        return array[index + offset];
    } BOOST_LOCAL_FUNCTION_NAME(access)

    for (int i = 0; i < size; ++i) {
        std::cout << access(array, i) << std::endl;
    }
}

int main() {
    int nums[3] = {1, 2, 3};
    bar(nums, 0, 3);
    return 0;
}
//]

