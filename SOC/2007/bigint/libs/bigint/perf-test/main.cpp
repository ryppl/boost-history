#include <iostream>
#include <ctime>
#include <vector>
#include <set>

#include <boost/bigint/bigint.hpp>

#ifdef BOOST_BIGINT_HAS_GMP_SUPPORT
#	include <boost/bigint/bigint_gmp.hpp>
#endif

#include <boost/bigint/bigint_default.hpp>
#include <boost/bigint/bigint_storage_vector.hpp>
#include <boost/bigint/bigint_storage_fixed.hpp>

#include "match.hpp"

#include "test_add.inl"
#include "test_sub.inl"

#include "test_mul.inl"
#include "test_div.inl"
#include "test_mod.inl"

#include "test_and.inl"
#include "test_or.inl"
#include "test_xor.inl"

#include "test_lshift.inl"
#include "test_rshift.inl"

#include "test_sqrt.inl"

#include "test_to_string.inl"
#include "test_from_string.inl"

#include "test_fib.inl"
#include "test_isprime.inl"
#include "test_gcd.inl"

enum output_type
{
	ot_none,
	ot_csv,
	ot_html,
	ot_list_tests,
	ot_list_implementations
};

class performance_tool
{
	const char* test_mask;
	const char* impl_mask;
	output_type output;

	std::vector<std::vector<std::pair<const char*, float> > > results;
	std::vector<const char*> impls;

	std::vector<std::pair<const char*, float> >* impl_results;

	template <typename impl, template <class> class test> void run_test(const char* impl_name, const char* test_name)
	{
		if (output == ot_list_tests)
		{
			std::cout << test_name << std::endl;
			return;
		}
		
		if (match(test_name, test_mask))
		{
			std::clog << "Running " << test_name << "/" << impl_name << "..." << std::endl;

			test<boost::bigint_base<impl> > t;
			
			clock_t start = clock();
			t.run();
			clock_t end = clock();

			impl_results->push_back(std::make_pair(test_name, float(end-start)/CLOCKS_PER_SEC));
		}
	}

	template <typename impl> void run_impl(const char* impl_name)
	{
		if (output == ot_list_implementations)
		{
			std::cout << impl_name << std::endl;
			return;
		}

		if (match(impl_name, impl_mask))
		{
			impls.push_back(impl_name);

			results.push_back(std::vector<std::pair<const char*, float> >());
			impl_results = &results.back();

			// test_add.inl
			run_test<impl, test_add_small>(impl_name, "add_small");
			run_test<impl, test_add_large>(impl_name, "add_large");

			// test_sub.inl
			run_test<impl, test_sub_small>(impl_name, "sub_small");
			run_test<impl, test_sub_large>(impl_name, "sub_large");

			// test_mul.inl
			run_test<impl, test_mul_small>(impl_name, "mul_small");
			run_test<impl, test_mul_medium>(impl_name, "mul_medium");
			run_test<impl, test_mul_large>(impl_name, "mul_large");

			// test_div.inl
			run_test<impl, test_div_small>(impl_name, "div_small");
			run_test<impl, test_div_medium>(impl_name, "div_medium");

			// test_mod.inl
			run_test<impl, test_mod_small>(impl_name, "mod_small");
			run_test<impl, test_mod_medium>(impl_name, "mod_medium");

			// test_and.inl
			run_test<impl, test_and_small>(impl_name, "and_small");
			run_test<impl, test_and_large>(impl_name, "and_large");

			// test_or.inl
			run_test<impl, test_or_small>(impl_name, "or_small");
			run_test<impl, test_or_large>(impl_name, "or_large");

			// test_xor.inl
			run_test<impl, test_xor_small>(impl_name, "xor_small");
			run_test<impl, test_xor_large>(impl_name, "xor_large");

			// test_lshift.inl
			run_test<impl, test_lshift_small>(impl_name, "lshift_small");
			run_test<impl, test_lshift_large>(impl_name, "lshift_large");
			run_test<impl, test_lshift_exact>(impl_name, "lshift_exact");

			// test_rshift.inl
			run_test<impl, test_rshift_small>(impl_name, "rshift_small");
			run_test<impl, test_rshift_large>(impl_name, "rshift_large");
			run_test<impl, test_rshift_exact>(impl_name, "rshift_exact");

			// test_sqrt.inl
			run_test<impl, test_sqrt_small>(impl_name, "sqrt_small");
			run_test<impl, test_sqrt_medium>(impl_name, "sqrt_medium");

			// test_to_string.inl
			run_test<impl, test_to_string_small>(impl_name, "to_string_small");
			run_test<impl, test_to_string_medium>(impl_name, "to_string_medium");

			// test_from_string.inl
			run_test<impl, test_from_string_small>(impl_name, "from_string_small");
			run_test<impl, test_from_string_medium>(impl_name, "from_string_medium");

			// test_fib.inl
			run_test<impl, test_fib_small>(impl_name, "fib_small");
			run_test<impl, test_fib_large>(impl_name, "fib_large");

			// test_isprime.inl
			run_test<impl, test_isprime_small>(impl_name, "isprime_small");
			run_test<impl, test_isprime_medium>(impl_name, "isprime_medium");

			// test_gcd.inl
			run_test<impl, test_gcd_small>(impl_name, "gcd_small");
			run_test<impl, test_gcd_medium>(impl_name, "gcd_medium");

			impl_results = 0;
		}
	}

public:
	performance_tool(const char* test, const char* impl, output_type out): test_mask(test), impl_mask(impl), output(out), impl_results(0)
	{
		if (output == ot_list_implementations)
		{
			std::cout << "Available implementations:" << std::endl;
		}

		if (output == ot_list_tests)
		{
			std::cout << "Available tests:" << std::endl;
		}
	}

