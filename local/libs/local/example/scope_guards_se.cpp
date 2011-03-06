
#include <boost/scope_exit.hpp>
#include <iostream>
#include <stdexcept>

void f() {
    bool error = false;

    BOOST_SCOPE_EXIT( (&error) ) {
        std::cout << "returning" << std::endl; // Not executed on throw.
    } BOOST_SCOPE_EXIT_END

    throw std::runtime_error("some error");
}

int main() {
    f();
    return 0;
}

