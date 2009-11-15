#include <iostream>
#include <cassert>
#include "boost/integer/pointer_plus_bits.hpp"

#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>

#define ASSERT_EQUALS(a,b) assert((a) == (b))
#define ASSERT_UNEQUALS(a,b) assert((a) != (b))
#define ASSERT(a) assert((a))


void test_tt()
{
    typedef boost::integer::pointer_plus_bits<int*,1,bool>::type pint_and_bool;

    int i=0;
    pint_and_bool v1;
    ASSERT_EQUALS(v1.pointer(),0);
    ASSERT_EQUALS(v1.small_int(),false);
    pint_and_bool v2(&i, true);
    ASSERT_EQUALS(v2.pointer(),&i);
    ASSERT_EQUALS(v2.small_int(),true);
    v1.pointer()=v2.pointer();
    v1.small_int()=true;
    ASSERT_EQUALS(v1.pointer(),&i);
    ASSERT_EQUALS(v1.small_int(),true);
    //ASSERT_EQUALS(0, 1);

}

void test_tt2()
{
    typedef boost::integer::pointer_plus_bits<boost::integer::pointer_plus_bits<int*,1,bool>::type,1, bool>::type pint_and_bool;

    //int i=0;
    pint_and_bool v1;
    boost::integer::pointer_plus_bits<int*,1,bool>::type p1 = v1.get_pointer();
//    ASSERT_EQUALS(v1.pointer(),0);
    ASSERT_EQUALS(v1.small_int(),false);
    ASSERT_EQUALS(v1.pointer().get_pointer(),0);
    ASSERT_EQUALS(v1.get_pointer().get_pointer(),0);
    ASSERT_EQUALS(v1.get_pointer().small_int(),false);

}

#if 0
void test_tt3()
{
    typedef boost::integer::pointer_plus_bitfields<int*,
        boolean<a>,boolean<b> >::type pint_and_bool;

    //int i=0;
    pint_and_bool v1;
    int* p1 = v1.get_pointer();
//    ASSERT_EQUALS(v1.pointer(),0);
    ASSERT_EQUALS(get<a>(v1),false);
    ASSERT_EQUALS(get<b>(v1),false);

}

#endif

//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    test_tt();
    test_tt2();

    std::cout << "All tests successful!\n";
    return 0;
}

