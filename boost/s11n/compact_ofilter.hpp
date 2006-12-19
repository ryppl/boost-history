#ifndef BOOST_S11N_COMPACT_OFILTER_HPP_INCLUDED
#define BOOST_S11N_COMPACT_OFILTER_HPP_INCLUDED

#include <boost/s11n/detail/common_oarchive.hpp>
#include <boost/s11n/detail/compact_filter.hpp>
#include <ostream>

namespace boost { namespace s11n
{

    template<class Archive>
    class compact_ofilter: public detail::common_oarchive<compact_ofilter<Archive> >
    {

		friend typename detail::common_oarchive<compact_ofilter<Archive> >;

    public:    
        
        compact_ofilter(Archive &ar): 
            m_archive(ar)
        {
        }
        
    private:    
        
        ///////////////////////////////////////////////////////////////////////
        // Passthrough for all not specified types
        
        template<class T> void save_primitive(T t)
        {
            m_archive & make_nvp(NULL, t);
        }

        ///////////////////////////////////////////////////////////////////////
        // Save operations for all supported types
        
        void save_primitive(unsigned short t) { save_unsigned(t); }
        void save_primitive(unsigned int t) { save_unsigned(t); }
        void save_primitive(unsigned long t) { save_unsigned(t); }
		void save_primitive(boost::uint64_t t) { save_unsigned(t); }

        void save_primitive(signed short t) { save_signed(t); }
        void save_primitive(signed int t) { save_signed(t); }
        void save_primitive(signed long t) { save_signed(t); }
		void save_primitive(boost::int64_t t) { save_signed(t); }

        ///////////////////////////////////////////////////////////////////////
        // Helpers
        
		template<class T> void save_unsigned(T t)
		{
            unsigned char buffer[compact_filter_detail::max_buffer_size];
            int size = compact_filter_detail::compact_unsigned(buffer, t);
            raw_bytes raw(reinterpret_cast<char *>(buffer), size);
			m_archive & make_nvp(NULL, raw);
		}
        
		template<class T> void save_signed(T t)
		{
            unsigned char buffer[compact_filter_detail::max_buffer_size];
            int size = compact_filter_detail::compact_signed(buffer, t);
            raw_bytes raw(reinterpret_cast<char *>(buffer), size);
            m_archive & make_nvp(NULL, raw);
		}

        // Underlying archive
        Archive &m_archive;
        
    };

} }

#endif
