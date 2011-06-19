#include <iostream>
#include <libs/accumulators/ecdf/test/ecdf.h>
#include <libs/accumulators/ecdf/test/ks_sim.h>

int main()
{
    test_ecdf();
    test_ks_sim(std::cout);

    return 0;
}