/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_binary_oprimitive.ipp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . Permission to copy, 
// use, modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided "as is"
// without express or ied warranty, and with no claim as to its suitability
// for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#include <ostream>

#include <boost/config.hpp> // for BOOST_DEDUCED_TYPENAME

#include <cstring>
#if defined(BOOST_NO_STDC_NAMESPACE) && ! defined(__LIBCOMO__)
namespace std{ 
    using ::strlen; 
} // namespace std
#endif

#include <boost/scoped_ptr.hpp>

#include <boost/archive/wcslen.hpp>
#include <boost/archive/codecvt_null.hpp>
#include <boost/archive/add_facet.hpp>

namespace boost {
namespace archive {

//////////////////////////////////////////////////////////////////////
// implementation of basic_binary_oprimitive

template<class Archive, class OStream>
void basic_binary_oprimitive<Archive, OStream>::init()
{
    // record native sizes of fundamental types
    // this is to permit detection of attempts to pass
    // native binary archives accross incompatible machines.
    // This is not foolproof but its better than nothing.
    this->This()->save(static_cast<unsigned char>(sizeof(int)));
    this->This()->save(static_cast<unsigned char>(sizeof(long)));
    this->This()->save(static_cast<unsigned char>(sizeof(float)));
    this->This()->save(static_cast<unsigned char>(sizeof(double)));
    // for checking endianness
    this->This()->save(int(1));
}

template<class Archive, class OStream>
void basic_binary_oprimitive<Archive, OStream>::save(const char * s)
{
    std::size_t l = std::strlen(s);
    this->This()->save(l);
    save_binary(s, l);
}

template<class Archive, class OStream>
void basic_binary_oprimitive<Archive, OStream>::save(const wchar_t * ws)
{
    std::size_t l = std::wcslen(ws);
    this->This()->save(l);
    save_binary(ws, l * sizeof(wchar_t) / sizeof(char));
}

template<class Archive, class OStream>
void basic_binary_oprimitive<Archive, OStream>::save(const std::string &s)
{
    std::size_t l = static_cast<unsigned int>(s.size());
    this->This()->save(l);
    save_binary(s.data(), l);
}

#ifndef BOOST_NO_STD_WSTRING
template<class Archive, class OStream>
void basic_binary_oprimitive<Archive, OStream>::save(const std::wstring &ws)
{
    std::size_t l = ws.size();
    this->This()->save(l);
    save_binary(ws.data(), l * sizeof(wchar_t) / sizeof(char));
}
#endif

template<class Archive, class OStream>
basic_binary_oprimitive<Archive, OStream>::basic_binary_oprimitive(
    OStream &os_, 
    bool no_codecvt
) : 
    os(os_),
    locale_saver(os),
    archive_locale(NULL)
{
    if(! no_codecvt){
        archive_locale.reset(
            add_facet(
                std::locale::classic(), 
                new codecvt_null<BOOST_DEDUCED_TYPENAME OStream::char_type>
            )
        );
        os.imbue(* archive_locale);
    }
}

// scoped_ptr requires that g be a complete type at time of
// destruction so define destructor here rather than in the header
template<class Archive, class OStream>
basic_binary_oprimitive<Archive, OStream>::~basic_binary_oprimitive(){
    os.flush();
}

} // namespace archive
} // namespace boost
