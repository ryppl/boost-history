#ifndef BOOST_S11N_MAP_HPP_INCLUDED
#define BOOST_S11N_MAP_HPP_INCLUDED

#include <boost/s11n/types.hpp>
#include <boost/s11n/utility.hpp>
#include <boost/s11n/nvp.hpp>
#include <map>

namespace boost { namespace s11n
{

    namespace detail
    {
        template<class T1, class T2>
        struct map_pair: public std::pair<T1, T2>
        {
            map_pair() { }
            map_pair(const T1 &v1, const T2 &v2): std::pair<T1, T2>(v1, v2) { }
            template<class Archive> void serialize(Archive &ar)
            {
                ar & make_concise_nvp("key", first);
                ar & make_nvp(NULL, second);
            }
        };
    }
    
    template<class Archive, class T1, class T2> void serialize(Archive &ar, std::map<T1, T2> &t)
    {
        if (Archive::is_loading)
        {
            container_size_t size;
            ar & size;
            t.clear();
            for (std::size_t i = 0; i < size; ++i)
            {
                detail::map_pair<T1, T2> tmp;
                ar & make_nvp("item", tmp);
                t[tmp.first] = tmp.second;
            }
        }
        else
        {
            container_size_t size(t.size());
            ar & size;
            for (typename std::map<T1, T2>::iterator it = t.begin(); it != t.end(); ++it)
            {
                detail::map_pair<T1, T2> tmp(it->first, it->second);
                ar & make_nvp("item", tmp);
            }
        }
    }

} }

#endif
