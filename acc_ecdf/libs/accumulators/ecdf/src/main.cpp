#include <iostream>
#include <libs/accumulators/ecdf/test/count.h>
#include <libs/accumulators/ecdf/test/cumulative_count.h>
#include <libs/accumulators/ecdf/test/pdf.h>
#include <libs/accumulators/ecdf/test/cdf.h>
#include <libs/accumulators/ecdf/test/kolmogorov_smirnov_statistic.h>
#include <libs/accumulators/ecdf/test/ks_gen.h>

int main()
{
    test_ecdf_count();
    test_ecdf_cumulative_count();
    test_ecdf_pdf();
    test_ecdf_cdf();
    test_ecdf_kolmogorov_smirnov_statistic();
    test_ks_gen(std::cout);

    return 0;
}