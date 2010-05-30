#include <iostream>

#include <libs/statistics/detail/non_parametric/example/frequency_int.h>
#include <libs/statistics/detail/non_parametric/example/kolmogorov_smirnov.h>
#include <libs/statistics/detail/non_parametric/test/contingency_table1.h>
#include <libs/statistics/detail/non_parametric/test/contingency_table2.h>

int main()
{
	example_frequency_int(std::cout);
	example_kolmogorov_smirnov(std::cout,0.1,6,10,10);
	example_kolmogorov_smirnov(std::cout,10.0,6,10,10);
	example_kolmogorov_smirnov(std::cout,100.0,6,10,10);
	test_contingency_table1();
	test_contingency_table2();

    return 0;
}