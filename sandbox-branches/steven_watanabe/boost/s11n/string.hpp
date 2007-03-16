#ifndef BOOST_S11N_STRING_HPP_INCLUDED
#define BOOST_S11N_STRING_HPP_INCLUDED

#include <boost/s11n/types.hpp>
#include <boost/s11n/nvp.hpp>
#include <string>

namespace boost { namespace s11n
{

    template<class Archive, class T> void serialize(Archive &ar, std::basic_string<T> &t)
    {
        if (Archive::is_loading)
        {
            container_size_t size;
            ar & size;
            t.clear();
            t.reserve(size);
            for (std::size_t i = 0; i < size; ++i)
            {
                T tmp;     
                ar & make_nvp("item", tmp);
                t.push_back(tmp);
            }
        }
        else
        {
            container_size_t size(t.size());
            ar & size;
            for (typename std::basic_string<T>::iterator it = t.begin(); it != t.end(); ++it)
                ar & make_nvp("item", *it);
        }
    }

} }

#endif
