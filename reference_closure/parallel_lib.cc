#include "callback_type.h"

void executer( callback_type arg ) { arg(); }
void passer( callback_type arg ) { executer( arg ); }