	void run()
	{
		if (output == ot_list_tests)
		{
			impl_mask = "default_32";
			run_impl<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_vector, 32> >("default_32");
			return;
		}

#ifdef BOOST_BIGINT_HAS_GMP_SUPPORT
		run_impl<boost::detail::bigint_gmp_implementation>("gmp");
#endif

		run_impl<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_vector, 8> >("default_8");
		run_impl<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_vector, 16> >("default_16");
		run_impl<boost::detail::bigint_default_implementation<boost::detail::bigint_storage_vector, 32> >("default_32");
	}

	~performance_tool()
	{
		if (output == ot_csv)
		{
			for (size_t i = 0; i < impls.size(); ++i)
				std::cout << ", " << impls[i];
		
			std::cout << std::endl;
		}
		else if (output == ot_html)
		{
			std::cout << "<html><head><title>Performance test results</title></head><body>\n";
			std::cout << "<h1>Performance tests results (test mask = " << test_mask << ", implementation mask = " << impl_mask << ")</h1>\n";
			std::cout << "<table border=1 cellspacing=0>\n";
			std::cout << "<tr><td></td>";
			
			for (size_t i = 0; i < impls.size(); ++i)
				std::cout << "<th>" << impls[i] << "</th>";
		
			std::cout << "</tr>\n";
		}

		std::set<std::string> tests_avail;
		std::vector<std::string> tests;

		for (size_t i = 0; i < results.size(); ++i)
			for (size_t j = 0; j < results[i].size(); ++j)
			{
				if (tests_avail.count(results[i][j].first) == 0)
				{
					tests_avail.insert(results[i][j].first);
					tests.push_back(results[i][j].first);
				}
			}

		for (std::vector<std::string>::iterator ti = tests.begin(); ti != tests.end(); ++ti)
		{
			std::vector<float> values;

			for (size_t i = 0; i < results.size(); ++i)
			{
				for (size_t j = 0; j < results[i].size(); ++j)
				{
					if (*ti == results[i][j].first)
					{
						values.push_back(results[i][j].second);
						break;
					}
				}
			}

			if (output == ot_html)
			{
				std::cout << "<tr><th>" << *ti << "</th>";

				float min = 0;
				
				if (!values.empty())
					min = *std::min_element(values.begin(), values.end());
				
				for (size_t i = 0; i < values.size(); ++i)
				{
					std::cout << "<td>";
					
					if (values[i] > min)
						std::cout << values[i];
					else
						std::cout << "<b>" << values[i] << "</b>";

					std::cout << "</td>";
				}

				std::cout << "</tr>\n";
			}
			else if (output == ot_csv)
			{
				std::cout << *ti;
				
				for (size_t i = 0; i < values.size(); ++i)
				{
					std::cout << ", " << values[i];
				}

				std::cout << "\n";
			}
		}

		if (output == ot_html)
		{
			std::cout << "</table></body></html>\n";
		}
	}
};

void run(const char* test_mask, const char* impl_mask, output_type output)
{
	performance_tool tool(test_mask, impl_mask, output);

	tool.run();
}

int main(int argc, char** argv)
{
	output_type output = ot_none;
	const char* test_mask = 0;
	const char* impl_mask = 0;

	// parsing arguments
	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			if (strcmp(argv[i], "--list-tests") == 0 && output == ot_none) output = ot_list_tests;
			else if (strcmp(argv[i], "--list-impls") == 0 && output == ot_none) output = ot_list_implementations;
			else if (strcmp(argv[i], "--output-csv") == 0 && output == ot_none) output = ot_csv;
			else if (strcmp(argv[i], "--output-html") == 0 && output == ot_none) output = ot_html;
			else
			{
				test_mask = 0;
				break;
			}
		}
		else if (!test_mask && !impl_mask)
		{
			test_mask = argv[i];
		}
		else if (!impl_mask)
		{
			impl_mask = argv[i];
		}
		else
		{
			test_mask = 0;
			break;
		}
	}

	if (output != ot_list_tests && output != ot_list_implementations && (!test_mask || !impl_mask))
	{
		std::cout << "Usage: perf-test testmask implmask [--output-csv] [--output-html]" << std::endl;
		std::cout << "Usage: perf-test --list-tests" << std::endl;
		std::cout << "Usage: perf-test --list-impls" << std::endl;
		return -1;
	}

	if (output == ot_none) output = ot_csv;

	run(test_mask, impl_mask, output);
}
