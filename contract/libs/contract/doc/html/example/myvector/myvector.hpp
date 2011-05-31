
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef MYVECTOR_HPP_
#define MYVECTOR_HPP_

//[ myvector_cpp

#include "pushable.hpp"         // Base classes for subcontracting.
#include "boundable.hpp"
#include "basic_begin.hpp"
#include <contract.hpp>         // This library.
#include <boost/utility.hpp>    // For `boost::prior()`.
#include <vector>               // STL vector.
    
// Wrapper that adds simple (not complete) contracts to C++ STL illustrating
// most library usages. For simplicity, assume T is comparable and copyable.
template<typename T>
class myvector: public pushable<T>,
        public boundable<typename std::vector<T>::const_iterator>,
        private basic_begin<typename std::vector<T>::const_iterator> {
public:
    typedef typename std::vector<T>::size_type size_type;
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;
    typedef typename std::vector<T>::const_reference const_reference;

private:
    // Class invariants (checked by any function with a contract).
    CONTRACT_CLASS( (myvector) // This class.
            // Subcontract from base classes (support for multiple inheritance).
            (pushable<T>) (boundable<const_iterator>) (basic_begin<const_iterator>)
    (static) (invariant)( // Static invariants cannot access object (optional).
        (true) // Dummy assertion in this example.
    )
    (invariant)( // Non-static invariants can access object (optional).
        (empty() == (size() == 0))
        // More invariants here...
    ) )

public:
    // Contract for constructor.
    CONTRACT_CONSTRUCTOR( 
    (public) (explicit) (myvector)( (size_type)(count) )
        (vector_(count)) // Member initializers.
        // Never object `this` in constructor preconditions.
        (postcondition)(
            // Never `CONTRACT_OLDOF(this)` in constructor postconditions.
            (size() == count)
        )
    ({
        // Do nothing in this example.
    }) )

    // Contractor for overloaded member (resolved by argumnet name).
    CONTRACT_CONSTRUCTOR( 
    (public) (myvector)( (const myvector&)(right) )
        (postcondition)(
            (vector_ == right.vector_)
        )
    (
        ; // Deferres body definition.
        // Deferred body definitions show useful compiler-error line numbers.
    ) )

    // Contract for destructor.
    CONTRACT_DESTRUCTOR( 
    // No `~` in destructor name.
    // Use `(void)` for empty function paramters.
    (public) (virtual) (myvector)( (void) )
        // No preconditions allowed (no arguments).
        // No postconditions allowed (no object after destructor).
    (
        ;
    ) )

    // Contract for member function.
    // Copy old values for object `this` and argument `where`.
    CONTRACT_FUNCTION(
    (public) (void) (insert)(
            // Multiple function parameters.
            (copyable)(iterator)(where)
            (size_type)(count)
            (const T&)(element) 
        ) (copyable)
        (precondition)( // Function preconditions (optional).
            (max_size() >= (size() + count))
            // More preconditions here...
        )
        (postcondition)( // Function postconditions (optional).
            // Old values via `CONTRACT_OLDOF()` for types qualified copyable.
            // Assertion with if-guard (i.e., "select-assertion").
            (if (capacity() == CONTRACT_OLDOF(this)->capacity()) (
                (all_equals(
                        boost::prior(CONTRACT_OLDOF(where)),
                        boost::prior(CONTRACT_OLDOF(where)) + count,
                        element))
            ) ) // Optional `(else ...)` allowed here.
            // More postconditions here...
        )
    ({ // Original function implementation.
        vector_.insert(where, count, element);
    }) )

    // Contract for constant member.
    // Non-void result type.
    CONTRACT_FUNCTION(
    (public) (const_iterator) (begin)( (void) ) (const) // Constant.
        (postcondition)(result)( // Return value named `result`.
            (if (empty()) (
                (result == end())
            ) )
        )
    ({
        return vector_.begin();
    }) )
    
    // Contract for overloaded member (resolved because not const).
    CONTRACT_FUNCTION( 
    (public) (iterator) (begin)( (void) )
        (postcondition)(result)(
            (if (empty()) (
                (result == end())
            ) )
        )
    (
        ;
    ) )
    
    // Contract for operator.
    CONTRACT_FUNCTION(
    // Must spell operator name also in words (e.g., `at`).
    (public) (const_reference) (operator([], at))( (size_type)(index) ) (const)
        (precondition)(
            (index < size())
        )
    (
        ;
    ) )

    // Main function example used in documentation.
    CONTRACT_FUNCTION( 
    (public) (void) (push_back)( (const T&)(element) ) (copyable)
        (precondition)(
            (size() < max_size())
        )
        (postcondition)(
            (size() == (CONTRACT_OLDOF(this)->size() + 1))
        )
    ({
        vector_.push_back(element);
    }) )
    
    // Contract for template plus static member function.
    CONTRACT_FUNCTION( 
    (public) (template)( (class)(Iter) ) // Function template.
        (requires)( // Concepts.
            (boost::InputIterator<Iter>)
            (boost::Comparable<T>) // For simplicity, assume T can be compared.
        )
    (static) (bool) (all_equals)( // Static member.
            (Iter)(first)
            (Iter)(last)
            (const T&)(element)
        )
        (precondition)(
            (first < last)
        )
    ({
        for (Iter i = first; i < last; ++i) {
            if (*i != element) return false;
        }
        return true;
    }) )
    
    // Similarly, complete contracts sketched here and add contracts
    // for all other functions (see [Crowl2006] vector example).
    bool empty(void) const { return vector_.empty(); }
    size_type size(void) const { return vector_.size(); }
    size_type max_size(void) const { return vector_.max_size(); }
    size_type capacity(void) const { return vector_.capacity(); }
    iterator end(void) { return vector_.end(); }
    const_iterator end(void) const { return vector_.end(); }
    const_reference back(void) const { return vector_.back(); }

private:
    std::vector<T> vector_;
};

// Deferred constructor body definition.
template<typename T>
CONTRACT_CONSTRUCTOR_BODY(myvector<T>, myvector)(const myvector& right) {
    vector_ = right.vector_;
}

// Deferred destructor body definition.
template<typename T>
CONTRACT_DESTRUCTOR_BODY(myvector<T>, myvector)(void) {
    // Do nothing in this case.
}

// Deferred member function definition.
template<typename T>
typename myvector<T>::iterator myvector<T>::CONTRACT_BODY(begin)(void) {
    return vector_.begin();
}

// Deferred member operator definition.
template<typename T>
typename myvector<T>::const_reference myvector<T>::CONTRACT_BODY(operator([], at))(
        size_type index) const {
    return vector_[index];
}

// Contract for non-member function.
CONTRACT_FUNCTION( 
(double) (abs_total)( (const myvector<double>&)(vector) )
    (postcondition)(total)( // Return value named `total` (instead of `result`).
        (total >= 0.0)
    )
({
    double total = 0.0;
    // Block invariants can appear anywhere in code block.
    CONTRACT_ASSERT_BLOCK_INVARIANT( total == 0.0 );

    { // Variant initialized locally to its loop.
        CONTRACT_INIT_LOOP_VARIANT;
        for (size_t i = 0; i < vector.size(); ++i) {
            // Block invariants used to assert loop invariants.
            CONTRACT_ASSERT_BLOCK_INVARIANT( i < vector.size() );
            // Loop variant (can only appear in loops).
            CONTRACT_ASSERT_LOOP_VARIANT( vector.size() - i );

            total += vector[i];
        }
    }
    return total < 0.0 ? -total : total;
}) )

//]

#endif // #include guard

