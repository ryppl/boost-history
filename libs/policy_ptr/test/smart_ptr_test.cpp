//----------------------------------------------------------------------------
// smart_ptr_test.cpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// This is the main test program for the policy-based smart pointer library.
// All tests should pass.  The ordering_test.cpp tests the ordering comparison
// operators (in a separate file, because they are enabled via a macro).  See
// libs/policy_ptr/doc/index.html
//----------------------------------------------------------------------------
// Change log:
// - 29 Oct 2004: Jonathan Turkanis modified move_copy_test to reflect cases
//   which should fail.
// Change log:
// - 06 May 2005: Larry Evans uncommented #include for unit_test and rm'ed that
//   for unit_test_framework because, with unit_test_framework, got
//     EXIT STATUS: 139
//----------------------------------------------------------------------------
#define BOOST_SP_DEBUG_MODE
#include <string>
#include <boost/test/unit_test.hpp>

#include "../../../boost/policy_ptr/smart_ptr.hpp"
#include "../../../boost/policy_ptr/policy/array_storage.hpp"
#include "../../../boost/policy_ptr/policy/com_ref_counted.hpp"
#include "../../../boost/policy_ptr/policy/ref_linked.hpp"
#include "../../../boost/policy_ptr/policy/deep_copy.hpp"
#include "../../../boost/policy_ptr/policy/no_copy.hpp"
#include "../../../boost/policy_ptr/policy/move_copy.hpp"
#include "../../../boost/policy_ptr/policy/auto_copy.hpp"
#include "../../../boost/policy_ptr/policy/reject_null.hpp"
//----------------------------------------------------------------------------
// Hack for VC's lack of ADL
namespace boost
{
//----------------------------------------------------------------------------
namespace test = unit_test_framework;
//----------------------------------------------------------------------------
// Debug hooks
template <class SP>
void sp_constructor_hook(SP const& sp, std::string msg)
{
    BOOST_MESSAGE(msg);
}

template <class SP>
void sp_destructor_hook(SP const& sp, std::string msg)
{
    BOOST_MESSAGE(msg);
}
//----------------------------------------------------------------------------
// Test types
//----------------------------------------------------------------------------
// Base types
class ext_counted
{
protected:          // Interface
                    ext_counted(bool& flag)
                        : destroyed_(flag)
    {
        BOOST_CHECK(!destroyed_);
    }

                   ~ext_counted(void)
    {
        BOOST_CHECK(!destroyed_);
        destroyed_ = true;
    }
private:            // Implementation
    bool&           destroyed_;
};
//----------------------------------------------------------------------------
class array_element
{
public:             // Interface
                   ~array_element(void)             { ++destroyed_; }
    static
    int             destroyed(void)                 { return destroyed_; }
private:            // Implementation
    static
    int             destroyed_;
};
//----------------------------------------------------------------------------
int             array_element::destroyed_ = 0;
//----------------------------------------------------------------------------
class IUnknown
{
public:             // Interface
                    IUnknown(void)
                        : count_(1)                 { }

    int             Count(void) const               { return count_; }

    void            AddRef(void)                    { ++count_; }
    long            Release(void)                   { return --count_; }
private:            // Implementation
    int             count_;
};
//----------------------------------------------------------------------------
class clonable
{
public:             // Interface
                    clonable(void)                  { ++created_; }
    virtual        ~clonable(void)                  { ++destroyed_; }

    virtual
    clonable*       clone(void) const = 0;

    static
    int             created(void)                   { return created_; }
    static
    int             destroyed(void)                 { return destroyed_; }
private:            // Implementation
    static
    int             created_;
    static
    int             destroyed_;
};
//----------------------------------------------------------------------------
clonable* clonable::clone(void) const
{
    ++created_;
    return 0;
}
int             clonable::created_ = 0;
int             clonable::destroyed_ = 0;
//----------------------------------------------------------------------------
class employee
{
public:             // Types
    typedef unsigned                                age_type;
    typedef double                                  salary_type;
public:             // Constants
    static
    age_type const  default_age;
    static
    salary_type const
                    default_salary;
public:             // Interface
                    employee(age_type age, salary_type salary)
                        : age_(age), salary_(salary)
                                                    { }
    virtual        ~employee(void)                  { }

