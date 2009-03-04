#include "trivial_closure.h"
void executer( closure arg ) { arg(); }
void passer( closure arg ) { executer( arg ); }
#include "trivial_initiator.h"
#include "trivial_main.h"
