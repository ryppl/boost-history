// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <boost/interfaces.hpp>
#include <boost/interfaces/delegation.hpp>
#include "./detail/hierarchies.hpp"

namespace boost { namespace interfaces { namespace test {

class delegator : public delegating<ifirst> { };

} } } // End namespaces.

int main()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    cfirst<1> first;
    delegator d;
    set_delegate<ifirst>(d, &first);
}
