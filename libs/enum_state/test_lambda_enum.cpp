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

// Helper class that will eventually move into the library header.

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

enum myEnum { north = 17, east = 4, south = 92, west = -5 };

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

int test_main(int, char*[])
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

    // Done
    return 0;
    }
