
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

//[ add_x_y_default_cpp
#include <boost/local/function.hpp>
#include <iostream>

int main() {
    int BOOST_LOCAL_FUNCTION_PARAMS( (int x) (int y)(default 2) ) {
        return x + y;
    } BOOST_LOCAL_FUNCTION_NAME(add)

    add(1);
    return 0;
}
//]

