#ifndef BOOST_S11N_XML_IARCHIVE_HPP_INCLUDED
#define BOOST_S11N_XML_IARCHIVE_HPP_INCLUDED

#include <boost/s11n/detail/common_iarchive.hpp>
#include <boost/s11n/nvp.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <string>
#include <stdexcept>

namespace boost { namespace s11n
{

	class xml_iarchive: public detail::common_iarchive<xml_iarchive>
    {

        friend class detail::common_iarchive<xml_iarchive>;

    public:    
        
        xml_iarchive(std::istream &stream): 
            m_pt(), m_current(&m_pt)
        {
            read_xml(stream, m_pt);
        }
        
    private:    
        
        template<class T> void load(T &t)
        {
            // Trying to serialize non-nvp data
            BOOST_STATIC_ASSERT(sizeof(T) == 0);    
        }

        template<class T> void load(nvp<T> &t)
        {
            if (t.name)
            {
                boost::property_tree::iptree::iterator it = m_current->find(t.name);
                if (it != m_current->end())
                {
                    boost::property_tree::iptree *previous = m_current;
                    m_current = &it->second;
                    internal_load(t.value);
                    m_current = previous;
                    if (!it->second.empty())
                        throw std::runtime_error("unused value \"" + it->second.front().first + "\" left in ptree");
                    m_current->erase(it);
                }
                else
                    throw std::runtime_error(std::string("key \"") + t.name + "\" not found and no default value provided");
            }
            else
                internal_load(t.value);
        }

        template<class T> void load(default_nvp<T> &t)
        {
            if (t.name)
            {
                boost::property_tree::iptree::iterator it = m_current->find(t.name);
                if (it != m_current->end())
                {
                    boost::property_tree::iptree *previous = m_current;
                    m_current = &it->second;
                    internal_load(t.value);
                    m_current = previous;
                    if (!it->second.empty())
                        throw std::runtime_error("unused value \"" + it->second.front().first + "\" left in ptree");
                    m_current->erase(it);
                }
                else if (t.default_value)
                {
                    t.value = *t.default_value;
                }
                else
                    throw std::runtime_error(std::string("key \"") + t.name + "\" not found and no default value provided");
            }
            else
                internal_load(t.value);
        }

        template<class T> void load(concise_nvp<T> &t)
        {
            using namespace boost::property_tree;
            iptree &attr = const_cast<iptree &>(m_current->get_child("<xmlattr>", empty_ptree<iptree>()));
            iptree::iterator it = attr.find(t.name);
            if (it != attr.end())
            {
                t.value = it->second.get_value<T>();
                attr.erase(it);
                if (attr.empty())
                    m_current->erase("<xmlattr>");
            }
            else if (t.default_value)
            {
                t.value = *t.default_value;
            }
            else
                throw std::runtime_error(std::string("key \"") + t.name + "\" not found and no default value provided");
        }

        void load(class_name_t &t)
        {
            boost::property_tree::iptree::iterator it = m_current->find("ClassName");
            if (it != m_current->end())
            {
                t = m_current->get<std::string>("ClassName");
                m_current->erase(it);
            }
            else
                throw std::runtime_error("ClassName not found");
        }

        void load(container_size_t &t)
        {
            t = m_current->size();
        }

        template<class T> void load_primitive(T &t)
        {
            t = m_current->get_value<T>();
        }

		void load_primitive(raw_bytes &t)
        {
			std::string bytes = m_current->get_value<std::string>();
			BOOST_ASSERT(bytes.size() >= t.size);
			memcpy(t.ptr, bytes.data(), t.size);
			m_current->put_value(bytes.substr(t.size, std::string::npos));
        }

		template<class T> inline void internal_load(T &t);

        boost::property_tree::iptree m_pt;
        boost::property_tree::iptree *m_current;
        
    };

    template<class T> inline void xml_iarchive::internal_load(T &t)
    {
		detail::common_iarchive<xml_iarchive>::load(t);
    }
    
    template<> inline void xml_iarchive::internal_load<std::string>(std::string &t)
    {
        t = m_current->get_value<std::string>();
    }

} }

#endif
