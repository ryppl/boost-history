
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#include <boost/config.hpp>
#if defined(BOOST_NO_VARIADIC_MACROS) || defined(BOOST_LOCAL_CONFIG_COMPLIANT)
#include <iostream>
int main() {
    std::cerr << "Error: This program requires variadic macros" << std::endl;
    return 0;
}
#else

//[ print_map_va_cpp
#include <boost/local/function.hpp>
#include <boost/utility/identity.hpp>
#include <map>
#include <string>
#include <iostream>

template<typename K, typename T> struct key_size { static const size_t value; };
template<typename K, typename T> const size_t key_size<K, T>::value = sizeof(K);

std::string cat(const std::string& a, const std::string& b) { return a + b; }

typedef int const& sign_t;

int main() {
    void BOOST_LOCAL_FUNCTION_PARAMS(
            // Identity macros handle commas.
            BOOST_IDENTITY_TYPE((const std::map<std::string, size_t>))& m,
            // Also, identity macros handle leading symbols.
            BOOST_IDENTITY_TYPE((::sign_t)) sign, default -1,
            const size_t& factor, default // Or use `BOOST_IDENTITY_VALE`.
                    (key_size<std::string, size_t>::value),
            const std::string& separator, default cat(":", " ")) {
        for (std::map<std::string, size_t>::const_iterator i = m.begin();
                i != m.end(); ++i) {
            std::cout << i->first << separator <<
                    sign * int(i->second) * int(factor) << std::endl;
        }
    } BOOST_LOCAL_FUNCTION_NAME(print)

    std::map<std::string, size_t> sizes;
    sizes["a"] = 1; sizes["ab"] = 2; sizes["abc"] = 3;
    print(sizes);

    return 0;
}
//]

#endif

