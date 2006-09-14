#ifndef BOOST_S11N_SERIALIZE_HPP_INCLUDED
#define BOOST_S11N_SERIALIZE_HPP_INCLUDED

#include <boost/s11n/access.hpp>

namespace boost { namespace s11n
{

    template<class Archive, class T> void serialize(Archive &ar, T &t)
    {
        access::serialize(ar, t);
    }

} }

#endif
