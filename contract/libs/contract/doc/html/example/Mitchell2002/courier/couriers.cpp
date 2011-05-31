
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include "couriers.hpp"
#include <contract.hpp>

// Courier //

double courier::MIN_INSURANCE_DOLLAR = 10.0e+6; 

CONTRACT_CONSTRUCTOR_BODY(courier, courier)(
        const double& insurance_cover_dollar) {
    insurance_cover_dollar_ = insurance_cover_dollar;
}

CONTRACT_DESTRUCTOR_BODY(courier, courier)(void) {}

double courier::CONTRACT_MEMBER_BODY(insurance_cover_dollar)(void) const {
    return insurance_cover_dollar_;
}

void courier::CONTRACT_MEMBER_BODY(deliver)(package& the_package,
        const std::string& destination) {
    the_package.location = destination;
    // Delivery takes 2.5 hours.
    the_package.delivered_hour = the_package.accepted_hour + 2.5;
}

// Different Courier //

double different_courier::DIFFERENT_INSURANCE_DOLLAR = 20.0e+6; 

CONTRACT_DESTRUCTOR_BODY(different_courier, different_courier)(void) {}

void different_courier::CONTRACT_MEMBER_BODY(deliver)(package& the_package,
        const std::string& destination) {
    the_package.location = destination;
    // Delivery takes only 0.5 hours.
    the_package.delivered_hour = the_package.accepted_hour + 0.5;
}

