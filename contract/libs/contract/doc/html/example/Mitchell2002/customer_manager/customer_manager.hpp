
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef CUSTOMER_MANAGE_HPP_
#define CUSTOMER_MANAGE_HPP_

#include <contract.hpp>
#include <string>
#include <map>

/** Basic customer information. */
class basic_customer_details {
    friend class customer_manager;

public:
    /** Identifier type. */
    typedef std::string identifier;

    /** Construct basic customer information. */
    explicit basic_customer_details(const identifier& the_id):
            id(the_id), name(), address(), birthday() {}
    
protected:
    /** Customer identifier. */
    identifier id;
    /** Customer name. */
    std::string name;
    /** Customer address. */
    std::string address;
    /** Customer date of birth. */
    std::string birthday;
};

/** Manage customers. */
class customer_manager {

    CONTRACT_CLASS( (customer_manager)
    (invariant)(
        (count() >= 0)
    ) )

public:
    // Construction //

    CONTRACT_CONSTRUCTOR(
    (public) (customer_manager)( (void) )
        // Cannot use member initializers here because separating definition.
    (
        ;
    ) )

    CONTRACT_DESTRUCTOR(
    (public) (virtual) (customer_manager)( (void) )
    (
        ;
    ) )

    // Basic Queries //

    /** Number of customers. */
    CONTRACT_FUNCTION(
    (public) (int) (count)( (void) ) (const)
        (postcondition)(result)( (result >= 0) )
    (
        ;
    ) )

    /** There is a customer with given identifier. */
    CONTRACT_FUNCTION(
    (public) (bool) (id_active)(
            (const basic_customer_details::identifier&)(id)
        ) (const)
    (
        ;
    ) )

    // Derived Queries //

    /** Name of customer with given identifier. */
    CONTRACT_FUNCTION(
    (public) (const std::string&) (name_for)(
            (const basic_customer_details::identifier&)(id)
        ) (const)
        (precondition)( (id_active(id)) )
    (
        ;
    ) )
    
    // Commands //

    /** Add given customer. */
    CONTRACT_FUNCTION(
    (public) (void) (add)( (const basic_customer_details&)(details) ) (copyable)
        (precondition)( (id_active(details.id) == false) )
        (postcondition)(
            (count() == (CONTRACT_OLDOF(this)->count() + 1))
            (id_active(details.id))
        )
    (
        ;
    ) )

    /** Set name of customer with given identifier. */
    CONTRACT_FUNCTION(
    (public) (void) (set_name)(
            (const basic_customer_details::identifier&)(id)
            (const std::string&)(name)
        )
        (precondition)( (id_active(id)) )
        (postcondition)( (name_for(id) == name) )
    (
        ;
    ) )

private:
    /** Customer agent. */
    class agent {};

    /** Basic customer. */
    class customer: public basic_customer_details {
    public:
        /** Customer agent/ */
        agent managing_agent;
        /** Customer last contacted. */
        std::string last_contact;
        
        explicit customer(const basic_customer_details& details):
                basic_customer_details(details), managing_agent(),
                last_contact() {}
    };

    std::map<basic_customer_details::identifier, customer> customers_;
};

#endif // #include guard

