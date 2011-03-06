
#include <boost/local/exit.hpp>
#include <iostream>

int f() {
    int error = 0; // No error to start with.

    BOOST_LOCAL_EXIT( (void) ) { // Same as D's `scope(exit)`.
        std::cout << "exit" << std::endl;
    } BOOST_LOCAL_EXIT_END

    BOOST_LOCAL_EXIT( (const bind& error) ) { // Sane as D's `scope(success)`.
        if (!error) std::cout << "success" << std::endl;
    } BOOST_LOCAL_EXIT_END
    
    BOOST_LOCAL_EXIT( (const bind& error) ) { // Same as D's `scope(failure)`.
        if (error) std::cout << "failure" << std::endl;
    } BOOST_LOCAL_EXIT_END
    
    // Cannot use `return <NUMBER>` otherwise scope exits do not know the
    // error number. Set error and exit using `return error` instead.
    error = -2; return error;
    error = -1; return error;
}

int main() {
    std::cout << "error number: " << f() << std::endl;
    return 0;
}

