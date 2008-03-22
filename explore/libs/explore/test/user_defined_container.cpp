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
#include <boost/explore.hpp>

class user_vector
{
public:
    user_vector()
    {
        m_vec.push_back(1);
        m_vec.push_back(2);
        m_vec.push_back(3);
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
    return explore::stream_container(ostr, u.start(), u.finish());
}

BOOST_AUTO_TEST_CASE( user_defined_stream_test )
{
    std::stringstream str_out;

    user_vector v;
    str_out << v;
    BOOST_CHECK_EQUAL(str_out.str(), "[1, 2, 3]");
}

