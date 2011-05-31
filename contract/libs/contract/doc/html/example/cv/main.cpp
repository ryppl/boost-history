
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#include <contract.hpp>
#include <iostream>

// Qualified member access works as usual:
// 1. Class invariants can access any const and const-volatile member.
// 2. Pre/postconditions of const members can access any const and
//    const-volatile member.
// 3. However, pre/postconditions of volatile members can only access
//    const-volatile members.

class b {

    CONTRACT_CLASS( (b) )

public:
    virtual ~b(void)
    CONTRACT_DESTRUCTOR(
            (public) (virtual) (b)( (void) )
    (body) ({
    }) )

    virtual int f(void)
    CONTRACT_FUNCTION(
            (public) (virtual) (int) (f)( (void) )
    (body) ({
        return -1;
    }) )

    virtual int f(void) const
    CONTRACT_FUNCTION(
            (public) (virtual) (int) (f)( (void) ) (const)
    (body) ({
        return -2;
    }) )
    
    virtual int f(void) volatile
    CONTRACT_FUNCTION(
            (public) (virtual) (int) (f)( (void) ) (volatile)
    (body) ({
        return -3;
    }) )
    
    virtual int f(void) const volatile
    CONTRACT_FUNCTION(
            (public) (virtual) (int) (f)( (void) ) (const) (volatile)
    (body) ({
        return -4;
    }) )
};

//[ cv_cpp

class z: public b { // Some base class b to show subcontracting.

    CONTRACT_CLASS( (z) (b)
    (invariant) ({ // const
        CONTRACT_ASSERT( check_c() && check_cv() );
    }) )

public:
    z(void) {}

    // For old object, must manually program const-correct copy
    // constructor for volatile type (instead use `z(constz&)`
    // automatically synthesized by C++ for non-volatile type).
    z(const volatile z& source) {}

    int f(void)
    CONTRACT_FUNCTION(
            (public) (int) (f)( (void) ) (copyable) // no-cv
    (precondition) ({ // const
        CONTRACT_ASSERT( check_c() && check_cv() );
    })
    (postcondition) (result) ({ // const
        CONTRACT_ASSERT( check_c() && check_cv() );
        CONTRACT_ASSERT( // oldof(this) also const
                CONTRACT_OLDOF(this)->check_c() &&
                CONTRACT_OLDOF(this)->check_cv() );
    })
    (body) ({ // no-cv
        check_nocv(); check_c(); check_v(); check_cv();
        return 1;
    }) )

    int f(void) const
    CONTRACT_FUNCTION(
            (public) (int) (f)( (void) ) (const) (copyable)
    (precondition) ({ // const
        CONTRACT_ASSERT( check_c() && check_cv() );
    })
    (postcondition) (result) ({ // const
        CONTRACT_ASSERT( check_c() && check_cv() );
        CONTRACT_ASSERT( // oldof(this) also const
                CONTRACT_OLDOF(this)->check_c() &&
                CONTRACT_OLDOF(this)->check_cv() );
    })
    (body) ({ // const
        check_c(); check_cv();
        return 2;
    }) )
    
    int f(void) volatile
    CONTRACT_FUNCTION(
            (public) (int) (f)( (void) ) (volatile) (copyable)
    (precondition) ({ // const volatile
        CONTRACT_ASSERT( check_cv() );
    })
    (postcondition) (result) ({ // const volatile
        CONTRACT_ASSERT( check_cv() );
        CONTRACT_ASSERT( // oldof(this) also const volatile
                CONTRACT_OLDOF(this)->check_cv() );
    })
    (body) ({ // volatile
        check_v(); check_cv();
        return 3;
    }) )
    
    int f(void) const volatile // const appears before volatile.
    CONTRACT_FUNCTION(
            (public) (int) (f)( (void) ) (const) (volatile) (copyable)
    (precondition) ({ // const volatile
        CONTRACT_ASSERT( check_cv() );
    })
    (postcondition) (result) ({ // const volatile
        CONTRACT_ASSERT( check_cv() );
        CONTRACT_ASSERT( // oldof(this) also const volatile
                CONTRACT_OLDOF(this)->check_cv() );
    })
    (body) ({ // const volatile
        check_cv();
        return 4;
    }) )
    
    // Used to demonstrate qualified member access constraints.
    bool check_nocv(void) { return true; }
    bool check_c(void) const { return true; }
    bool check_v(void) volatile { return true; }
    bool check_cv(void) const volatile { return true; }
};

//]

int main() {
    z nocv_z;
    std::cout << nocv_z.f() << std::endl;
    
    const z& c_z = nocv_z;
    std::cout << c_z.f() << std::endl;
    
    volatile z& v_z = nocv_z;
    std::cout << v_z.f() << std::endl;
    
    const volatile z& cv_z = nocv_z;
    std::cout << cv_z.f() << std::endl;

    return 0;
}

