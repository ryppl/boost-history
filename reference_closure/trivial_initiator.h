#include "trivial_closure.h"
#include "parallel_lib.h"

void initiator() {
    frame mine = { 3, 4, 5 };

#if REFERENCE_CLOSURE
    
    closure child;
    child.func = nested; child.scope = &mine;
    
#elif STD_FUNCTION

    lambda child;
    child.scope = &mine;
    
#endif 
    passer( child );
}
