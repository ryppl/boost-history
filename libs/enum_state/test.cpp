// Test program to verify smart_enum<> and friends.

// Boost library headers.
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/if.hpp>
#include <boost/lambda/switch.hpp>
#include <boost/lambda/loops.hpp>
#include <boost/lambda/exceptions.hpp>
#include <boost/static_assert.hpp>
#define BOOST_INCLUDE_MAIN
#include <boost/test/test_tools.hpp>

// The class we're testing.
#include <boost/enum_state.hpp>

class sequential_enum_test
    {
  public:
    sequential_enum_test()
        {
        typedef sequential_smart_enum<myEnum, state1, state4> my_enum_t;
        my_enum_t e(state1);

        // Test the boundaries
        BOOST_STATIC_ASSERT((myEnum(my_enum_t::min) == state1));
        BOOST_STATIC_ASSERT((myEnum(my_enum_t::max) == state4));

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
        typedef wrapped_smart_enum<myEnum, state1, state4> my_enum_t;
        my_enum_t e(state1);

        // Test the boundaries
        BOOST_STATIC_ASSERT((myEnum(my_enum_t::min) == state1));
        BOOST_STATIC_ASSERT((myEnum(my_enum_t::max) == state4));

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
        typedef bounded_smart_enum<myEnum, state1, state4> my_enum_t;
        my_enum_t e(state1);

        // Test the boundaries
        BOOST_STATIC_ASSERT((myEnum(my_enum_t::min) == state1));
        BOOST_STATIC_ASSERT((myEnum(my_enum_t::max) == state4));

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

template<typename enumT>
class LambdaIncrementor
    {
  public:
    typedef enumT enum_type;
    typedef boost::function<void, enum_type, int, enum_type&> function_type;

    LambdaIncrementor(const function_type f) : fun(f) { }

    enum_type operator() (int val, int n) const
        {
        enum_type res;
        fun(enum_type(val), n, res);
        return res;
        }
  private:
    function_type fun;
    };

// There will to be some serious magic going on here soon. Trust me:
// The following "using" statement should make you nervous.

using namespace boost::lambda;

class lambda_enum_test
    {
  public:
    lambda_enum_test()
        {
        typedef smart_enum<myEnum, MagicIncrementor> my_enum_t;
        my_enum_t e(north);

        // Test operator++
        BOOST_CRITICAL_TEST(e == north);
        BOOST_CRITICAL_TEST(++e == east);
        BOOST_CRITICAL_TEST(++e == south);
        BOOST_CRITICAL_TEST(++e == west);
        BOOST_CRITICAL_TEST(++e == north);
        BOOST_CRITICAL_TEST(++e == east);

        // Test operator--
        BOOST_CRITICAL_TEST(--e == north);
        BOOST_CRITICAL_TEST(--e == west);
        BOOST_CRITICAL_TEST(--e == south);
        BOOST_CRITICAL_TEST(--e == east);
        BOOST_CRITICAL_TEST(--e == north);

        // Test operator+= and operator-=
        BOOST_CRITICAL_TEST((e += 1) == east);
        BOOST_CRITICAL_TEST((e -= 2) == west);
        BOOST_CRITICAL_TEST((e -= 8) == west);
        BOOST_CRITICAL_TEST((e += 7) == south);

        // Test assignment
        BOOST_CRITICAL_TEST((e = -5) == west);
        try { e = 0; BOOST_CRITICAL_TEST(false); }
        catch(const std::out_of_range&) { }
        BOOST_CRITICAL_TEST(e == west);
        BOOST_CRITICAL_TEST((e = 80 + 12) == south);
        BOOST_CRITICAL_TEST((e = e / 23) == east);
        }

  private:
    enum myEnum { north = 17, east = 4, south = 92, west = -5 };

    // Abacadabra!

    struct MagicIncrementor : public LambdaIncrementor<myEnum>
        {
        MagicIncrementor() : LambdaIncrementor<myEnum>((
            // Verify that _1 is a valid myEnum
            if_then_else(_1 != north && _1 != east && _1 != south && _1 != west,
                         throw_exception(std::out_of_range("invalid myEnum!")),
                         _3 = _1),
            // Now increment it appropriately
            _2 = _2 % 4,
            if_then_else(_2 >= 0,
                         while_loop(var(_2)-- > 0, switch_statement(_3,
                                                                    case_statement<north>(_3 = east),
                                                                    case_statement<east> (_3 = south),
                                                                    case_statement<south>(_3 = west),
                                                                    case_statement<west> (_3 = north)
                                                                    )),
                         while_loop(var(_2)++ < 0, switch_statement(_3,
                                                                    case_statement<north>(_3 = west),
                                                                    case_statement<east> (_3 = north),
                                                                    case_statement<south>(_3 = east),
                                                                    case_statement<west> (_3 = south)
                                                                    ))
                        )
                                                       ))
            {
            }
        };
    };

int test_main(int, char*[])
    {
    sequential_enum_test();
    wrapped_enum_test();
    bounded_enum_test();
    lambda_enum_test();
    return 0;
    }
