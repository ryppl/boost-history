// Boost.Explore library

// Copyright Jared McIntyre 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <vector>
#include <boost/assign.hpp>
#include <boost/explore.hpp>

class user_vector
{
public:
    user_vector()
    {
        using namespace boost::assign;
        m_vec += 1,2,3;
    }

    std::vector<int>::const_iterator start() const
    {
        return m_vec.begin();
    }

    std::vector<int>::const_iterator finish() const
    {
        return m_vec.end();
    }

private:
    std::vector<int> m_vec;
};

std::ostream& operator<<(std::ostream& ostr, const user_vector& u)
{
    return boost::explore::stream_container(ostr, u.start(), u.finish());
}

BOOST_AUTO_TEST_CASE( user_defined_stream_test )
{
    std::stringstream str_out;

    user_vector v;
    str_out << v;
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2, 3]");
}

class my_container
{
public:
    my_container()
    {
        using namespace boost::assign;
        m_data += 1, 2, 3, 4, 5, 6;
    }

    friend std::ostream& operator<<(std::ostream& ostr, const my_container& c);

private:
    std::vector<int> m_data;
};

std::ostream& operator<<(std::ostream& ostr, const my_container& c)
{
    using namespace boost::explore;
    return ostr << custom() << delimiters("/", "::", "/") << c.m_data;
}

BOOST_AUTO_TEST_CASE( custom_stream_test )
{
    using namespace boost::assign;
    using namespace boost::explore;
    std::stringstream str_out;

    my_container mc;
    str_out << mc;
    BOOST_CHECK_EQUAL(str_out.str(), "/1::2::3::4::5::6/");

    str_out.str("");

    std::vector<my_container> vmc;
    vmc += mc, mc;
    str_out << delimiters("{\n   ", "\n   ", "\n}") << vmc;
    BOOST_CHECK_EQUAL(str_out.str(),
        "{\n"
        "   /1::2::3::4::5::6/\n"
        "   /1::2::3::4::5::6/\n"
        "}");

    str_out.str("");

    std::vector<std::vector<my_container> > vvmc;
    vvmc += vmc, vmc;
    str_out << level(1) << delimiters("[", " xxx ", "]") << vvmc;
    BOOST_CHECK_EQUAL(str_out.str(),
        "{\n"
        "   [/1::2::3::4::5::6/ xxx /1::2::3::4::5::6/]\n"
        "   [/1::2::3::4::5::6/ xxx /1::2::3::4::5::6/]\n"
        "}");
}