    unsigned        age(void) const                 { return age_; }
    double          salary(void) const              { return salary_; }

    void            promote(salary_type salary)     { salary_ = salary; }
private:            // Implementation
    unsigned        age_;
    double          salary_;
};
//----------------------------------------------------------------------------
employee::age_type const employee::default_age = 42;
employee::salary_type const employee::default_salary = 65535.99;
//----------------------------------------------------------------------------
class manager
{
public:             // Types
    typedef unsigned                                office_type;
public:             // Interface
                    manager(office_type office)
                        : office_(office)           { }

    office_type     office(void) const              { return office_; }
private:            // Implementation
    office_type     office_;
};
//----------------------------------------------------------------------------
// Derived types
class rc_employee : private ext_counted, public employee
{
public:             // Interface
                    rc_employee(
                        bool& flag,
                        age_type age = default_age,
                        salary_type salary = default_salary
                    )   : ext_counted(flag), employee(age, salary)
                                                    { }
};
//----------------------------------------------------------------------------
class rc_manager : public rc_employee, public manager
{
public:             // Interface
                    rc_manager(
                        bool& flag, office_type office,
                        age_type age = default_age,
                        salary_type salary = default_salary
                    )   : rc_employee(flag, age, salary), manager(office)
                                                    { }
};
//----------------------------------------------------------------------------
class array_employee : public array_element, public employee
{
public:             // Interface
                    array_employee(void)
                        : employee(default_age, default_salary)
                                                    { }
};
//----------------------------------------------------------------------------
class com_employee : public IUnknown, public employee
{
public:             // Interface
                    com_employee(
                        age_type age = default_age,
                        salary_type salary = default_salary
                    )   : employee(age, salary)     { }
};
//----------------------------------------------------------------------------
class clone_employee : public clonable, public employee
{
public:             // Interface
                    clone_employee(
                        age_type age,
                        salary_type salary = default_salary
                    )   : employee(age, salary)     { }

