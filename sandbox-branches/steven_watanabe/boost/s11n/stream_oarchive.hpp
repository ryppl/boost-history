#ifndef BOOST_S11N_STREAM_OARCHIVE_HPP_INCLUDED
#define BOOST_S11N_STREAM_OARCHIVE_HPP_INCLUDED

#include <boost/s11n/detail/common_oarchive.hpp>
#include <ostream>

namespace boost { namespace s11n
{

	class stream_oarchive: public detail::common_oarchive<stream_oarchive>
    {

        friend class detail::common_oarchive<stream_oarchive>;

    public:    
        
        ///////////////////////////////////////////////////////////////////////
        // Construction & destruction
        
        stream_oarchive(std::ostream &stream): 
            m_stream(stream)
        {
        }
        
    private:    
        
        ///////////////////////////////////////////////////////////////////////
        // Load operations
        
		// Generic save
        template<class T> void save_primitive(T t)
        {
            m_stream.write(reinterpret_cast<char *>(&t), sizeof(t));
        }
        
        // Save for raw bytes
        void save_primitive(const raw_bytes &t)
        {
            m_stream.write(t.ptr, static_cast<std::streamsize>(t.size));
        }

		// Underlying stream
        std::ostream &m_stream;
        
    };

} }

#endif
