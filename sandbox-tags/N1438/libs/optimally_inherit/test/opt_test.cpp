#include "../../../boost/optimally_inherit.hpp"
#include <string>
#include <iostream>

class P1
{
    short x;
public:
    P1() { }
    P1(P1 const&) { }
};

class P2a
{
public:
    P2a() { }
    P2a(P2a const&) { }
};
class P2b
{
public:
    P2b() { }
    P2b(P2b const&) { }
    P2b(P2a const&) { }
};

class P3
{
public:
    P3() { }
    P3(P3 const&) { }
};
class P4
{
    short y;
public:
    P4() { }
    P4(P4 const&) { }
};


template <class T1, class T2, class T3, class T4>
class smart_ptr
    : public boost::optimally_inherit<
        typename boost::optimally_inherit<T1, T2>::type,
        typename boost::optimally_inherit<T3, T4>::type
    >::type
{
public:
    typedef boost::optimally_inherit<
        boost::optimally_inherit<T1, T2>::type,
        boost::optimally_inherit<T3, T4>::type
    >::type base_type;

    smart_ptr() { }

    template <class U1, class U2, class U3, class U4>
    smart_ptr(smart_ptr<U1, U2, U3, U4>& rhs)
    : base_type(
        static_cast<typename smart_ptr<U1, U2, U3, U4>::base_type&>(rhs)
    )
    { }
};

int main()
{
    smart_ptr<P1, P2a, P3, P4> ptr1;
    smart_ptr<P1, P2a, P3, P4> ptr4(ptr1);
    smart_ptr<P1, P2b, P3, P4> ptr2(ptr1);
    //smart_ptr<P1, P2a, P3, P4> ptr3(ptr2);

    std::cout << "opt_test:" << std::endl;
    std::cout << "sizeof(ptr1): " << sizeof(ptr1) << std::endl;
    std::cout << "sizeof(opt<P1, P2a>): " << sizeof(boost::optimally_inherit<P1, P2a>::type) << std::endl;
    std::cout << "sizeof(opt<P3, P4>): " << sizeof(boost::optimally_inherit<P3, P4>::type) << std::endl;
    std::cout << "sizeof(opt<opt<>, opt<>>): "
        << sizeof(boost::optimally_inherit<
                  boost::optimally_inherit<P1, P2a>::type,
                  boost::optimally_inherit<P3, P4>::type
                  >::type) << std::endl;
    std::cout << "sizeof(P1): " << sizeof(P1) << std::endl;

    std::string s;
    std::getline(std::cin, s);

#ifdef BOOST_MSVC
    return 0;
#endif
}

