// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.

// Disclaimer: Not a Boost library.

#include <string>
#include <boost/interfaces.hpp>
#include "./detail/template_hierarchies.hpp"

int main()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    const ctemplate_first<1>           first1;
    itemplate_first<int, const char*>  first = first1;  
}
