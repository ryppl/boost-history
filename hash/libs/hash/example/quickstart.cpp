
//
// Copyright 2010 Scott McMurray.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/hash.hpp>

#include <iostream>
#include <string>

int main() {
    using namespace boost::hashes;
    std::string s = "Hello World!";
    typedef sha2<256> HashAlgorithm;
    HashAlgorithm::digest_type digest = compute_digest<HashAlgorithm>(s);
    std::cout << digest << "\n";
}

