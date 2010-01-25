#ifndef BOOST_CHRONO_SLEEP_FOR__HPP
#define BOOST_CHRONO_SLEEP_FOR__HPP

#include <boost/chrono/chrono.hpp>

namespace boost {
namespace this_thread {


template <class Rep, class Period>
void sleep_for(const boost::chrono::duration<Rep, Period>& d)
{
    boost::chrono::high_resolution_clock::time_point go = boost::chrono::high_resolution_clock::now() + d;
    while (boost::chrono::high_resolution_clock::now() < go);
}
}
}

#endif //BOOST_CHRONO_SLEEP_FOR__HPP
