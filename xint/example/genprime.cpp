#include <iostream>
#include <boost/xint/xint.hpp>

bool callback() {
    std::cout << '.' << std::flush;
    return true; // Return false to abort a long operation
}

int main() {
    using namespace std;
    using namespace boost;

    const unsigned int bits=512;

    try {
        // Use the system's strong random number generator, via the XInt-
        // provided convenience class.
        xint::set_random_generator(new xint::strong_random_generator);

        // Generate the prime number, keeping the user informed of the
        // progress.
        cout << "Generating...";
        xint::integer p=xint::random_prime(bits, callback);

        // Success!
        cout << "\nA random " << bits << "-bit prime is: " << p << endl;
    } catch (xint::no_strong_random& e) {
        // xint::strong_random_generator will throw this if the system doesn't
        // have a strong random number generator.
        cout << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
