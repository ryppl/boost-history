#ifndef BOOST_S11N_XML_OARCHIVE_HPP_INCLUDED
#define BOOST_S11N_XML_OARCHIVE_HPP_INCLUDED

#include <boost/s11n/detail/common_oarchive.hpp>
#include <boost/s11n/nvp.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/assert.hpp>
#include <string>

namespace boost { namespace s11n
{

    class xml_oarchive: public detail::common_oarchive<xml_oarchive>
    {

        friend class detail::common_oarchive<xml_oarchive>;

    public:    
        
        xml_oarchive():
            m_pt(), m_current(&m_pt)
        {
        }

        void get_xml(std::ostream &stream)
        {
            write_xml(stream, m_pt);
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
            m_current->put(string("<xmlattr>.") + t.name, t.value);
            //boost::property_tree::iptree::value_type v(t.name, boost::property_tree::empty_ptree<boost::property_tree::iptree>());
            //v.second.put_value(t.value);
            //m_current->push_back(v);
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

        void save_primitive(const raw_bytes &t)
        {
			std::string bytes(t.ptr, t.ptr + t.size);
			m_current->put_value(bytes);
        }

        template<class T> inline void internal_save(const T &t);

        boost::property_tree::iptree m_pt;
        boost::property_tree::iptree *m_current;
        
    };

    template<class T> inline void xml_oarchive::internal_save(const T &t)
    {
        detail::common_oarchive<xml_oarchive>::save(t);
    }
    
    template<> inline void xml_oarchive::internal_save<std::string>(const std::string &t)
    {
        m_current->put_value(t);
    }

} }

#endif