    clone_employee* clone(void) const
    {
        clonable::clone();
        return new clone_employee(*this);
    }
};
//----------------------------------------------------------------------------
// Test functions
void policy_adaptor_test(void)
{
    smart_ptr<scalar_storage>::to<employee>();
    smart_ptr<array_storage>::to<employee>();
    smart_ptr<ref_counted>::to<employee>();
    smart_ptr<ref_linked>::to<employee>();
    smart_ptr<assert_check>::to<employee>();
    smart_ptr<reject_null>::to<employee>();
    smart_ptr<allow_conversion>::to<employee>();
    smart_ptr<array_storage, ref_linked>::to<employee>();
    smart_ptr<no_check, no_copy>::to<employee>();
    smart_ptr<
        allow_conversion, com_ref_counted, array_storage
    >::to<com_employee>();
}
//----------------------------------------------------------------------------
void default_test(void)
{
    smart_ptr<>::to<employee> p;
    employee* q = 0;
    BOOST_CHECK_EQUAL(sizeof(p), sizeof(employee*) + sizeof(unsigned));
    BOOST_CHECK(!p);
    BOOST_CHECK(p == q);
    BOOST_CHECK(!get_impl(p));
}
//----------------------------------------------------------------------------
void init_const_test(smart_ptr<>::to<rc_employee> const& p)
{
    BOOST_CHECK_EQUAL(p->age(), employee::default_age);
    BOOST_CHECK_EQUAL((*p).salary(), employee::default_salary);
}
//----------------------------------------------------------------------------
void init_test(void)
{
    bool destroyed(false);
    {
        smart_ptr<>::to<rc_employee> p(new rc_employee(destroyed));
        BOOST_CHECK_EQUAL(p->age(), employee::default_age);
        BOOST_CHECK_EQUAL((*p).salary(), employee::default_salary);
        init_const_test(p);
        smart_ptr<>::to<rc_employee> q;
        BOOST_CHECK(p != q);
        if (p)  BOOST_CHECK(true);
        else    BOOST_CHECK(false);
    }
    BOOST_CHECK(destroyed);
}
//----------------------------------------------------------------------------
void copy_test(void)
{
    bool employee_destroyed(false);
    {
        smart_ptr<>::to<rc_employee> p(new rc_employee(employee_destroyed));
        BOOST_CHECK_EQUAL(use_count(p), 1U );
        bool manager_destroyed(false);
        {
            smart_ptr<>::to<rc_employee> q(p);
            BOOST_CHECK(p == q);
            BOOST_CHECK_EQUAL(q->age(), employee::default_age);
            BOOST_CHECK_EQUAL(use_count(p), 2U );
            BOOST_CHECK_EQUAL(use_count(q), 2U );

            reset(q, new rc_manager(manager_destroyed, 3));
            smart_ptr<>::to<rc_manager> c(static_pointer_cast<rc_manager>(q));
            BOOST_CHECK_EQUAL(use_count(c), 2U );
            smart_ptr<>::to<rc_manager> d(dynamic_pointer_cast<rc_manager>(q));
            BOOST_CHECK_EQUAL(use_count(q), 3U );
            smart_ptr<>::to<rc_manager const> e(const_pointer_cast<rc_manager const>(d));
            BOOST_CHECK_EQUAL(use_count(q), 4U );
            smart_ptr<>::to<rc_manager> f(const_pointer_cast<rc_manager>(e));
            BOOST_CHECK_EQUAL(use_count(q), 5U );
        }
        BOOST_CHECK(manager_destroyed);
        BOOST_CHECK_EQUAL(use_count(p), 1U );
    }
    BOOST_CHECK(employee_destroyed);
}
//----------------------------------------------------------------------------
void assign_test(void)
{
    using boost::swap;
    bool destroyed1(false);
    bool destroyed2(false);
    rc_manager* x = 0;
    {
        smart_ptr<>::to<rc_manager> p(new rc_manager(destroyed1, 14));
        smart_ptr<>::to<rc_employee> q;
        q = p;
        BOOST_CHECK(p == q);
        BOOST_CHECK_EQUAL(use_count(p), 2U );
        smart_ptr<>::to<rc_manager> c;
        c = p;
        BOOST_CHECK_EQUAL(use_count(p), 3U );
        smart_ptr<>::to<rc_manager> d(new rc_manager(destroyed2, 27));
        swap(c, d);
        BOOST_CHECK_EQUAL(d->office(), 14U );
        BOOST_CHECK_EQUAL(c->office(), 27U );
        BOOST_CHECK_EQUAL(use_count(c), 1U );
        release(c, x);
        BOOST_CHECK(x != 0);
        BOOST_CHECK_EQUAL(use_count(c), 0U );
        BOOST_CHECK(!c);
    }
    BOOST_CHECK(destroyed1);
    BOOST_CHECK(!destroyed2);
    delete x;
}
//----------------------------------------------------------------------------
void array_test(void)
{
    {
        smart_ptr<array_storage>::to<array_employee> p(new array_employee[4]);
        BOOST_CHECK_EQUAL(sizeof(p), sizeof(array_employee*) + sizeof(int));
        p[0].promote(3.0);
        p[1].promote(5.0);
        p[2].promote(7.0);
        p[3].promote(11.0);
        BOOST_CHECK_EQUAL(p[2].salary(), 7.0);
        BOOST_CHECK_EQUAL(p[0].salary(), 3.0);
        BOOST_CHECK_EQUAL(p[3].salary(), 11.0);
        BOOST_CHECK_EQUAL(p[1].salary(), 5.0);
        smart_ptr<array_storage>::to<array_employee> q;
        q = p;
        BOOST_CHECK(q == p);
        array_employee* x;
        reset(q, 0);
        release(p, x);
        BOOST_CHECK(x != 0);
        BOOST_CHECK(!p);
        reset(q, x);
    }
    BOOST_CHECK_EQUAL(array_element::destroyed(), 4);
}
//----------------------------------------------------------------------------
void com_test(void)
{
    com_employee c(37);
    {
        smart_ptr<com_ref_counted>::to<com_employee> p(&c);
        BOOST_CHECK_EQUAL(sizeof(p), sizeof(com_employee*));
        BOOST_CHECK_EQUAL(p->age(), 37U );
        BOOST_CHECK_EQUAL(p->Count(), 1);
        smart_ptr<com_ref_counted>::to<com_employee> q = p;
        BOOST_CHECK(p == q);
        BOOST_CHECK_EQUAL(p->Count(), 2);
        reset(q, new com_employee(99));
        BOOST_CHECK_EQUAL(p->Count(), 1);
        com_employee* x = 0;
        release(q, x);
        BOOST_CHECK(x != 0);
        BOOST_CHECK_EQUAL(x->Count(), 1);
        BOOST_CHECK(!q);
        delete x;
    }
    BOOST_CHECK_EQUAL(c.Count(), 0);
}
//----------------------------------------------------------------------------
void ref_linked_test(void)
{
    bool destroyed1(false);
    bool destroyed2(false);
    {
        smart_ptr<ref_linked>::to<rc_employee> p(new rc_employee(destroyed1));
        BOOST_CHECK_EQUAL(sizeof(p), sizeof(rc_employee*) + sizeof(&p) * 2);
        BOOST_CHECK_EQUAL(p->age(), 42U );
        smart_ptr<ref_linked>::to<rc_employee> q = p;
        BOOST_CHECK(p == q);
        reset(q, new rc_employee(destroyed2));
        BOOST_CHECK(p != q);
        rc_employee* x = 0;
        release(q, x);
        BOOST_CHECK(x != 0);
        BOOST_CHECK(!q);
        BOOST_CHECK(!destroyed2);
        delete x;
    }
    BOOST_CHECK(destroyed1);
    BOOST_CHECK(destroyed2);
}
//----------------------------------------------------------------------------
void deep_copy_test(void)
{
    {
        smart_ptr<deep_copy>::to<clone_employee> p(new clone_employee(8));
        BOOST_CHECK_EQUAL(sizeof(p), sizeof(clone_employee*));
        BOOST_CHECK_EQUAL(p->age(), 8U );
        smart_ptr<deep_copy>::to<clone_employee> q;
        q = p;
        BOOST_CHECK(p != q);
        BOOST_CHECK_EQUAL(clone_employee::created(), 2);
        reset(q, new clone_employee(16));
        BOOST_CHECK(p != q);
        BOOST_CHECK_EQUAL(clone_employee::destroyed(), 1);
        BOOST_CHECK_EQUAL((*q).age(), 16U );
        clone_employee* x = 0;
        release(q, x);
        BOOST_CHECK(x != 0);
        BOOST_CHECK(!q);
        delete x;
    }
    BOOST_CHECK_EQUAL(clone_employee::created(), 3);
    BOOST_CHECK_EQUAL(clone_employee::destroyed(), 3);
}
//----------------------------------------------------------------------------
void no_copy_test(void)
{
    bool destroyed1(false);
    bool destroyed2(false);
    {
        smart_ptr<no_copy>::to<rc_employee> p(new rc_employee(destroyed1));
        BOOST_CHECK_EQUAL(sizeof(p), sizeof(rc_employee*));
        BOOST_CHECK_EQUAL(p->age(), 42U );
        rc_employee* x = 0;
        release(p, x);
        BOOST_CHECK(x != 0);
        BOOST_CHECK(!p);
        reset(p, new rc_employee(destroyed2));
        BOOST_CHECK(p != x);
        delete x;
    }
    BOOST_CHECK(destroyed1);
    BOOST_CHECK(destroyed2);
}
//----------------------------------------------------------------------------

smart_ptr<move_copy>::to<rc_employee> move_ptr_source(void)
{
    return smart_ptr<move_copy>::to<rc_employee>();
}
//----------------------------------------------------------------------------
void move_copy_test(void)
{
    bool destroyed(false);
    {
        smart_ptr<move_copy>::to<rc_employee> p(new rc_employee(destroyed));
        BOOST_CHECK_EQUAL(sizeof(p), sizeof(rc_employee*));
        BOOST_CHECK_EQUAL(p->age(), 42U );
        smart_ptr<move_copy>::to<rc_employee> q(move_ptr_source());
        BOOST_CHECK(!q);
        BOOST_CHECK(!!p);
        //smart_ptr<move_copy>::to<rc_employee> r(p); // Shouldn't compile.
        //smart_ptr<move_copy>::to<rc_employee> const w;
        //smart_ptr<move_copy>::to<rc_employee> s(w); // Shouldn't compile.
    }
    BOOST_CHECK(destroyed);
}
//----------------------------------------------------------------------------

smart_ptr<auto_copy>::to<rc_employee> auto_ptr_source(void)
{
    return smart_ptr<auto_copy>::to<rc_employee>();
}
//----------------------------------------------------------------------------
void auto_copy_test(void)
{
    bool destroyed(false);
    {
        smart_ptr<auto_copy>::to<rc_employee> p(new rc_employee(destroyed));
        BOOST_CHECK_EQUAL(sizeof(p), sizeof(rc_employee*));
        BOOST_CHECK_EQUAL(p->age(), 42U );
        smart_ptr<auto_copy>::to<rc_employee> q(auto_ptr_source());
        BOOST_CHECK(!q);
        BOOST_CHECK(!!p);
        smart_ptr<auto_copy>::to<rc_employee> r(p); // Shouldn't compile.
    }
    BOOST_CHECK(destroyed);
}
//----------------------------------------------------------------------------
void conversion_test(void)
{
    bool destroyed(false);
    {
        smart_ptr<allow_conversion>::to<rc_employee> p(new rc_employee(destroyed));
        rc_employee* x = p;
        (void) x;
    }
    BOOST_CHECK(destroyed);
}
//----------------------------------------------------------------------------
void concept_interface_test(void)
{
    smart_ptr<>::to<employee> p(new employee(42, 30.0));
    employee* x = get_pointer(p);
    BOOST_CHECK(x == p);
    employee y(53, 50.0);
    smart_ptr<>::to<employee> q(&y);
    less<smart_ptr<> > comp;
    if (x < &y)
    {
        BOOST_CHECK(comp(p, q));
    }
    else
    {
        BOOST_CHECK(comp(q, p));
    }
    release(q, x);
    smart_ptr<>::to<employee>::rebind<manager>::other();
}
//----------------------------------------------------------------------------
#if 0
class super2
{
public:
    super2(int ssn) : ssn_(ssn) { }

