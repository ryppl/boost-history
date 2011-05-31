
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef NAMES_HPP_
#define NAMES_HPP_

#include <contract.hpp>
#include <string>
#include <list>

/** List of names. */
class name_list {
    
    CONTRACT_CLASS( (name_list) )

public:
    // Creation //

    /** Create object. */
    CONTRACT_CONSTRUCTOR(
    (public) (name_list)( (void) )
    (
        ;
    ) )

    /** Destroy object. */
    CONTRACT_DESTRUCTOR(
    (public) (virtual) (name_list)( (void) )
    (
        ;
    ) )
    
    // Queries //

    /** If specified names is in list. */
    CONTRACT_FUNCTION(
    (public) (bool) (has)( (const std::string&)(name) ) (const)
    (
        ;
    ) )

    /** Number of names in list. */
    CONTRACT_FUNCTION(
    (public) (unsigned int) (count)( (void) ) (const)
    (
        ;
    ) )

    // Commands //

    /** Add name to list. */
    CONTRACT_FUNCTION(
    (public) (virtual) (void) (put)( (const std::string&)(name) ) (copyable)
        (precondition)( (has(name) == false) )
        (postcondition)(
            // If-guard allows subcontracts to relax postconditions.
            (if (!CONTRACT_OLDOF(this)->has(name)) (
                (has(name))
                (count() == (CONTRACT_OLDOF(this)->count() + 1))
            ) )
        )
    (
        ;
    ) )

private:
    std::list<std::string> names_;
};

/** List of names that allows for duplicates. */
class relaxed_name_list: public name_list {

    // Subcontract from `name_list`.
    CONTRACT_CLASS( (relaxed_name_list) (name_list) )

public:
    // Creation //

    CONTRACT_CONSTRUCTOR(
    (public) (relaxed_name_list)( (void) )
    (
        ;
    ) )

    CONTRACT_DESTRUCTOR(
    (public) (virtual) (relaxed_name_list)( (void) )
    (
        ;
    ) )

    // Commands //

    CONTRACT_FUNCTION(
    (public) (void) (put)( (const std::string&)(name) ) (copyable)
        (precondition)(
            // Relax inherited precondition `has(name) == false`.
            (has(name))
        )
        (postcondition)(
            // Inherited postcondition not checked because of its if-guard.
            (if (CONTRACT_OLDOF(this)->has(name)) (
                (count() == CONTRACT_OLDOF(this)->count())
            ) )
        )
    (
        ;
    ) )
};

#endif // #include guard

