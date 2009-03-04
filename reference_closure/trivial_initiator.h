#include "trivial_closure.h"
#include "parallel_lib.h"

void initiator() {
    frame mine
#if !DISABLE_WORK
        = { 3, 4, 5 }
#endif 
    ;

#if REFERENCE_CLOSURE
    
    closure child;
    child.func = nested; child.scope = &mine;
    
#elif STD_FUNCTION

    lambda child;
    child.scope = &mine;
    
#endif 
    passer( child );
}
