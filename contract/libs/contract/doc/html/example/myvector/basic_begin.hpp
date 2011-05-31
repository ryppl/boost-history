
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef BASIC_BEGIN_HPP_
#define BASIC_BEGIN_HPP_

//[ basic_begin_cpp

#include <contract.hpp>

template<class ConstIter>
class basic_begin {

    CONTRACT_CLASS( (basic_begin) )

public:
    virtual ~basic_begin(void) {}

    CONTRACT_FUNCTION( 
    (public) (virtual) (ConstIter) (begin)( (void) ) (const)
    ({
        return ConstIter(); // Dummy implementation (for example only).
    }) )
};

//]

#endif // #include guard

