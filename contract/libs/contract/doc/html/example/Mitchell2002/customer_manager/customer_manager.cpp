
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include "customer_manager.hpp"
#include <contract.hpp>

CONTRACT_CONSTRUCTOR_BODY(customer_manager, customer_manager)(void) {}

CONTRACT_DESTRUCTOR_BODY(customer_manager, customer_manager)(void) {}

int customer_manager::CONTRACT_MEMBER_BODY(count)(void) const {
    return customers_.size();
}

bool customer_manager::CONTRACT_MEMBER_BODY(id_active)(
        const basic_customer_details::identifier& id) const {
    return customers_.find(id) != customers_.end();
}

const std::string& customer_manager::CONTRACT_MEMBER_BODY(name_for)(
        const basic_customer_details::identifier& id) const {
    // Find != end() because of id_active() precondition.
    return customers_.find(id)->second.name;
}

void customer_manager::CONTRACT_MEMBER_BODY(add)(
        const basic_customer_details& details) {
    customers_.insert(std::pair<basic_customer_details::identifier,
            customer>(details.id, customer(details)));
}

void customer_manager::CONTRACT_MEMBER_BODY(set_name)(
        const basic_customer_details::identifier& id,
        const std::string& name) {
    // Find != end() because of id_active() precondition.
    customers_.find(id)->second.name = name;
}

