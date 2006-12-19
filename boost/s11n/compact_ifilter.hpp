#ifndef BOOST_S11N_COMPACT_IFILTER_HPP_INCLUDED
#define BOOST_S11N_COMPACT_IFILTER_HPP_INCLUDED

#include <boost/s11n/detail/common_oarchive.hpp>
#include <boost/s11n/detail/compact_filter.hpp>
#include <ostream>

namespace boost { namespace s11n
{

    template<class Archive>
    class compact_ifilter: public detail::common_iarchive<compact_ifilter<Archive> >
    {

        friend detail::common_iarchive<compact_ifilter<Archive> >;

    public:    
        
        compact_ifilter(Archive &ar): 
            m_archive(ar)
        {
        }
        
    private:    
        
        ///////////////////////////////////////////////////////////////////////
        // Passthrough for all not specified types
        
        template<class T> void load_primitive(T &t)
        {
            m_archive & make_nvp(NULL, t);
        }

        ///////////////////////////////////////////////////////////////////////
        // Save operations for all supported types
        
        void load_primitive(unsigned short &t) { load_unsigned(t); }
        void load_primitive(unsigned int &t) { load_unsigned(t); }
        void load_primitive(unsigned long &t) { load_unsigned(t); }
		void load_primitive(boost::uint64_t &t) { load_unsigned(t); }

        void load_primitive(signed short &t) { load_signed(t); }
        void load_primitive(signed int &t) { load_signed(t); }
        void load_primitive(signed long &t) { load_signed(t); }
		void load_primitive(boost::int64_t &t) { load_signed(t); }

        ///////////////////////////////////////////////////////////////////////
        // Helpers
        
		template<class T> void load_unsigned(T &t)
		{
            unsigned char buffer[compact_filter_detail::max_buffer_size];
			for (int i = 0; i < compact_filter_detail::max_buffer_size; ++i)
			{
				m_archive & make_nvp(NULL, buffer[i]);
				if (compact_filter_detail::expand_unsigned(buffer, i + 1, t))
					break;
			}
		}
        
		template<class T> void load_signed(T &t)
		{
            unsigned char buffer[compact_filter_detail::max_buffer_size];
			for (int i = 0; i < compact_filter_detail::max_buffer_size; ++i)
			{
				raw_bytes raw(reinterpret_cast<char *>(buffer + i), 1);
				m_archive & make_nvp(NULL, raw);
				if (compact_filter_detail::expand_signed(buffer, i + 1, t))
					break;
			}
		}

        // Underlying archive
        Archive &m_archive;
        
    };

} }

#endif
