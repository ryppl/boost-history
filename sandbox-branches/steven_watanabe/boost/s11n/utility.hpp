#ifndef BOOST_S11N_UTILITY_HPP_INCLUDED
#define BOOST_S11N_UTILITY_HPP_INCLUDED

#include <boost/s11n/nvp.hpp>
#include <utility>

namespace boost { namespace s11n
{

    template<class Archive, class T1, class T2> void serialize(Archive &ar, std::pair<T1, T2> &t)
    {
        ar & make_nvp("first", t.first);
        ar & make_nvp("second", t.second);
    }

} }

#endif
