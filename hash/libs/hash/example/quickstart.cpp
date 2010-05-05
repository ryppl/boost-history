
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
    std::string s = "Hello World!";
    typedef boost::hash::sha2<256> HashPolicy;
    HashPolicy::digest_type digest = boost::hash::compute_digest<HashPolicy>(s);
    std::cout << digest << "\n";
}

