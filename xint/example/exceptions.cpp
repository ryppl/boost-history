
#include <boost/xint/xint.hpp>
#include <ostream>

int main() {
    using namespace std;
    using namespace boost;
    using namespace xint::blockable;

    integer n;

    // It *is* possible to block exceptions multiple times, so this assert
    // checks to see whether they're presently allowed. It is not needed,
    // it's only here for demonstration purposes.
    assert(xint::exceptions_allowed() == true);

    // We'll wrap this in a try block for demonstration purposes, just to
    // prove that it doesn't throw an exception. You normally don't need a
    // try block if you're only expecting blockable exceptions.
    try {
        // Create the "blocker" token. Until the token is destroyed, exceptions
        // will be blocked, except when explicitly re-enabled.
        xint::token blocker=xint::block_exceptions();

        // This assert is only here for demonstration purposes.
        assert(xint::exceptions_allowed() == false);

        // Call the function(s) that should be exception-blocked. The example
        // call tries to interpret an empty string as an integer, which would
        // normally throw an xint::invalid_digit exception.
        n = integer("");

        // n should be Not-a-Number, because there was an error but exceptions
        // were blocked. This example code is fairly useless, and is only here
        // for demonstration purposes, but you should always test for Not-a-Number
        // when blocking exceptions.
        if (n.is_nan()) {
            cout << "n is Not-a-Number, do some intelligent error handling here."
                << endl;
        } else {
            // This example code should never get to this block, but you would
            // usually put the normal program flow code here.
            cout << "Error, n should be Not-a-Number and it isn't!" << endl;
        }
    } catch (std::exception& e) {
        // This example code should never get to this block, because exceptions
        // were blocked.
        cout << "Error, threw an exception when exceptions were blocked!\n" <<
            e.what() << endl;
    }

    // Now exceptions are re-enabled, because the blocking behavior ended when
    // the "blocker" token was destroyed at the end of the try block. Note that
    // this assert is not needed, it's only here for demonstration purposes.
    assert(xint::exceptions_allowed() == true);
}
