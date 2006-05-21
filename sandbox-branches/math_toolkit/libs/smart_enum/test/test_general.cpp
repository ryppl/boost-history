// Test program to verify smart_enum<> and friends.

#include <boost/test/unit_test.hpp>
using boost::unit_test_framework::test_suite;

template<typename enumT>
struct UncheckedEnumTraits
    {
    typedef enumT enum_type;

    static enum_type cast(const int value)
        {
        return static_cast<enum_type>(value);
        }

    static void increment(enum_type& value, signed int offset)
        {
        value = static_cast<enum_type>(value + offset);
        }
    };

template
    <
    typename EnumT,
    typename EnumTraits = UncheckedEnumTraits<EnumT>
    >
class smart_enum
    {
  public:
    typedef EnumT enum_type;
    typedef EnumTraits enum_traits;
    typedef smart_enum self_type;

    smart_enum(const enum_type value) : m_value(value)
        {
        }
    self_type& operator= (const enum_type value)
        {
        m_value = value;
        return *this;
        }

    smart_enum(const int value) : m_value(enum_traits::cast(value))
        {
        }
    self_type& operator= (const int value)
        {
        m_value = enum_traits::cast(value);
        return *this;
        }

    self_type& operator += (signed int offset)
        {
        enum_traits::increment(m_value, offset);
        return *this;
        }
    self_type& operator -= (signed int offset)
        {
        return (*this += (-offset));
        }

    operator const enum_type () const
        {
        return m_value;
        }

    friend self_type& operator++ (self_type& value)
        {
        return (value += 1);
        }
    friend self_type& operator-- (self_type& value)
        {
        return (value -= 1);
        }
    friend const self_type operator++ (self_type& value, int)
        {
        self_type tmp(value);
        value += 1;
        return tmp;
        }
    friend const self_type operator-- (self_type& value, int)
        {
        self_type tmp(value);
        value -= 1;
        return tmp;
        }
#if 0
    friend bool operator== (self_type& lhs, self_type& rhs)
        {
        return (lhs.m_value == rhs.m_value);
        }
    friend bool operator!= (self_type& lhs, self_type& rhs)
        {
        return (lhs.m_value != rhs.m_value);
        }
#endif
  private:
    enum_type m_value;
    };


enum myEnum { STATE1, STATE2, STATE3, STATE4 };

void smart_enum_size_test()
    {
    BOOST_MESSAGE("sizeof(smart_enum<>) = " << sizeof(smart_enum<myEnum>));
    BOOST_WARN(sizeof(smart_enum<myEnum>) == sizeof(myEnum));
    }

void state_enum_test()
    {
    typedef smart_enum<myEnum> enum_type;
    enum_type state = 0;
    BOOST_CHECK_EQUAL((state += 1), STATE2);
    BOOST_CHECK_EQUAL((++state), STATE3);
    BOOST_CHECK_EQUAL((--state), STATE2);
    BOOST_CHECK_EQUAL((state++), STATE2);
    BOOST_CHECK_EQUAL((state--), STATE3);
    BOOST_CHECK_EQUAL((state), STATE2);
    BOOST_CHECK_EQUAL((state -= 1), STATE1);

    state = STATE4;
    BOOST_CHECK_EQUAL(state, STATE4);

    enum_type tmp = state;
    BOOST_CHECK_EQUAL(state, tmp);
    BOOST_CHECK(state < tmp+1);

    tmp = state - 1;
    BOOST_CHECK_EQUAL(tmp, STATE3);
    }

test_suite* init_unit_test_suite(int argc, char** argv)
    {
    test_suite* test = BOOST_TEST_SUITE("test_general");
    test->add(BOOST_TEST_CASE( &smart_enum_size_test ) );
    test->add(BOOST_TEST_CASE( &state_enum_test ) );
    return test;
    }
