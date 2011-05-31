
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef SUM_HPP_
#define SUM_HPP_

#include <contract.hpp>

double sum(int count, double* array)
CONTRACT_FUNCTION( (double) (sum)( (int)(count) (double*)(array) )
(precondition) ({
    CONTRACT_ASSERT( count % 4 == 0 );
})
(body) (
    ;
) )

#endif // #include guard

