
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef COURIERS_HPP_
#define COURIERS_HPP_

#include <contract.hpp>
#include <string>

/** Basic package information. */
struct package {
    /** Weight in kilograms. */ 
    double weight_kg;
    /** Current location. */
    std::string location;
    /** Hour when it was accepted for delivery (from some 0-hour). */
    double accepted_hour;
    /** Hour when was delivered (from some 0-hour). */
    double delivered_hour;
    
    package(const double& the_weight_kg,
            const std::string the_location = "",
            const double& the_accepted_hour = 0.0,
            const double& the_delivered_hour = 0.0):
            weight_kg(the_weight_kg),
            location(the_location),
            accepted_hour(the_accepted_hour),
            delivered_hour(the_delivered_hour)
            {}
};

/** Basic courier for package delivery. */
class courier {

    CONTRACT_CLASS( (courier)
    (static)(invariant)(
        (MIN_INSURANCE_DOLLAR > 0.0)
    )
    (invariant)(
        (insurance_cover_dollar() >= MIN_INSURANCE_DOLLAR)
    ) )

public:
    static double MIN_INSURANCE_DOLLAR;

    // Creation //

    /** Create courier with specified insurance value. */
    CONTRACT_CONSTRUCTOR(
    (public) (explicit) (courier)(
            (const double&)(insurance_cover_dollar)
                    (default)(MIN_INSURANCE_DOLLAR)
        )
        (precondition)( (insurance_cover_dollar > 0.0) )
    (
        ;
    ) )

    /** Destroy courier. */
    CONTRACT_DESTRUCTOR(
    (public) (virtual) (courier)( (void) )
    (
        ;
    ) )

    // Queries //

    /** Return insurance cover. */
    CONTRACT_FUNCTION(
    (public) (double) (insurance_cover_dollar)( (void) ) (const)
    (
        ;
    ) )

    // Commands //

    /** Deliver package to destination. */
    CONTRACT_FUNCTION(
    (public) (virtual) (void) (deliver)(
            (package&)(the_package)
            (const std::string&)(destination)
        )
        (precondition)( (the_package.weight_kg <= 5.0) ) // Max weight.
        (postcondition)(
            (bool(3.0 >= (the_package.delivered_hour -
                    the_package.accepted_hour))) // Max delivery time.
            (the_package.location == destination) // Delivered at destination.
        )
    (
        ;
    ) )

private:
    double insurance_cover_dollar_;
};

/** Different courier for package delivery. */
class different_courier: public courier {

    CONTRACT_CLASS( (different_courier) (courier)
    (static)(invariant)(
        // Stronger invariant on insurance value (higher amount).
        (DIFFERENT_INSURANCE_DOLLAR >= courier::MIN_INSURANCE_DOLLAR)
    )
    (invariant)(
        (insurance_cover_dollar() >= DIFFERENT_INSURANCE_DOLLAR)
    ) )

public:
    static double DIFFERENT_INSURANCE_DOLLAR;

    // Creation //

    /** Create courier with specified insurance value. */
    CONTRACT_CONSTRUCTOR(
    (public) (different_courier)(
            (const double&)(insurance_cover_dollar)
                    (default)(DIFFERENT_INSURANCE_DOLLAR)
        ) (courier(insurance_cover_dollar))
        (precondition)( (insurance_cover_dollar >= 0.0) )
    ({
        // Cannot separate definition because has member (base) initialization.
    }) )

    /** Destroy courier. */
    CONTRACT_DESTRUCTOR(
    (public) (virtual) (different_courier)( (void) )
    (
        ;
    ) )

    // Commands //

    CONTRACT_FUNCTION(
    (public) (virtual) (void) (deliver)(
            (package&)(the_package)
            (const std::string&)(destination)
        )
        (precondition)(
            // Weaker precondition on weight (can weight more).
            (the_package.weight_kg <= 8.0)
        )
        (postcondition)(
            // Stronger postcondition on deliver time (faster deliver).
            (bool(2.0 >= (the_package.delivered_hour -
                    the_package.accepted_hour)))
            // Inherits "delivered at destination" postcondition.
        )
    (
        ;
    ) )
};

#endif // #include guard

