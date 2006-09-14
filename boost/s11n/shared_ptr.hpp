#ifndef BOOST_S11N_SHARED_PTR_HPP_INCLUDED
#define BOOST_S11N_SHARED_PTR_HPP_INCLUDED

#include <boost/s11n/nvp.hpp>
#include <boost/shared_ptr.hpp>

namespace boost { namespace s11n
{

    template<class Archive, class T> void serialize(Archive &ar, boost::shared_ptr<T> &t)
    {
        if (Archive::is_loading)
        {
            T *tmp;
            ar & make_nvp(NULL, tmp);
            t.reset(tmp);
        }
        else
        {
            T *tmp = t.get();
            ar & make_nvp(NULL, tmp);
        }
    }

} }

#endif
