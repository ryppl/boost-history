
#include <boost/hash/digest.hpp>
#include <boost/hash/digest_io.hpp>

#include <iostream>
#include <sstream>

#include <cassert>

int main() {
    using boost::hashes::digest;

    {
    std::stringstream ss;
    digest<160> d;
    ss << d;
    std::cout << ss.str() << "\n";
    // This test fails for the author in g++ 4.3.4
    // because of a bug in value-initialization
    assert(ss.str() == std::string(160/4, '0'));
    }

    {
    std::stringstream ss("0123456789abcdef0123456789ABCDEF");
    digest<32*4> d;
    ss >> d;
    assert(ss);
    std::cout << d << "\n";
    assert(d == "0123456789abcdef0123456789abcdef");
    }

    {
    std::stringstream ss("0123456789abcdef0123456789ABCDEF");
    digest<32*4+8> d;
    ss >> d;
    assert(!ss);
    std::cout << d << "\n";
    assert(d == "0123456789abcdef0123456789abcdef00");
    }

    {
    using boost::hashes::truncate;
    using boost::hashes::resize;
    std::stringstream ss("0123456789abcdeffedcba9876543210");
    digest<32*4> d;
    ss >> d;
    std::cout << truncate<16*4>(d) << "\n";
    assert(truncate<16*4>(d) == "0123456789abcdef");
    std::cout << resize<16*3*4>(d) << "\n";
    assert(resize<16*3*4>(d) == "0123456789abcdeffedcba98765432100000000000000000");
    }

}
