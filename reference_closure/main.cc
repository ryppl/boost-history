#include "trivial_initiator.h"

int main() {
    for ( int i = 0; i < 1000*1000*100; i++ )
       initiator();
    return 0;
}
