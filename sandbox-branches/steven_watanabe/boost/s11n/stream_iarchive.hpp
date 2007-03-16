#ifndef BOOST_S11N_STREAM_IARCHIVE_HPP_INCLUDED
#define BOOST_S11N_STREAM_IARCHIVE_HPP_INCLUDED

#include <boost/s11n/detail/common_iarchive.hpp>
#include <istream>

namespace boost { namespace s11n
{

	class stream_iarchive: public detail::common_iarchive<stream_iarchive>
    {

        friend class detail::common_iarchive<stream_iarchive>;

    public:    
        
        ///////////////////////////////////////////////////////////////////////
        // Construction & destruction
        
        stream_iarchive(std::istream &stream): 
            m_stream(stream)
        {
        }
        
    private:    
        
        ///////////////////////////////////////////////////////////////////////
        // Load operations
        
		// Generic load
		template<class T> void load_primitive(T &t)
        {
            m_stream.read(reinterpret_cast<char *>(&t), sizeof(t));
        }
        
        // Load for raw bytes
		void load_primitive(raw_bytes &t)
        {
            m_stream.read(t.ptr, static_cast<std::streamsize>(t.size));
        }

		// Underlying stream
		std::istream &m_stream;
        
    };

} }

#endif
