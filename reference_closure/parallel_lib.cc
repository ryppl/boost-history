#include "callback_type.h"
#include "move.h"

void executer( callback_type arg ) { arg(); }
void passer( callback_type arg ) { executer(
#if STD_FUNCTION
        std::move( arg )
#elif REFERENCE_CLOSURE
        arg
#endif 
        ); }
