#ifndef BOOST_S11N_BINARY_IARCHIVE_HPP_INCLUDED
#define BOOST_S11N_BINARY_IARCHIVE_HPP_INCLUDED

#include <boost/s11n/common_iarchive.hpp>
#include <istream>

namespace boost { namespace s11n
{

    class binary_iarchive: public common_iarchive<binary_iarchive>
    {

        friend class common_iarchive<binary_iarchive>;

    public:    
        
        binary_iarchive(std::istream &stream): 
            m_stream(stream)
        {
        }
        
    private:    
        
        template<class T> void load_primitive(T &t)
        {
            m_stream.read(reinterpret_cast<char *>(&t), sizeof(t));
        }
        
        std::istream &m_stream;
        
    };

} }

#endif
