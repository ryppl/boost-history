#ifndef BOOST_S11N_PTREE_OARCHIVE_HPP_INCLUDED
#define BOOST_S11N_PTREE_OARCHIVE_HPP_INCLUDED

#include <boost/s11n/common_oarchive.hpp>
#include <boost/s11n/nvp.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/assert.hpp>
#include <string>

namespace boost { namespace s11n
{

    class ptree_oarchive: public common_oarchive<ptree_oarchive>
    {

        friend class common_oarchive<ptree_oarchive>;

    public:    
        
        ptree_oarchive(boost::property_tree::iptree &pt): 
            m_pt(pt), m_current(&pt)
        {
        }
        
    private:    
        
        template<class T> void save(const T &t)
        {
            // Trying to serialize non-nvp data
            BOOST_STATIC_ASSERT(sizeof(T) == 0);    
        }

        template<class T> void save(const nvp<T> &t)
        {
            if (t.name)
            {
                boost::property_tree::iptree *previous = m_current;
                boost::property_tree::iptree::value_type v(t.name, boost::property_tree::empty_ptree<boost::property_tree::iptree>());
                m_current = &m_current->push_back(v)->second;
                internal_save(t.value);
                m_current = previous;
            }
            else
                internal_save(t.value);
        }

        template<class T> void save(const default_nvp<T> &t)
        {
            if (t.default_value && t.value == *t.default_value)
                return;
            if (t.name)
            {
                boost::property_tree::iptree *previous = m_current;
                boost::property_tree::iptree::value_type v(t.name, boost::property_tree::empty_ptree<boost::property_tree::iptree>());
                m_current = &m_current->push_back(v)->second;
                internal_save(t.value);
                m_current = previous;
            }
            else
                internal_save(t.value);
        }

        template<class T> void save(const concise_nvp<T> &t)
        {
            if (t.default_value && t.value == *t.default_value)
                return;
            boost::property_tree::iptree::value_type v(t.name, boost::property_tree::empty_ptree<boost::property_tree::iptree>());
            v.second.put_value(t.value);
            m_current->push_back(v);
        }

        void save(const class_name_t &t)
        {
            //m_current->put_value(static_cast<const std::string &>(t));
            m_current->put("ClassName", static_cast<const std::string &>(t));
        }

        void save(container_size_t t)
        {
            // Ignore
        }

        template<class T> void save_primitive(T t)
        {
            m_current->put_value(t);
        }

        template<class T> inline void internal_save(const T &t);

        boost::property_tree::iptree &m_pt;
        boost::property_tree::iptree *m_current;
        
    };

    template<class T> inline void ptree_oarchive::internal_save(const T &t)
    {
        common_oarchive<ptree_oarchive>::save(t);
    }
    
    template<> inline void ptree_oarchive::internal_save<std::string>(const std::string &t)
    {
        m_current->put_value(t);
    }

} }

#endif
