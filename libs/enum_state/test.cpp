// Test program to verify smart_enum<> and friends.

// ISO C++ headers.
#include <stdexcept>
#include <functional>

template<typename enumT, enumT minVal, enumT maxVal>
struct SequentialIncrementor : public std::binary_function<const int, const int, enumT>
    {
    enumT operator() (const int val, const int n) const
        {
        if (val + n >= minVal && val + n <= maxVal)
            return enumT(val + n);
        else
            throw std::out_of_range("incrementation would yield invalid enum");
        }
    };

template<typename enumT, enumT minVal, enumT maxVal>
struct WrappingIncrementor : public std::binary_function<const int, const int, enumT>
    {
    enumT operator() (const int val, const int n) const
        {
        return enumT(((val - minVal + n) % (maxVal - minVal + 1)) + minVal);
        }
    };

template<typename enumT, enumT minVal, enumT maxVal>
struct BoundedIncrementor : public std::binary_function<const int, const int, enumT>
    {
    enumT operator() (const int val, const int n) const
        {
        int res = val + n;
        if (res < minVal)
            return minVal;
        else if (res > maxVal)
            return maxVal;
        else
            return enumT(res);
        }
    };

template<typename enumT, class incrementorT>
class smart_enum
    {
  public:
    typedef enumT         enum_type;
    typedef incrementorT  incrementor_type;

    smart_enum(const int i, const incrementor_type func = incrementor_type())
            : incrementor(func)
        {
        val = incrementor(i, 0);
        }
    smart_enum(const enum_type i, const incrementor_type func = incrementor_type())
            : incrementor(func), val(i)
        {
        }

    smart_enum&      operator++ ()      { return (*this = incrementor(val, 1)); }
    smart_enum&      operator-- ()      { return (*this = incrementor(val, -1)); }
    const smart_enum operator++ (int)   { enum_type tmp(val); ++(*this); return tmp; }
    const smart_enum operator-- (int)   { enum_type tmp(val); --(*this); return tmp; }
    smart_enum&      operator+= (int n) { return (*this = incrementor(val, n)); }
    smart_enum&      operator-= (int n) { return (*this = incrementor(val, -n)); }

    operator enum_type() const          { return val; }

  private:
    incrementor_type    incrementor;
    enum_type           val;
    };

//////////////////////////////////////////////////
// Test program.                                //
//////////////////////////////////////////////////

// ISO C++ headers.
#include <iostream>

// Boost library headers.
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/static_assert.hpp>
#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>

class sequential_enum_test
    {
  public:
    sequential_enum_test()
        {
        smart_enum<myEnum, SequentialIncrementor<myEnum, state1, state4> > e(state1);

        // Test operator++
        BOOST_CRITICAL_TEST(e == state1);
        BOOST_CRITICAL_TEST(++e == state2);
        BOOST_CRITICAL_TEST(++e == state3);
        BOOST_CRITICAL_TEST(e++ == state3);
        BOOST_CRITICAL_TEST(e == state4);
        try { ++e; BOOST_CRITICAL_TEST(false); }
        catch(const std::out_of_range&) { }
        BOOST_CRITICAL_TEST(e == state4);

        // Test assignment
        try { e = 50; BOOST_CRITICAL_TEST(false); }
        catch(const std::out_of_range&) { }
        BOOST_CRITICAL_TEST(e == state4);
        BOOST_CRITICAL_TEST((e = state4) == state4);

        // Test operator--
        BOOST_CRITICAL_TEST(e-- == state4);
        BOOST_CRITICAL_TEST(e == state3);
        BOOST_CRITICAL_TEST(--e == state2);
        BOOST_CRITICAL_TEST(--e == state1);
        try { --e; BOOST_CRITICAL_TEST(false); }
        catch(const std::out_of_range&) { }
        BOOST_CRITICAL_TEST(e == state1);

        // Test operator+= and operator-=
        BOOST_CRITICAL_TEST((e += 1) == state2);
        try { e -= 5; BOOST_CRITICAL_TEST(false); }
        catch(const std::out_of_range&) { }
        BOOST_CRITICAL_TEST(e == state2);
        BOOST_CRITICAL_TEST((e -= 1) == state1);
        BOOST_CRITICAL_TEST((e -= -1) == state2);

        // Test free operator+ and operator-
        BOOST_CRITICAL_TEST((e = state1 + 1) == state2);
        try { e = e - 80; BOOST_CRITICAL_TEST(false); }
        catch(const std::out_of_range&) { }
        BOOST_CRITICAL_TEST(e == state2);
        try { e = -4 - 80; BOOST_CRITICAL_TEST(false); }
        catch(const std::out_of_range&) { }
        BOOST_CRITICAL_TEST(e == state2);
        BOOST_CRITICAL_TEST((e = 84 - 81) == state4);
        }
  private:
    enum myEnum { state1, state2, state3, state4 };
    };

class wrapped_enum_test
    {
  public:
    wrapped_enum_test()
        {
        smart_enum<myEnum, WrappingIncrementor<myEnum, state1, state4> > e(state1);

        // Verify wrapping properties
        BOOST_CRITICAL_TEST(e == state1);
        BOOST_CRITICAL_TEST(++e == state2);
        BOOST_CRITICAL_TEST(++e == state3);
        BOOST_CRITICAL_TEST(e++ == state3);
        BOOST_CRITICAL_TEST(e == state4);
        BOOST_CRITICAL_TEST(++e == state1);
        BOOST_CRITICAL_TEST((e += state4-state1+1) == state1);
        BOOST_CRITICAL_TEST((e = state3 + (state4-state1+1)*2) == state3);
        }
  private:
    enum myEnum { state1 = 5, state2, state3, state4 };
    };

class bounded_enum_test
    {
  public:
    bounded_enum_test()
        {
        smart_enum<myEnum, BoundedIncrementor<myEnum, state1, state4> > e(state1);

        // Verify upper bound
        BOOST_CRITICAL_TEST(e == state1);
        BOOST_CRITICAL_TEST(++e == state2);
        BOOST_CRITICAL_TEST(++e == state3);
        BOOST_CRITICAL_TEST(++e == state4);
        BOOST_CRITICAL_TEST(++e == state4);

        // Verify lower bound
        BOOST_CRITICAL_TEST(--e == state3);
        BOOST_CRITICAL_TEST(--e == state2);
        BOOST_CRITICAL_TEST(--e == state1);
        BOOST_CRITICAL_TEST(--e == state1);

        // Verify assignment
        BOOST_CRITICAL_TEST((e = 50) == state4);
        BOOST_CRITICAL_TEST((e = -50) == state1);
        }
  private:
    enum myEnum { state1 = 5, state2, state3, state4 };
    };


int test_main(int, char*[])
    {
    sequential_enum_test();
    wrapped_enum_test();
    bounded_enum_test();
    return 0;
    }
