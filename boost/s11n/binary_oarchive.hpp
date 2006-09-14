#ifndef BOOST_S11N_BINARY_OARCHIVE_HPP_INCLUDED
#define BOOST_S11N_BINARY_OARCHIVE_HPP_INCLUDED

#include <boost/s11n/common_oarchive.hpp>
#include <ostream>

namespace boost { namespace s11n
{

    class binary_oarchive: public common_oarchive<binary_oarchive>
    {

        friend class common_oarchive<binary_oarchive>;

    public:    
        
        binary_oarchive(std::ostream &stream): 
            m_stream(stream)
        {
        }
        
    private:    
        
        template<class T> void save_primitive(T t)
        {
            m_stream.write(reinterpret_cast<char *>(&t), sizeof(t));
        }
        
        std::ostream &m_stream;
        
    };

} }

#endif
