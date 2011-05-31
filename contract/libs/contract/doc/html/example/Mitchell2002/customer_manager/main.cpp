
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include "customer_manager.hpp"
#include <iostream>

int main() {
    std::cout << "constructor()" << std::endl;
    customer_manager mgr;
    
    basic_customer_details d("sci");
    std::cout << std::endl << "add()" << std::endl;
    mgr.add(d);

    std::cout << std::endl << "set_name()" << std::endl;
    mgr.set_name("sci", "Galileo");
    
    std::cout << std::endl << "name_for()" << std::endl;
    std::cout << mgr.name_for("sci");

    std::cout << std::endl << "count()" << std::endl;
    std::cout << mgr.count() << std::endl;
    
    std::cout << std::endl << "id_active()" << std::endl;
    std::cout << mgr.id_active("sci") << std::endl;

    std::cout << std::endl << "destructor()" << std::endl;
    return 0;
}