    int ssn(void) const { return ssn_; }
private:
    int ssn_;
};
//----------------------------------------------------------------------------
class child_mi : public employee, public super2
{
public:
    child_mi(bool& destroyed, int age, int ssn) : employee(destroyed), super2(ssn), age_(age) { }
    int age(void) const { return age_; }
private:
    int age_;
};
//----------------------------------------------------------------------------
void multi_inherit_test(void)
{
    bool destroyed(false);
    {
        typedef smart_ptr<>::to<child_mi> sp_type;
        typedef smart_ptr<>::to<super2> sp2_type;
        sp2_type p;
        BOOST_CHECK_EQUAL(use_count(p), 0u );
        {
            sp_type q(new child_mi(destroyed,50,999999999));
            p = q;
            BOOST_CHECK_EQUAL(use_count(p), 2u );
        }
        BOOST_CHECK(!destroyed);
        BOOST_CHECK_EQUAL(use_count(p), 1u );
    }
    BOOST_CHECK(destroyed);
}
#endif
}
//----------------------------------------------------------------------------
using namespace boost;
test::test_suite* init_unit_test_suite(int argc, char* argv[])
{
    test::test_suite* test = BOOST_TEST_SUITE("Basic smart_ptr framework tests");
    test->add(BOOST_TEST_CASE(&policy_adaptor_test));
    test->add(BOOST_TEST_CASE(&default_test));
    test->add(BOOST_TEST_CASE(&init_test));
    test->add(BOOST_TEST_CASE(&copy_test));
    test->add(BOOST_TEST_CASE(&assign_test));
    test->add(BOOST_TEST_CASE(&array_test));
    test->add(BOOST_TEST_CASE(&com_test));
    test->add(BOOST_TEST_CASE(&ref_linked_test));
    test->add(BOOST_TEST_CASE(&deep_copy_test));
    test->add(BOOST_TEST_CASE(&no_copy_test));
    test->add(BOOST_TEST_CASE(&move_copy_test));
    test->add(BOOST_TEST_CASE(&conversion_test));
    test->add(BOOST_TEST_CASE(&concept_interface_test));
//    test->add(BOOST_TEST_CASE(&multi_inherit_test));

    return test;
}
//----------------------------------------------------------------------------
