#include <tr1/functional>
using std::tr1::function;
void executer( function<void()> arg ) { arg(); }
void passer( function<void()> arg ) { executer( arg ); }
#include "trivial_closure.h"
#include "trivial_initiator.h"
#include "trivial_main.h"
