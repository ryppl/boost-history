#include <iostream>
#define BOOST_ASSIGN_CHECK_EQUAL(a,b) BOOST_ASSERT(a==b)
#include <boost/assign/auto_size/check/ref_list_of.hpp>
//#include <boost/assign/auto_size/check/ref_csv.hpp>
#undef BOOST_ASSIGN_CHECK_EQUAL
#include <libs/assign/example/range.h>

int main (int argc, char * const argv[]) {

    using namespace boost::assign::detail::auto_size;

    void (*fp)();
    
    {   // 1
        fp = check_ref_list_of_example1<int>;
       (*fp)();
    }
    {   // 2
        fp = check_ref_list_of_iterator<int>;
       (*fp)();
    }
    {   // 3
        fp = check_ref_list_of_array<int>;
       (*fp)();
    }
    {   // 4
        fp = check_ref_list_of_copy_iterator<int>;
       (*fp)();
    }
    {   // 5
        fp = check_ref_list_of_copy_array<int>;
       (*fp)();
    }
    {   // 6
        fp = check_ref_list_of_rebind_array<int>;
       (*fp)();
    }
    {   // 7
        fp = check_ref_list_of_converter_list<int>;
       (*fp)();
    }
    {   // 8
        fp = check_ref_list_of_converter_vector<int>;
       (*fp)();
    }
    {   // 9
        fp = check_ref_list_of_converter_array<int>;
       (*fp)();
    }
    {   // 10
        fp = check_ref_list_of_converter_set<int>;
       (*fp)();
    }
    {   // 11
        fp = check_ref_list_of_adapter_stack<int>;
       (*fp)();
    }
    {   // 12
        fp = check_ref_list_of_adapter_queue<int>;
       (*fp)();
    }
	{   // 13
        fp = check_ref_list_of_chain_r<int>;
       (*fp)();
    }
/*
    {   // 14
        fp = check_ref_list_of_chain_l<int>;
       (*fp)();
    }
    {   // 15
        fp = check_ref_list_of_example2<int>;
       (*fp)();
    }
*/    std::cout << "check_ref_list_of : ok" << std::endl;

    example_range(std::cout);

    return 0;

}
