
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

//[ push_back_cpp

#include "pushable.hpp" // Some base class (for subcontracting).
#include <contract.hpp> // This library.
#include <vector>       // C++ STL vector.

// Wrapper class that adds contracts to std::vector.
template<typename T>
class myvector: public pushable<T> {

    CONTRACT_CLASS( /*< The class signature tokens are passed to the `__CONTRACT_CLASS__()` macro in the exact same order as they appear in the class declaration. The parenthesis `()` around the tokens are mandatory (the create a __Boost_Preprocessor__ /sequence/. >*/ (myvector) (pushable<T>) /*< Subcontract from base contracts in `pushable<T>` (for `push_back()` in this example). Multiple bases classes can be specified to support subcontracting for multiple-inheritance. >*/
    (invariant)( /*< Static class invariants are also supported. >*/
        (empty() == (size() == 0)) /*< By default, a contract assertion failure terminates the program. However, programmers can customize the action to take on contract assertion failure to throw exceptions, exit the program, etc. >*/
        // More invariants here...
    ) ) /*< No need for "`;`" after the macro closing parenthesis "`)`". >*/

public:
    CONTRACT_FUNCTION( /*< The function signature tokens are passed to the `__CONTRACT_FUNCTION__()` macro in the exact same order as they appear in the function declaration. The parenthesis `()` around the tokens are mandatory (they create a __Boost_Preprocessor__ sequence). >*/
    (public) (void) (push_back)( (const T&)(element) ) (copyable) /*< The object is qualified copyable within this function so postconditions can use `CONTRACT_OLDOF(this)` to access a copy of the object before the body is executed. Similarly, the function arguments can also be qualified copyable: For example, if `(copyable)(const T&)(element)` is specified in the function signature then postconditions can use `CONTRACT_OLDOF(element)` to access a copy of the `element` argument before the body execution. >*/
        (precondition)( /*< The precondition section `(precondition) ({...})` and the postcondition section `(postcondition) ({...})` can both be omitted, the body section `(body) ({...})` is mandatory instead. >*/
            (size() < max_size())
            // More preconditions here...
        )
        (postcondition)(
            (size() == (CONTRACT_OLDOF(this)->size() + 1))
            // More postconditions here...
        )
    ({ /*< The body specifies the original function definition. Instead of `{ vector_.push_back(element); }`, `";"` can be used to separate the function definition from its declaration, or `" = 0;"` can be used to write contracts for pure virtual functions. >*/
        vector_.push_back(element); // Original implementation.
    }) ) /*< Again, no need for "`;`" after macro closing parenthesis "`)`". >*/
    
    // Rest of the class here (with more contracts if needed)...
    typedef typename std::vector<T>::size_type size_type;
    size_type size(void) const { return vector_.size(); }
    size_type max_size(void) const { return vector_.max_size(); }
    bool empty(void) const { return vector_.empty(); }
    const T& back(void) const { return vector_.back(); }
private:
    std::vector<T> vector_;
};

//]

int main() { // Simple test program.
    myvector<int> v;
    v.push_back(123);
    return 0;
}

