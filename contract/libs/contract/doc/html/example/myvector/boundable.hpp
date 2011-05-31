
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOUNDABLE_HPP_
#define BOUNDABLE_HPP_

//[ boundable_cpp

#include <contract.hpp>

template<class ConstIter>
class boundable {

    CONTRACT_CLASS( (boundable)
    (invariant)(
        (begin() <= end())
    ) )

public:
    virtual ~boundable(void) {}

    CONTRACT_FUNCTION(
    (public) (virtual) (ConstIter) (begin)( (void) ) (const)
    (
        = 0;
    ) )

    virtual ConstIter end(void) const = 0;
};

//]

#endif // #include guard

